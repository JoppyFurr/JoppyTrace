#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>

#include <SDL2/SDL.h>

#include "jt_types.h"
#include "jt_machine.h"
#include "jt_thread.h"
#include "jt_vector.h"
#include "jt_ray.h"
#include "jt_primitive.h"
#include "jt_colour.h"
#include "jt_scene.h"


extern jt_machine_t machine;

/*    Scratchpad - Code does not belong here     */
/*                                               */

jt_scene_t test_scene;

void jt_bake_test_scene ()
{
    test_scene.eye.x  =   0.0;
    test_scene.eye.y  =   0.0;
    test_scene.eye.z  =   0.0;

    test_scene.up.x   =   0.0;
    test_scene.up.y   =   1.0;
    test_scene.up.z   =   0.0;

    test_scene.lookat.x =   0.0;
    test_scene.lookat.y =   0.0;
    test_scene.lookat.z = -10.0;

    /* TODO: This is not currently using 'real' degrees */
    test_scene.fov    =  30.0;

    test_scene.primitive = malloc (2 * sizeof (jt_primitive_t));
    test_scene.primitive_count = 2;

    test_scene.primitive[0].intersect = jt_sphere_intersect;
    test_scene.primitive[0].sphere.centre.x =   0.0;
    test_scene.primitive[0].sphere.centre.y =   0.0;
    test_scene.primitive[0].sphere.centre.z = -10.0;
    test_scene.primitive[0].sphere.radius   =   4.0;

    test_scene.primitive[1].intersect = jt_sphere_intersect;
    test_scene.primitive[1].sphere.centre.x =   8.0;
    test_scene.primitive[1].sphere.centre.y =   0.0;
    test_scene.primitive[1].sphere.centre.z = -10.0;
    test_scene.primitive[1].sphere.radius   =   3.0;
}

jt_colour_t background_colour = {0.5, 0.5, 0.5};
jt_colour_t sphere_colour = {1.0, 0.0, 0.0};

jt_colour_t jt_cast_ray_at_test_scene (jt_ray_t r)
{
    jt_float_t ret;

    ret = jt_scene_intersect (&test_scene, r, NULL);

    if (ret == 0.0)
        return background_colour;

    return sphere_colour;
}

jt_colour_t jt_render_pixel (int x, int y)
{
    jt_vector_t pixel_position;
    jt_ray_t ray;
    /* Calculate the initial ray */
    /* Note:  Could this be simplified
     * by interpolation? */
    ray.origin = test_scene.eye;

    /* y component */
    pixel_position = jt_vector_add (test_scene.lookat,
                                    jt_vector_scale (test_scene.up, (y - 240.0) * (test_scene.fov / 640.0)));

    /* x component */
    pixel_position = jt_vector_add (
            pixel_position,
            jt_vector_scale (jt_vector_cross (test_scene.up, jt_vector_unit ( jt_vector_sub (test_scene.lookat, test_scene.eye))),
                             (x - 320.0) * (test_scene.fov / 640.0)));

    ray.direction = jt_vector_unit (jt_vector_sub (pixel_position, test_scene.eye));

    return jt_cast_ray_at_test_scene (ray);
}

void jt_still_do_chunk (uint32_t chunk)
{
    int x;
    jt_colour_t pixel;

    usleep (10000); /* some fake work... */

    for (x = 0; x < 640; x++)
    {
        pixel = jt_render_pixel (x, chunk);

        jt_write_colour_to_mem (
                &machine.pixels [640 * sizeof (uint32_t) * chunk + sizeof(uint32_t) * x],
                pixel);
    }

}
/*                                               */
/*    Scratchpad - Code does not belong here     */

void jt_render_still ()
{
    SDL_Event event;
    uint32_t render_start_time;
    uint32_t render_end_time = 0;
    uint32_t frame_start_time;
    uint32_t frame_end_time;

    /* TODO: Remove this once we have a parser... */
    jt_bake_test_scene ();

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
