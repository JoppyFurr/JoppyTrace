
typedef enum /* TODO: Now that we have it.. Use it */
{
    JT_FALSE = 0,
    JT_TRUE
} jt_bool_t;

#define USE_DOUBLE 1

#ifdef USE_DOUBLE

typedef double jt_float_t;
#define JT_SQRT sqrt
#define JT_TAN tan

#else

typedef float jt_float_t;
#define JT_SQRT sqrtf
#define JT_TAN tanf

#endif /* USE_DOUBLE */
