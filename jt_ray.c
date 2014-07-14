#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "jt_types.h"
#include "jt_vector.h"
#include "jt_ray.h"

jt_vector_t jt_point_on_ray (jt_ray_t *r, jt_float_t t)
{
    return (jt_vector_t) {r->origin.x + t * r->direction.x,
                          r->origin.y + t * r->direction.y,
                          r->origin.z + t * r->direction.z};
}
