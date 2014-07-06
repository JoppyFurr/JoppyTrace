#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>

#include <SDL2/SDL.h>

#include "jt_machine.h"
#include "jt_thread.h"

extern jt_machine_t machine;

void jt_still_do_chunk (uint32_t chunk)
{
    unsigned char pixels[640 * sizeof (uint32_t)];
    int x;

    usleep (100000); /* some fake work... */

    for (x = 0; x < 640; x++)
    {
        machine.pixels [640 * sizeof (uint32_t) * chunk + sizeof(uint32_t) * x + 0] = 128;
        machine.pixels [640 * sizeof (uint32_t) * chunk + sizeof(uint32_t) * x + 1] = 128;
        machine.pixels [640 * sizeof (uint32_t) * chunk + sizeof(uint32_t) * x + 2] = 128;
        machine.pixels [640 * sizeof (uint32_t) * chunk + sizeof(uint32_t) * x + 3] = 255;
    }

}

void jt_render_still ()
{
    SDL_Event event;
    uint32_t render_start_time;
    uint32_t render_end_time = 0;
    uint32_t frame_start_time;
    uint32_t frame_end_time;

    /* Kick-off rendering */
    machine.work_do_chunk = jt_still_do_chunk;
    machine.work_total_chunks = 480;
    machine.state = JT_STATE_RUNNING;
    render_start_time = SDL_GetTicks ();
    jt_post_threads (); /* kick */

    /* Wait for the user to quit */
    while (machine.state == JT_STATE_RUNNING || machine.state == JT_STATE_READY)
    {
        frame_start_time = SDL_GetTicks ();

        /* Process events */
        while (SDL_PollEvent (&event))
        {
            if (event.type == SDL_QUIT)
            {
                machine.state = JT_STATE_QUIT;
            }
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        machine.state = JT_STATE_QUIT;
                        break;
                    default:
                        break;
                }
            }

        }

        /* Rendering logic */
        if (render_end_time == 0 && machine.state == JT_STATE_READY)
        {
            render_end_time = SDL_GetTicks();
            if (machine.debug)
            {
                fprintf (stdout, "Render took %d ms.\n", render_end_time - render_start_time);
            }
        }

        if (SDL_UpdateTexture (machine.texture, NULL, machine.pixels, machine.width * sizeof (uint32_t)))
        {
            fprintf (stderr, "Error: SDL_UpdateTexture: %s\n", SDL_GetError ());
            machine.state = JT_STATE_ABORT;
            return;
        }

        SDL_RenderClear(machine.renderer);
        SDL_RenderCopy(machine.renderer, machine.texture, NULL, NULL);
        SDL_RenderPresent (machine.renderer);

        /* Frame limiting */
        frame_end_time = SDL_GetTicks ();

        if (frame_end_time - frame_start_time < 16)
        {
            SDL_Delay(16 - (frame_end_time - frame_start_time));
        }
    }
}
