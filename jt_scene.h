typedef struct jt_scene_s
{
    /* Camera */
    jt_vector_t     up;
    jt_vector_t     eye;
    jt_vector_t     lookat;
    jt_float_t      fov; /* TODO: Confirm that fov calculations are correct. */

    /* Lighting */
    /* TODO: Consider a more complex lighting model... */
    /* For now, directional white light only. */
    jt_vector_t     lighting_direction;
    jt_float_t      lighting_intensity;
    jt_float_t      lighting_ambient;
    jt_colour_t     background;

    /* Scene contents */
    jt_material_t  *material;
    uint32_t        material_count;
    jt_primitive_t *primitive; /* For now.. A linear array will do */
    uint32_t        primitive_count;
} jt_scene_t;

jt_float_t jt_scene_intersect (jt_scene_t *scene, jt_ray_t *r, jt_vector_t *normal, jt_material_t *material);
