#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>

#include <SDL2/SDL.h>

#include "jt_machine.h"
#include "jt_render.h"
#include "jt_thread.h"

jt_machine_t machine;

void jt_initialize ()
{
    machine.state = JT_STATE_INIT;
    machine.mode = JT_MODE_STILL;
    machine.debug = JT_TRUE;
    machine.width = 640;
    machine.height = 480;
    machine.thread_count = 4;

    if (SDL_Init (SDL_INIT_EVERYTHING) == -1)
    {
        fprintf (stderr, "Error: SDL_Init: %s\n", SDL_GetError ());
        machine.state = JT_STATE_ABORT;
        return;
    }

    SDL_CreateWindowAndRenderer (0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
                                 &machine.window, &machine.renderer);
    if (machine.window == NULL || machine.renderer == NULL)
    {
        fprintf (stderr, "Error: SDL_CreateWindowAndRenderer: %s\n", SDL_GetError ());
        machine.state = JT_STATE_ABORT;
        return;
    }

    machine.texture = SDL_CreateTexture (machine.renderer, SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_STATIC, machine.width, machine.height);
    if (machine.texture == NULL)
    {
        fprintf (stderr, "Error: SDL_CreateTexture: %s\n", SDL_GetError ());
        machine.state = JT_STATE_ABORT;
        return;
    }

    /* Clear the screen */
    SDL_SetRenderDrawColor (machine.renderer, 0, 0, 0, 255);
    SDL_RenderClear (machine.renderer);
    SDL_RenderPresent (machine.renderer);

    /* Initialize threads */
    jt_create_threads ();

    machine.state = JT_STATE_READY;
}

void jt_cleanup ()
{
    jt_join_threads ();

    if (machine.renderer)
        SDL_DestroyRenderer (machine.renderer);

    if (machine.window)
        SDL_DestroyWindow (machine.window);

    //if (machine.texture)
    //    SDL_FreeTexture (machine.texture);

    SDL_Quit();
}

int main (int argc, char **argv)
{
    fprintf (stdout, "JoppyTrace %s\n", JT_VERSION);

    jt_initialize ();

    if (machine.state = JT_STATE_READY)
    {
        switch (machine.mode)
        {
            case JT_MODE_STILL:
                jt_render_still ();
                break;

            case JT_MODE_ANIMATION:
                fprintf (stderr, "Error: Animation mode not yet implemented.\n");
                machine.state = JT_STATE_ABORT;
                break;

            default:
                fprintf (stderr, "Error: Invalid mode.\n");
                machine.state = JT_STATE_ABORT;
                break;
        }
    }

    jt_cleanup ();

    if (machine.state == JT_STATE_ABORT)
        return EXIT_FAILURE;
    else
        return EXIT_SUCCESS;
}
