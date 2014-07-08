#define USE_DOUBLE 1

/* There is probably a better way to get this... */
typedef unsigned char uint8_t;

#ifdef USE_DOUBLE

typedef double jt_float_t;
#define JT_SQRT sqrt

#else

typedef float jt_float_t;
#define JT_SQRT sqrtf

#endif /* USE_DOUBLE */
