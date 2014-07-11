#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "jt_types.h"
#include "jt_vector.h"

/* TODO: This code currently works by value. Is reference faster? */

jt_vector_t jt_vector_add (jt_vector_t a, jt_vector_t b)
{
    jt_vector_t result = {a.x + b.x, a.y + b.y, a.z + b.z};
    return result;
}

jt_vector_t jt_vector_cross (jt_vector_t a, jt_vector_t b)
{
    jt_vector_t result = {a.y * b.z - b.y * a.z,
                          a.z * b.x - b.z * a.x,
                          a.x * b.y - b.x * a.y};
    return result;
}

jt_float_t jt_vector_distance (jt_vector_t a, jt_vector_t b)
{
    return JT_SQRT ( (a.x - b.x) * (a.x - b.x)
                   + (a.y - b.y) * (a.y - b.y)
                   + (a.z - b.z) * (a.z - b.z));
}

jt_float_t jt_vector_dot (jt_vector_t a, jt_vector_t b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

jt_vector_t jt_vector_scale (jt_vector_t v, jt_float_t s)
{
    jt_vector_t result = {v.x * s, v.y * s, v.z * s};
    return result;
}

jt_vector_t jt_vector_sub (jt_vector_t a, jt_vector_t b)
{
    jt_vector_t result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return result;
}

jt_vector_t jt_vector_unit (jt_vector_t v)
{
    jt_float_t length = JT_SQRT (v.x * v.x + v.y * v.y + v.z * v.z);
    jt_vector_t result = {v.x / length, v.y / length, v.z / length};
    return result;
}
