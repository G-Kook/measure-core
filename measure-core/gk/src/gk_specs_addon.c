#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS 1
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <time.h>
#include <limits.h>
#ifdef __linux__
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#elif _WIN32
//#include "dirent.h"
#include <Windows.h>
#endif
#ifdef __linux__
#include "/opt/hp4070/include/tis.h"
#include "/opt/SPECS/sys/include/SPECS.h"
#include "/opt/SPECS/sys/include/ALGUTIL.h"
#include "/opt/SPECS/sys/include/TOOLCTRL.h"
#include "/opt/SPECS/sys/include/UTILITY.h"
#include "/opt/SPECS/sys/include/PROBER.h"
#elif _WIN32
#include "specs_dummy.h"
#endif
#include "gkio.h"
#include "gktml.h"
#include "gk_specs_addon.h"

DEFINE_STATIC_OBJECT_GETTER(gkstr, _tplname, gkstr_make("UNDEFINED"))
gkstr gkfwk_tplname() { return *_tplname(); }

DEFINE_STATIC_OBJECT_GETTER(gkstr, _fwkname, gkstr_make("UNDEFINED"))
gkstr gkfwk_fwkname() { return *_fwkname(); }

DEFINE_STATIC_OBJECT_GETTER(gkstr, _algname, gkstr_make("UNDEFINED"))
gkstr gkfwk_algname() { return *_algname(); }

DEFINE_STATIC_OBJECT_GETTER(gkstr, _opname, gkstr_make("UNDEFINED"))
gkstr gkfwk_opname() { return *_opname(); }

DEFINE_STATIC_OBJECT_GETTER(gkstr, _lotid, gkstr_make("UNDEFINED"))
gkstr gkfwk_lotid() { return *_lotid(); }

DEFINE_STATIC_OBJECT_GETTER(gkstr, _itemname, gkstr_make("UNDEFINED"))
gkstr gkfwk_itemname() { return *_itemname(); }

DEFINE_STATIC_OBJECT_GETTER(gkstr, _slotid, gkstr_make("UNDEFINED"))
gkstr gkfwk_slotid() { return *_slotid(); }

static int* _diex() { static int dat = INT_MIN; return &dat; }
int gkfwk_diex() { return *_diex(); }
static int* _diey() { static int dat = INT_MIN; return &dat; }
int gkfwk_diey() { return *_diey(); }

DEFINE_STATIC_OBJECT_GETTER(gkstr, _testname, gkstr_make("UNDEFINED"))
gkstr gkfwk_testname() { return *_testname(); }

void gkfwk_on_system_init()
{
    start_logging(ELogChannel_FRAMEWORK);
    const gkstr cur_time = gkio_current_time_yyyymmdd();
    logprintf(ELogChannel_FRAMEWORK, "<%s> current system clock: %s\n", __FUNCTION__, &cur_time);
    logprintf(ELogChannel_FRAMEWORK, "<%s> start logging for framework operations.\n", __FUNCTION__);
}

void gkfwk_on_fwk_begin(const char* algname)
{
    const gkstr command = gkstr_printf("python /users/hp4071a/SPECS.py %s %s", "RUN", algname);
    system((const char*)&command);
    logprintf(ELogChannel_FRAMEWORK, "<%s> runs the following command: '%s'\n", __FUNCTION__, &command); 
}

void gkfwk_on_fwk_end(const char* algname)
{
    const gkstr command = gkstr_printf("python /users/hp4071a/SPECS.py %s %s", "IDLE", algname);
    system((const char*)&command);
    logprintf(ELogChannel_FRAMEWORK, "<%s> runs the following command: '%s'\n", __FUNCTION__, &command);
}

void gkfwk_on_taginfo_set(const char* tplname, const char* fwkname, const char* algname, const char* opname, const char* savdir, const char* itemname, const char* lotid)
{
    *_tplname() = gkstr_make(tplname);
    *_fwkname() = gkstr_make(fwkname);
    *_algname() = gkstr_make(algname);
    *_opname() = gkstr_make(opname);
    assertf(gkio_provide_directory(savdir), "<%s> Failed to create directory '%s'.\n", __FUNCTION__, savdir);
    gkio_set_savedir(savdir);
    *_itemname() = gkstr_make(itemname);
    const gkstr up_lotid = gkstr_toupper(lotid);
    *_lotid() = gkstr_make((const char*)&up_lotid);

    logprintf(ELogChannel_FRAMEWORK, "<%s> Test info is set as following:\n", __FUNCTION__);
    logprintf(ELogChannel_FRAMEWORK, "  TPLName:       %s\n", tplname);
    logprintf(ELogChannel_FRAMEWORK, "  FWKName:       %s\n", fwkname);
    logprintf(ELogChannel_FRAMEWORK, "  ALGName:       %s\n", algname);
    logprintf(ELogChannel_FRAMEWORK, "  OperatorName:  %s\n", opname);
    logprintf(ELogChannel_FRAMEWORK, "  SaveDir:       %s\n", savdir);
    logprintf(ELogChannel_FRAMEWORK, "  ItemName:      %s\n", itemname);
    logprintf(ELogChannel_FRAMEWORK, "  LOTID:         %s\n", &up_lotid);
}

