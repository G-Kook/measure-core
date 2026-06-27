#include "string.h"
#include "gkdat.h"

static size_t* _wdx_size() { static size_t dat = 0; return &dat; }
static size_t* _pdx_size() { static size_t dat = 0; return &dat; }
static size_t* _bdx_size() { static size_t dat = 0; return &dat; }
static size_t wdx_size() { return *_wdx_size(); }
static size_t pdx_size() { return *_pdx_size(); }
static size_t bdx_size() { return *_bdx_size(); }

size_t gkdat_capacity() { return GKDATA_SIZE; }

void gkdat_setup(size_t wdx_count, size_t pdx_count, size_t bdx_count)
{
    *_wdx_size() = wdx_count;
    *_pdx_size() = pdx_count;
    *_bdx_size() = bdx_count;
}

gkdat gkdat_make()
{
    gkdat retval;
    memset(&retval, 0, sizeof(gkdat));
    return retval;
}

static void assert_idx(size_t wdx, size_t pdx, size_t bdx)
{
    assertf(wdx < wdx_size(), "<%s> wx(%zu) is out of range (>= %zu)\n", __FUNCTION__, wdx, wdx_size());
    assertf(pdx < pdx_size(), "<%s> wx(%zu) is out of range (>= %zu)\n", __FUNCTION__, pdx, pdx_size());
    assertf(bdx < bdx_size(), "<%s> wx(%zu) is out of range (>= %zu)\n", __FUNCTION__, bdx, bdx_size());
}

static size_t get_linear_idx(size_t wdx, size_t pdx, size_t bdx)
{
    assert_idx(wdx, pdx, bdx);
    return bdx + pdx * bdx_size() + wdx * pdx_size() * bdx_size();
}

void gkdat_setat(gkdat* gd, size_t wdx, size_t pdx, size_t bdx, double v)
{
    const size_t idx = get_linear_idx(wdx, pdx, bdx);
    gd->is_valid[idx] = true;
    gd->value[idx] = v;
}

bool gkdat_isvalid(gkdat* gd, size_t wdx, size_t pdx, size_t bdx)
{
    const size_t idx = get_linear_idx(wdx, pdx, bdx);
    return gd->is_valid[idx];
}

double gkdat_getat(gkdat* gd, size_t wdx, size_t pdx, size_t bdx)
{
    assertf(gkdat_isvalid(gd, wdx, pdx, bdx), "<%s> No data is available.\n", __FUNCTION__);
    const size_t idx = get_linear_idx(wdx, pdx, bdx);
    return gd->value[idx];
}

size_t gkdat_count_over(gkdat* gd, const char* wls, const char* planes, const char* bls)
{
    size_t count = 0;
    size_t wdx, pdx, bdx;

    for (wdx = gkmask_begin(wls); wdx != gkmask_end(); wdx = gkmask_next(wls, wdx))
    {
        for (pdx = gkmask_begin(planes); pdx != gkmask_end(); pdx = gkmask_next(planes, pdx))
        {
            for (bdx = gkmask_begin(bls); bdx != gkmask_end(); bdx = gkmask_next(bls, bdx))
            {
                if (gkdat_isvalid(gd, wdx, pdx, bdx)) count++;
            }
        }
    }
    return count;
}

bool gkdat_any_over(gkdat* gd, const char* wls, const char* planes, const char* bls)
{
    size_t wdx, pdx, bdx;

    for (wdx = gkmask_begin(wls); wdx != gkmask_end(); wdx = gkmask_next(wls, wdx))
    {
        for (pdx = gkmask_begin(planes); pdx != gkmask_end(); pdx = gkmask_next(planes, pdx))
        {
            for (bdx = gkmask_begin(bls); bdx != gkmask_end(); bdx = gkmask_next(bls, bdx))
            {
                if (gkdat_isvalid(gd, wdx, pdx, bdx)) return true;
            }
        }
    }
    return false;
}

double gkdat_avg_over(gkdat* gd, const char* wls, const char* planes, const char* bls)
{
    assertf(gkdat_any_over(gd, wls, planes, bls), "<%s> No data is available.\n", __FUNCTION__);
    double sum = 0;
    size_t count = 0;
    size_t wdx, pdx, bdx;
    for (wdx = gkmask_begin(wls); wdx != gkmask_end(); wdx = gkmask_next(wls, wdx))
    {
        for (pdx = gkmask_begin(planes); pdx != gkmask_end(); pdx = gkmask_next(planes, pdx))
        {
            for (bdx = gkmask_begin(bls); bdx != gkmask_end(); bdx = gkmask_next(bls, bdx))
            {
                if (gkdat_isvalid(gd, wdx, pdx, bdx))
                {
                    sum += gkdat_getat(gd, wdx, pdx, bdx);
                    count++;
                }
            }
        }
    }
    return sum / count;
}