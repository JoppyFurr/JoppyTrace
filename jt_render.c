#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <pthread.h>
#include <semaphore.h>

#include <SDL2/SDL.h>

#include "jt_types.h"
#include "jt_machine.h"
#include "jt_thread.h"
#include "jt_vector.h"
#include "jt_ray.h"
#include "jt_colour.h"
#include "jt_material.h"
#include "jt_primitive.h"
#include "jt_scene.h"


extern jt_machine_t machine;

/* TODO: Figure out something tidy for this... */
jt_scene_t scene;

/*    Scratchpad - Code does not belong here     */
/*                                               */

void jt_bake_test_scene ()
{
    scene.eye    = (jt_vector_t) {0.0, 0.0,   0.0};
    scene.up     = (jt_vector_t) {0.0, 1.0,   0.0};
    scene.lookat = (jt_vector_t) {0.0, 0.0, -10.0};
    scene.fov    = 30.0 / 0.0174532925;


    /* TODO: malloc without free */
    scene.material = malloc (3 * sizeof (jt_material_t));
    scene.material_count = 3;

    /* Red */
    scene.material[0].colour = (jt_colour_t) {1.0, 0.0, 0.0};

    /* Yellow */
    scene.material[1].colour = (jt_colour_t) {0.8, 0.8, 0.0};

    /* Green */
    scene.material[2].colour = (jt_colour_t) {0.0, 0.5, 0.0};

    /* TODO: malloc without free */
    scene.primitive = malloc (3 * sizeof (jt_primitive_t));
    scene.primitive_count = 3;

    /* Sphere 1 */
    scene.primitive[0].intersect = jt_sphere_intersect;
    scene.primitive[0].material = &scene.material[0];
    scene.primitive[0].sphere.centre  = (jt_vector_t) {0.0, 0.0, -10.0};
    scene.primitive[0].sphere.radius   =   0.4;

    /* Sphere 2 */
    scene.primitive[1].intersect = jt_sphere_intersect;
    scene.primitive[1].material = &scene.material[1];
    scene.primitive[1].sphere.centre  = (jt_vector_t) {1.0, 0.4, -10.0};
    scene.primitive[1].sphere.radius   =   0.4;

    /* Sphere 3 - Planet */
    scene.primitive[2].intersect = jt_sphere_intersect;
    scene.primitive[2].material = &scene.material[2];
    scene.primitive[2].sphere.centre  = (jt_vector_t) {0.0, -101.0, -10.0};
    scene.primitive[2].sphere.radius   =   100.0;
}

jt_colour_t background_colour = {0.5, 0.5, 0.6};

jt_colour_t jt_cast_ray_at_test_scene (jt_ray_t r)
{
    jt_float_t ret;
    jt_material_t material;

    ret = jt_scene_intersect (&scene, r, NULL, &material);

    if (ret == 0.0)
        return background_colour;

    return material.colour;
}

jt_colour_t jt_render_pixel (int x, int y)
{
    jt_vector_t pixel_position;
    jt_ray_t ray;
    /* Calculate the initial ray */
    /* Note:  Could this be simplified
     * by interpolation? */
    ray.origin = scene.eye;

    jt_float_t picture_width = jt_vector_distance (scene.eye, scene.lookat) * JT_TAN (scene.fov);

    /* y component */
    pixel_position = jt_vector_add (scene.lookat,
                                    jt_vector_scale (scene.up, (machine.height / 2.0 - y) * (picture_width / machine.width)));

    /* x component */
    pixel_position = jt_vector_add (
            pixel_position,
            jt_vector_scale (jt_vector_cross (jt_vector_unit ( jt_vector_sub (scene.lookat, scene.eye)), scene.up),
                             (x - machine.width / 2.0) * (picture_width / machine.width)));

    ray.direction = jt_vector_unit (jt_vector_sub (pixel_position, scene.eye));

    return jt_cast_ray_at_test_scene (ray);
}

void jt_still_do_chunk (uint32_t chunk)
{
    int x;
    jt_colour_t pixel;

    for (x = 0; x < machine.width; x++)
    {
        pixel = jt_render_pixel (x, chunk);

        jt_write_colour_to_mem (
                &machine.pixel_data [machine.width * sizeof (uint32_t) * chunk + sizeof(uint32_t) * x],
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
    machine.work_total_chunks = machine.height;
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

        if (SDL_UpdateTexture (machine.texture, NULL, machine.pixel_data, machine.width * sizeof (uint32_t)))
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
