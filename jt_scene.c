#include <stdio.h>
#include <stdlib.h>

#include "jt_types.h"
#include "jt_vector.h"
#include "jt_ray.h"
#include "jt_primitive.h"
#include "jt_scene.h"

/* We will also need to return the material... */
jt_float_t jt_scene_intersect (jt_scene_t *scene, jt_ray_t r, jt_vector_t *normal)
{
    jt_float_t closest = 0.0;
    jt_float_t this;
    int i;

    for (i = 0; i < scene->primitive_count; i++)
    {
        this = (scene->primitive[i].intersect) (&scene->primitive[i], r, normal);

        if (this != 0.0 && (closest == 0.0 || this < closest))
        {
            closest = this;
        }
    }

    return closest;
}
