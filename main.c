#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

typedef enum {JT_STATE_INIT, JT_STATE_RUNNING, JT_STATE_ABORT} jt_state_t;
typedef enum {JT_MODE_INIT, JT_MODE_STILL, JT_MODE_ANIMATION} jt_mode_t;

typedef struct jt_machine_s
{
    jt_state_t state;
    jt_mode_t  mode;
    SDL_Window *window;
    SDL_Renderer *renderer;
} jt_machine_t;

jt_machine_t machine;

void initialize ()
{
    machine.state = JT_STATE_INIT;

    if (SDL_Init (SDL_INIT_EVERYTHING) == -1)
    {
        fprintf (stderr, "Error: SDL_Init failed.\n");
        machine.state = JT_STATE_ABORT;
        return;
    }

    SDL_CreateWindowAndRenderer (0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &machine.window, &machine.renderer);
    if (machine.window == NULL || machine.renderer == NULL)
    {
        fprintf (stderr, "Error: SDL_CreateWindowAndRenderer failed.\n");
        machine.state = JT_STATE_ABORT;
        return;
    }

    /* Clear the screen */
    SDL_SetRenderDrawColor (machine.renderer, 0, 0, 0, 255);
    SDL_RenderClear (machine.renderer);
    SDL_RenderPresent (machine.renderer);

}

void cleanup ()
{
    if (machine.renderer)
        SDL_DestroyRenderer (machine.renderer);

    if (machine.window)
        SDL_DestroyWindow (machine.window);

    SDL_Quit();
}

int main (int argc, char **argv)
{
    fprintf (stdout, "JoppyTrace %s\n", JT_VERSION);

    initialize ();

    usleep (5000000);

    cleanup ();

    return EXIT_SUCCESS;
}
