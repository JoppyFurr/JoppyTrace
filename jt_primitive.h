/* TODO: Find the correct value for this */
#define JT_SPHERE_MARGIN 0.001

typedef struct jt_sphere_s
{
    jt_vector_t centre;
    jt_float_t radius;
} jt_sphere_t;

typedef struct jt_primitive_s
{
    jt_float_t  (*intersect) (struct jt_primitive_s *p, jt_ray_t *r, jt_vector_t *normal);
    jt_material_t *material;
    union
    {
        jt_sphere_t sphere;
    };
} jt_primitive_t;

jt_float_t jt_sphere_intersect (jt_primitive_t *p, jt_ray_t *r, jt_vector_t *normal);
