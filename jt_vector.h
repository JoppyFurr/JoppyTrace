typedef struct jt_vector_s
{
    jt_float_t x;
    jt_float_t y;
    jt_float_t z;
} jt_vector_t;

jt_vector_t jt_cross_product (jt_vector_t a, jt_vector_t b);

jt_float_t jt_dot_product (jt_vector_t a, jt_vector_t b);

jt_vector_t jt_vector_unit (jt_vector_t v);