void gkfwk_on_loop_begin()
{
    bool is_valid_dir = false;
    const gkstr savedir = gkio_savedir();
    const gkstr lotid = gkfwk_lotid();
    const gkstr cur_time = gkio_current_time_yyyymmdd_hhmmss();
    const gkstr itemname = gkfwk_itemname();
    const gkstr outdir_root = gkstr_printf("%s/%s/", &savedir, &lotid);
    const gkstr outdir = gkstr_printf("%s/%s/%s-%s", &savedir, &lotid, &cur_time, &itemname);
    assertf(gkio_provide_directory((const char*)&outdir_root), "<%s> Failed to create directory '%s'.\n", __FUNCTION__, &outdir_root);
    assertf(gkio_provide_directory((const char*)&outdir), "<%s> Failed to create directory '%s'.\n", __FUNCTION__, &outdir);
    gkio_set_outdir((const char*)&outdir);
    logprintf(ELogChannel_FRAMEWORK, "<%s> made directory '%s'\n", __FUNCTION__, &outdir);
}

void gkfwk_on_wafer_begin(const char* slotid, const char* wafermap)
{
    *_slotid() = gkstr_make(slotid);
    logprintf(ELogChannel_FRAMEWORK, "<%s> SlotID is set to '%s'\n", __FUNCTION__, slotid);

    const gkstr cachepath = gkio_cachepath();
    const gkstr command = gkstr_printf("uname -n > %s", &cachepath);
    system((const char*)&command);
    gkstr tester_name;
    FILE* file = fopen((const char*)&cachepath, "rb");
    if (file) { fgets((char*)&tester_name, (int)gkstr_capacity(), file), fclose(file); }
    else tester_name = gkstr_make("NO_INFO");
    logprintf(ELogChannel_FRAMEWORK, "<%s> host name is set to '%s'\n", __FUNCTION__, &tester_name);

    const gkstr outdir = gkio_outdir();
    const gkstr outpath = gkstr_printf("%s/%s", &outdir, slotid);
    assertf(gkio_provide_file((const char*)&outpath), "<%s> Failed to create file '%s'.\n", __FUNCTION__, &outpath);
    gkio_set_outpath((const char*)&outpath);
    
    const gkstr opname = gkfwk_opname();
    const gkstr tplname = gkfwk_tplname();
    const gkstr fwkname = gkfwk_fwkname();
    const gkstr algname = gkfwk_algname();
    const gkstr cur_time = gkio_current_time_yyyymmdd_hhmmss();
    recordf("[CRUISE MEASUREMENT DATA FROM C ALGORITHM]\n");
    recordf("  PATH       %s\n", &outpath);
    recordf("  OPERATOR   %s\n", &outpath);
    recordf("  TESTER     %s\n", &outpath);
    recordf("  TPL        %s\n", &outpath);
    recordf("  FRAMEWORK  %s\n", &outpath);
    recordf("  ALGORITHM  %s\n", &outpath);
    recordf("  WAFER_MAP  %s\n", &outpath);
    recordf("  START_TIME %s\n", &outpath);
    recordf("------------------------------------------------------------\n");
}

void gkfwk_on_die_begin(int x, int y)
{
    disable_and_disconnect_all();
    *_diex() = x;
    *_diey() = y;
    recordf("$LOC=(Y:%d,X:%d)\n", y, x);
}

void gkfwk_on_test_begin(const char* testname)
{
    *_testname() = gkstr_make(testname);
    recordf("$TEST=%s\n", testname);
}

void gkfwk_on_wafer_end()
{
    const gkstr cur_time = gkio_current_time_yyyymmdd_hhmmss();
    recordf("------------------------------------------------------------\n");
    recordf(" END TIME     %s\n\n", &cur_time);
}

void et_v_apply_ex(int port, double v, double range, double compliance)
{
    force_v(port, v, range, compliance);
}

void et_v_apply(int port, double v) { et_v_apply_ex(port, v, v, 1e-6); }

double et_i_measure_ex(int port, double range)
{
    double current;
    measure_i(port, &current, range);
    return current;
}

double et_i_measure(int port) { return et_i_measure_ex(port, 0); }

void et_pulse_apply(size_t count, double period) { force_pg((double)count, period); }

void pg_set2p(int port, double a, double d, double w) { pg_set2p_ex(port, a, d, (0.1*w), w, (0.1*w)); }
void pg_set2p_ex(int port, double a, double d, double le, double w, double te)
{
    set_type_pg(port, 1);
    set_level_pg(port, CLAMP(a, -40.l, 40.l), 0, 0, 0.l);
    set_time_pg1(port, (w+le), d, (le*0.8), (te*0.8));
}
void pg_set3p(int port, double a1, double d1, double w1, double a2, double d2, double w2) { pg_set3p_ex(port, a1, d1, (0.1*w1), w1, (0.1*w1), a2, d2, (0.1*w2), w2, (0.1*w2)); }
void pg_set3p_ex(int port, double a1, double d1, double le1, double w1, double te1, double a2, double d2, double le2, double w2, double te2)
{
    set_type_pg(port, 2);
    set_level_pg(port, CLAMP(a1, -40.l, 40.l), CLAMP(a2, -40.l, 40.l), 0, 0.l);
    le1 = MIN(le1, 25e-9);
    te1 = MIN(te1, 25e-9);
    le2 = MIN(le2, 25e-9);
    te2 = MIN(te2, 25e-9);
    set_time_pg(port, (w1+le1), d1, (le1*0.8), (te1*0.8), (w2+le2), d2, (le2*0.8), (te2*0.8));
}
void alwg_set_path(const char* fpath)
{
    if (gkio_file_exists(fpath)) spgu_alwg_load((char*)fpath);
    else fprintf(stdout, "ALWG waveform file does not exist.\n");
}

