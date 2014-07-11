#define USE_DOUBLE 1

/* There is probably a better way to get this... */
typedef unsigned char uint8_t;

#ifdef USE_DOUBLE

typedef double jt_float_t;
#define JT_SQRT sqrt
#define JT_TAN tan

#else

typedef float jt_float_t;
#define JT_SQRT sqrtf
#define JT_TAN tanf

#endif /* USE_DOUBLE */
