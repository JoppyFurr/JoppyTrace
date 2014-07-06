#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>

#include <SDL2/SDL.h>

#include "jt_machine.h"
#include "jt_render.h"

extern jt_machine_t machine;

void *jt_worker (void *data)
{
    int id = (uint64_t) data;
    int chunk;

    for (;;)
    {
        switch (machine.state)
        {
            /* No work yet */
            case JT_STATE_INIT:
            case JT_STATE_READY:
                sem_wait (&machine.wake_sem[id]);
                break;

            /* Do some work */
            case JT_STATE_RUNNING:

                pthread_mutex_lock (&machine.work_mutex);
                if (machine.work_next_chunk < machine.work_total_chunks)
                {
                    chunk = machine.work_next_chunk++;
                    pthread_mutex_unlock (&machine.work_mutex);

                    (*machine.work_do_chunk) (chunk);

                    pthread_mutex_lock (&machine.work_mutex);
                    machine.work_complete_chunks++;
                    pthread_mutex_unlock (&machine.work_mutex);

                    if (machine.work_complete_chunks == machine.work_total_chunks)
                        machine.state = JT_STATE_READY;
                }
                else
                {
                    pthread_mutex_unlock (&machine.work_mutex);
                    sem_wait (&machine.wake_sem[id]);
                }

                break;

            /* Bye-bye */
            case JT_STATE_QUIT:
            case JT_STATE_ABORT:
            default:
                pthread_exit (NULL);
                break;
        }
    }

    pthread_exit (NULL);
}

void jt_create_threads ()
{
    uint64_t i;

    machine.work_next_chunk = 0;
    machine.work_total_chunks = 0;
    machine.work_complete_chunks = 0;

    pthread_mutex_init (&machine.work_mutex, NULL);

    machine.thread = malloc (machine.thread_count * sizeof (pthread_t));
    machine.wake_sem = malloc (machine.thread_count * sizeof (sem_t));
    for (i = 0; i < machine.thread_count; i++)
    {
        sem_init (&machine.wake_sem[i], 0, 0);
        pthread_create (&machine.thread[i], NULL, jt_worker, (void *) i);
    }
}

void jt_post_threads ()
{
    int i;
    for (i = 0; i < machine.thread_count; i++)
    {
        sem_post (&machine.wake_sem[i]);
    }
}

void *jt_join_threads ()
{
    int i;

    jt_post_threads ();

    if (machine.debug)
        printf ("Debug: Joining threads...\n");

    for (i = 0; i < machine.thread_count; i++)
    {
        pthread_join (machine.thread[i], NULL);
        sem_destroy (&machine.wake_sem[i]);
    }

    if (machine.debug)
        printf ("Debug: Joining complete.\n");

    free (machine.wake_sem);
    free (machine.thread);

    pthread_mutex_destroy (&machine.work_mutex);
}
