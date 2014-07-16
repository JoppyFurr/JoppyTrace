typedef struct jt_material_s
{
    jt_colour_t colour;   /* Colour used in ambient / diffuse calulations */
    jt_float_t  specular; /* Brightness of specular component */
    jt_float_t  shine;    /* Tightness of focus of the specular component */
    jt_float_t  reflection; /* Reflection ray component */
    /* Transmission value + refractive index */
} jt_material_t;
