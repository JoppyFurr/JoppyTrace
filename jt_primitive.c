#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "jt_types.h"
#include "jt_vector.h"
#include "jt_ray.h"
#include "jt_colour.h"
#include "jt_material.h"
#include "jt_primitive.h"

jt_float_t jt_sphere_intersect (jt_primitive_t *p, jt_ray_t *r, jt_vector_t *normal)
{
    jt_sphere_t s = p->sphere;

    jt_float_t t0;
    jt_float_t t1;
    jt_vector_t q;
    jt_float_t vDotQ;
    jt_float_t root;
    jt_float_t squareDifference;
    jt_float_t discrim;

    q                = jt_vector_sub (s.centre, r->origin);
    vDotQ            = jt_vector_dot (r->direction, q);
    squareDifference = jt_vector_dot (q, q) - s.radius * s.radius;
    discrim          = vDotQ * vDotQ - squareDifference;

    if (discrim >= 0.0) {

        root = JT_SQRT (discrim);
        t0   = (vDotQ - root);
        t1   = (vDotQ + root);

        if (t0 > JT_SPHERE_MARGIN) {
            *normal = jt_vector_unit ( jt_vector_sub (jt_point_on_ray (r, t0), s.centre ));
            /* TODO: Will t0 evere equal 0.0? */
            return t0;
        }
        else if(t1 > JT_SPHERE_MARGIN) {
            *normal = jt_vector_unit ( jt_vector_sub (jt_point_on_ray (r, t1), s.centre ));
            /* TODO: Will t1 evere equal 0.0? */
            return t1;
        }
    }

    /* No intersection */
    return 0.0;
}
