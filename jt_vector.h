typedef struct jt_vector_s
{
    jt_float_t x;
    jt_float_t y;
    jt_float_t z;
} jt_vector_t;

jt_vector_t jt_vector_add      (jt_vector_t *a, jt_vector_t *b);
jt_vector_t jt_vector_cross    (jt_vector_t *a, jt_vector_t *b);
jt_float_t  jt_vector_distance (jt_vector_t *a, jt_vector_t *b);
jt_float_t  jt_vector_dot      (jt_vector_t *a, jt_vector_t *b);
jt_vector_t jt_vector_scale    (jt_vector_t *v, jt_float_t  *s);
jt_vector_t jt_vector_sub      (jt_vector_t *a, jt_vector_t *b);
jt_vector_t jt_vector_unit     (jt_vector_t *v);
jt_vector_t jt_vector_unit_sub (jt_vector_t *a, jt_vector_t *b);



