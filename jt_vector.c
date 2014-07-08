#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "jt_types.h"
#include "jt_vector.h"

jt_vector_t jt_cross_product (jt_vector_t a, jt_vector_t b)
{
    jt_vector_t result = {a.y * b.z - b.y * a.z,
                          a.z * b.x - b.z * a.x,
                          a.x * b.y - b.x * a.y};
    return result;
}

jt_float_t jt_dot_product (jt_vector_t a, jt_vector_t b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

jt_vector_t jt_vector_unit (jt_vector_t v)
{
    jt_float_t length = JT_SQRT (v.x * v.x + v.y * v.y + v.z * v.z);
    jt_vector_t result = {v.x / length, v.y / length, v.z / length};
    return result;
}
