#ifndef GKSTR
#define GKSTR

#include "gk_core.h"

/** DEFINITION */
#define GKSTR_SIZE 1024
struct gkstr
{
    char dat[GKSTR_SIZE];
};
typedef struct gkstr gkstr;

/** MAKE */
size_t gkstr_capacity();
gkstr gkstr_make(const char* s);
gkstr gkstr_concat(const char* s, size_t arg_count, ...);
gkstr gkstr_printf(const char* format, ...);
gkstr gkstr_setat(const char* s, size_t idx, char c);
gkstr gkstr_fill(char c, size_t n);
gkstr gkstr_strip(const char* s);
gkstr gkstr_toupper(const char* s);
gkstr gkstr_tolower(const char* s);

/** INVESTIGATION */
size_t gkstr_len(const char* s);
char gkstr_getat(const char* s, size_t idx);

/** COMPARISON */
bool gkstr_eq(const char* s, const char* other);
bool _gkstr_isin(const char* s, size_t arg_count, ...);
#define gkstr_isin(s, ...) _gkstr_isin(s, GET_NARG(__VA_ARGS__(, __VA_ARGS__)
bool gkstr_issub(const char* c, const char* p);
bool gkstr_startswith(const char* s, const char* other);
bool gkstr_endswith(const char* s, const char* other);

/** MARKING CONVENTION */
#define __MARK 'x'
#define __UNMARK '-'
#define gkmask_test(s, i) (gkstr_getat(s, i) == __MARK)
gkstr gkmask_set(const char* s, size_t idx);
gkstr gkmask_reset(const char* s, size_t idx);

size_t gkmask_begin(const char* s);
size_t gkmask_end();
size_t gkmask_next(const char* s, size_t cur);
size_t gkmask_rbegin(const char* s);
size_t gkmask_rend();
size_t gkmask_rnext(const char* s, size_t cur);

gkstr _gkmask_union(const char* s1, size_t arg_count, ...);
#define gkmask_union(s1, ...) _gkmask_union(s1, GET_NARG(__VA_ARGS__), __VA_ARGS__)
gkstr _gkmask_intersect(const char* s1, size_t arg_count, ...);
#define gkmask_intersect(s1, ...) _gkmask_intersect(s1, GET_NARG(__VA_ARGS__), __VA_ARGS__)

#endif
