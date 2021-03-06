#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "jt_types.h"
#include "jt_colour.h"

jt_colour_t jt_colour_product (jt_colour_t *a, jt_colour_t *b)
{
    return (jt_colour_t) { a->r * b->r,
                           a->g * b->g,
                           a->b * b->b };
}

/* Temporary */
void jt_write_colour_to_mem (uint8_t *base, jt_colour_t *c)
{
    base[3] = 255;
    base[2] = fmin (1.0, c->r) * 255; /* should we round()? */
    base[1] = fmin (1.0, c->g) * 255;
    base[0] = fmin (1.0, c->b) * 255;
}
