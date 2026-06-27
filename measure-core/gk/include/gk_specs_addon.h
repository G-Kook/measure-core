#ifndef GK_SPECS_ADDON
#define GK_SPECS_ADDON

#include <stdbool.h>
#include "gk_core.h"
#include "gkstr.h"

/** FWK GLOBALS */
gkstr gkfwk_tplname();
gkstr gkfwk_fwkname();
gkstr gkfwk_algname();
gkstr gkfwk_opname();
gkstr gkfwk_lotid();
gkstr gkfwk_itemname();
gkstr gkfwk_slotid();
int gkfwk_diex();
int gkfwk_diey();
gkstr gkfwk_testname();

void gkfwk_on_system_init();
void gkfwk_on_fkw_begin(const char* algname);
void gkfwk_on_fwk_end(const char* algname);
void gkfwk_on_taginfo_set(const char* tplname, const char* fwkname, const char* algname, const char* opname, const char* savdir, const char* itemname, const char* lotid);
void gkfwk_on_loop_begin();
void gkfwk_on_wafer_begin(const char* slotid, const char* wafermap);
void gkfwk_on_die_begin(int x, int y);
void gkfwk_on_test_begin(const char* testname);
void gkfwk_on_wafer_end();

void et_v_apply_ex(int port, double a, double d, double w);
void et_v_apply(int port, double v);
double et_i_measure_ex(int port, double range);
double et_i_measure(int port);
void et_pulse_apply(size_t count, double period);

#define pg_setup_spgu() spgu_mode(0)
void pg_set2p(int port, double a, double d, double w);
void pg_set2p_ex(int port, double a, double d, double le, double w, double te);
void pg_set3p(int port, double a1, double d1, double w1, double a2, double d2, double w2);
void pg_set3p_ex(int port, double a1, double d1, double le1, double w1, double te1, double a2, double d2, double le2, double w2, double te2);

#define alwg_setup_dpgu() spgu_mode(1)
void alwg_set_path(const char* fpath);

#define highspeed_adc_link_smu(port) set_smu_ch(port, 0, 0)
#define highspeed_adc_integ_manual() set_adc(0, 0, 0, 0)
#define highspeed_adc_integ_short() set_adc(0, 1, 0, 0)
#define highspeed_adc_integ_medium() set_adc(0, 2, 0, 0)
#define highspeed_adc_integ_long() set_adc(0, 3, 0, 0)
#define highres_adc_link_smu(port) set_smu_ch(port, 1, 0)
#define highres_adc_integ_manual() set_adc(1, 0, 0, 1)
#define highres_adc_integ_short() set_adc(1, 1, 0, 1)
#define highres_adc_integ_medium() set_adc(1, 2, 0, 1)
#define highres_adc_integ_long() set_adc(1, 3, 0, 1)

#endif
