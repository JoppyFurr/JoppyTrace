typedef struct jt_scene_s
{
    jt_vector_t up;
    jt_vector_t eye;
    jt_vector_t lookat;
    jt_float_t fov;

    /* jt_material_t */

    jt_primitive_t *primitive; /* For now.. A linear array will do */
    int primitive_count; /* TODO: Sort out things like uint32_t... Might need to switch to C99.. So why not just use C11? */

} jt_scene_t;

jt_float_t jt_scene_intersect (jt_scene_t *scene, jt_ray_t r, jt_vector_t *normal);
