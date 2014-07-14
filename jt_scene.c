#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "jt_types.h"
#include "jt_vector.h"
#include "jt_ray.h"
#include "jt_colour.h"
#include "jt_material.h"
#include "jt_primitive.h"
#include "jt_scene.h"

/* TODO: Even though pointers are used.. A copy is made of normal and material D: */
jt_float_t jt_scene_intersect (jt_scene_t *scene, jt_ray_t *r, jt_vector_t *normal, jt_material_t *material)
{
    jt_float_t closest = 0.0;
    jt_float_t this;
    jt_vector_t this_normal; /* TODO: Index or pointer instead of vector? */
    int i;

    for (i = 0; i < scene->primitive_count; i++)
    {
        this = (scene->primitive[i].intersect) (&scene->primitive[i], r, &this_normal);

        if (this != 0.0 && (closest == 0.0 || this < closest))
        {
            closest = this;
            *material = *scene->primitive[i].material;
            *normal = this_normal;
        }
    }

    return closest;
}
