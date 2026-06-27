#ifndef GK_CORE
#define GK_CORE

#include <stddef.h>
#include <stdbool.h>

/** CONVENIENT MACROS */
#define ABS(a) ( ((a)<0) ? (-(a)) : (a))
#define MIN(a, b) ( ((a)<(b)) ? (a) : (b))
#define MAX(a, b) ( ((a)<(b)) ? (b) : (a))
#define CLAMP(x, a, b) ( ((x)<(a)) ? (a) : ( ((x)<(b)) ? (x) : (b)))
#define SGN(n) (((n) < 0) ? -1 : 1)
#define FORCE_EXPAND(x) x
#define GET_3RD_ARG(_1,_2,_3,...) _3
#define GET_4TH_ARG(_1,_2,_3,_4,...) _4
#define GET_7TH_ARG(_1,_2,_3,_4,_5,_6,_7,...) _7
#define GET_50TH_ARG(\
    _1,_2,_3,_4,_5,_6,_7,_8,_9,_10,\
    _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
    _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
    _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
    _41,_42,_43,_44,_45,_46,_47,_48,_49,_50,N,...) N
#define GET_NARG(...)\
    FORCE_EXPAND(GET_50TH_ARG(\
        __VA_ARGS__,49,48,47,46,45,44,43,42,41,40,\
        39,38,37,36,35,34,33,32,31,30,\
        29,28,27,26,25,24,23,22,21,20,\
        19,18,17,16,15,14,13,12,11,10,\
        9,8,7,6,5,4,3,2,1,0))

/** FUNCTIONS FOR BASIC FUNCTIONALITIES */
void assertf(bool statement, const char* format, ...);
double dround(double v, size_t d);
void sleep_sec(double sec);

#define DEFINE_STATIC_OBJECT_GETTER(type, _funcname, init_val)\
static type* _funcname()\
{\
    static bool init;\
    static type dat;\
    if (!init)\
    {\
        init = true;\
        dat = init_val;\
    }\
    return &dat;\
}

#endif
