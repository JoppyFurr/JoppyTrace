typedef struct jt_ray_s
{
    jt_vector_t origin;
    jt_vector_t direction;
} jt_ray_t;

jt_vector_t jt_point_on_ray (jt_ray_t *r, jt_float_t *t);
