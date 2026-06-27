#ifndef GKDAT
#define GKDAT

#include <stdbool.h>
#include <stdlib.h>
#include "gk_core.h"
#include "gkstr.h"

#define GKDATA_SIZE 16384
struct gkdat
{
    bool is_valid[GKDATA_SIZE];
    double value[GKDATA_SIZE];
};
typedef struct gkdat gkdat;

/** DEFINITION */
size_t gkdat_capacity();
void gkdat_setup(size_t wsize, size_t psize, size_t bsize);
gkdat gkdat_make();

/** POINT */
void gkdat_setat(gkdat* gd, size_t wdx, size_t pdx, size_t bdx, double v);
bool gkdat_isvalid(gkdat* gd, size_t wdx, size_t pdx, size_t bdx);
double gkdat_getat(gkdat* gd, size_t wdx, size_t pdx, size_t bdx);

/** STATISTICS */
size_t gkdat_count_over(gkdat* gd, const char* wls, const char* planes, const char* bls);
bool gkdat_any_over(gkdat* gd, const char* wls, const char* planes, const char* bls);
double gkdat_avg_over(gkdat* gd, const char* wls, const char* planes, const char* bls);

#endif
