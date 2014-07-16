#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "jt_types.h"
#include "jt_vector.h"

/* TODO: This code currently works by value. Is reference faster? */

jt_vector_t jt_vector_add (jt_vector_t *a, jt_vector_t *b)
{
    return (jt_vector_t) { a->x + b->x,
                           a->y + b->y,
                           a->z + b->z };
}

jt_vector_t jt_vector_cross (jt_vector_t *a, jt_vector_t *b)
{
    return (jt_vector_t) { a->y * b->z - b->y * a->z,
                           a->z * b->x - b->z * a->x,
                           a->x * b->y - b->x * a->y };
}

jt_float_t jt_vector_distance (jt_vector_t *a, jt_vector_t *b)
{
    return JT_SQRT ( (a->x - b->x) * (a->x - b->x)
                   + (a->y - b->y) * (a->y - b->y)
                   + (a->z - b->z) * (a->z - b->z));
}

jt_float_t jt_vector_dot (jt_vector_t *a, jt_vector_t *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

jt_vector_t jt_vector_reflection (jt_vector_t *d, jt_vector_t *n)
{
    jt_float_t normal_scale = -2.0 * jt_vector_dot (n ,d);

    return (jt_vector_t) { n->x * normal_scale + d->x,
                           n->y * normal_scale + d->y,
                           n->z * normal_scale + d->z };
}

jt_vector_t jt_vector_scale (jt_vector_t *v, jt_float_t *s)
{
    return (jt_vector_t) { v->x * *s,
                           v->y * *s,
                           v->z * *s };
}

jt_vector_t jt_vector_sub (jt_vector_t *a, jt_vector_t *b)
{
    return (jt_vector_t) { a->x - b->x,
                           a->y - b->y,
                           a->z - b->z };
}

jt_vector_t jt_vector_unit (jt_vector_t *v)
{
    jt_float_t length = JT_SQRT (v->x * v->x + v->y * v->y + v->z * v->z);
    return (jt_vector_t) {v->x / length,
                          v->y / length,
                          v->z / length};
}

jt_vector_t jt_vector_unit_sub (jt_vector_t *a, jt_vector_t *b)
{
    jt_vector_t sub_result = { a->x - b->x,
                               a->y - b->y,
                               a->z - b->z };
    jt_float_t length = JT_SQRT ( sub_result.x * sub_result.x
                                + sub_result.y * sub_result.y
                                + sub_result.z * sub_result.z);

    return (jt_vector_t) { sub_result.x / length,
                           sub_result.y / length,
                           sub_result.z / length };
}
