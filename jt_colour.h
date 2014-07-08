typedef struct jt_colour_s
{
    jt_float_t r;
    jt_float_t g;
    jt_float_t b;
} jt_colour_t;

/* Temporary */
void jt_write_colour_to_mem (uint8_t *base, jt_colour_t c);
