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
#include "jt_parse.h"
#include "jt_lighting.h"

extern jt_machine_t machine;

/* TODO: Figure out something tidy for this... */
jt_scene_t scene;

/*    Scratchpad - Code does not belong here     */
/*                                               */

jt_colour_t jt_cast_ray_at_test_scene (jt_ray_t *r)
{
    jt_float_t ret;
    jt_vector_t normal;
    jt_material_t material;

    /* TODO: normal and material are outputs.. How do we differentiate these from inputs? */
    ret = jt_scene_intersect (&scene, r, &normal, &material);

    if (ret == 0.0)
        return scene.background;

    return jt_phong_illumination (&material, r, &normal, &scene);
}

jt_colour_t jt_render_pixel (int x, int y)
{
    jt_vector_t pixel_position;
    jt_ray_t ray;
    /* Calculate the initial ray */
    /* Note:  Could this be simplified
     * by interpolation? */
    ray.origin = scene.eye;

    /* TODO: Is the distance even required? Assuming a distance of 1 may simplify the math */
    jt_float_t picture_width = jt_vector_distance (&scene.eye, &scene.lookat) * JT_TAN (scene.fov);

    /* y component */
    jt_float_t scene_y_movement_f  = (machine.height / 2.0 - y) * (picture_width / machine.width);
    jt_vector_t scene_y_movement_v = jt_vector_scale (&scene.up, &scene_y_movement_f);

    pixel_position = jt_vector_add (&scene.lookat, &scene_y_movement_v);

    /* x component */
    jt_float_t scene_x_movement_f   = (x - machine.width / 2.0) * (picture_width / machine.width);
    jt_vector_t scene_z_direction_v = jt_vector_unit_sub (&scene.lookat, &scene.eye);
    jt_vector_t scene_x_direction_v = jt_vector_cross (&scene_z_direction_v, &scene.up);
    jt_vector_t scene_x_movement_v  = jt_vector_scale (&scene_x_direction_v, &scene_x_movement_f);

    pixel_position = jt_vector_add (&pixel_position, &scene_x_movement_v);

    ray.direction = jt_vector_unit_sub (&pixel_position, &scene.eye);

    return jt_cast_ray_at_test_scene (&ray);
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
                &pixel);
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

    /* TODO: Yet another copy.. */
    /* TODO: Free what we malloc. scene, materials, primitives... */

    scene = *jt_parse_scene ("test_scene.jts");

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
