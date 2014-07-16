#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "jt_types.h"
#include "jt_vector.h"
#include "jt_ray.h"
#include "jt_colour.h"
#include "jt_material.h"
#include "jt_primitive.h" /* TODO: Do something about these includes... */
#include "jt_scene.h"
#include "jt_illumination.h"

jt_colour_t jt_ambient_illumination (jt_material_t *m, jt_ray_t *r, jt_vector_t *normal, jt_scene_t *scene)
{
    return (jt_colour_t) { m->colour.r * scene->lighting_ambient,
                           m->colour.g * scene->lighting_ambient,
                           m->colour.b * scene->lighting_ambient };
};

jt_colour_t jt_phong_illumination (jt_material_t *m, jt_ray_t *r, jt_vector_t *normal, jt_scene_t *scene)
{
    jt_colour_t result = { m->colour.r * scene->lighting_ambient,
                           m->colour.g * scene->lighting_ambient,
                           m->colour.b * scene->lighting_ambient };

    jt_float_t diffuse_factor;

    /* Diffuse */
    diffuse_factor = jt_vector_dot (&scene->lighting_direction, normal) * scene->lighting_intensity;
    diffuse_factor = fmax (0, diffuse_factor);
    result.r += m->colour.r * diffuse_factor;
    result.g += m->colour.g * diffuse_factor;
    result.b += m->colour.b * diffuse_factor;

    /* Specular */
    jt_vector_t halfway = jt_vector_unit_sub (&scene->lighting_direction, &r->direction);
    jt_float_t specular_factor = pow (jt_vector_dot (&halfway, normal), m->shine) * scene->lighting_intensity;
    result.r += m->specular * specular_factor;
    result.g += m->specular * specular_factor;
    result.b += m->specular * specular_factor;

    return result;
}
