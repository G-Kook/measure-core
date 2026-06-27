#include <stdlib.h>
#include "specs_dummy.h"
#include "gkstr.h"
#include "gkio.h"
#include "gk_specs_addon.h"
#include "device_base.h"

static const DeviceDef** _device_def() { static const DeviceDef* dat; return &dat; }
static const DeviceDef* device_def() { return *_device_def(); }
void set_device_def(const DeviceDef* def) { *_device_def() = def; }

static connect_bls(int port, const char* bls)
{
	for (size_t bdx = gkmask_begin(bls); bdx != gkmask_end(); bdx = gkmask_next(bls, bdx))
		connect_pin(port, device_def()->BL[bdx]);
}

static connect_wls(int port, const char* wls)
{
	for (size_t wdx = gkmask_begin(wls); wdx != gkmask_end(); wdx = gkmask_next(wls, wdx))
		connect_pin(port, device_def()->WL[wdx]);
}


static gkstr make_bls_select(size_t bdx) { return gkmask_set((const char*)&device_def()->bls_empty, bdx); }
static gkstr make_wls_select(size_t wdx) { return gkmask_set((const char*)&device_def()->wls_empty, wdx); }

static gkstr make_bl_name(size_t bdx) { return gkstr_printf("BL%02d", bdx); }
static gkstr make_wl_name(size_t wdx) { return gkstr_printf("WL%02d", wdx); }

/** FOR ADVANCED USERS: ADJUST OPERATION PARAMETERS DIRECTLY */
void draw_idvg_base(OpParams_Idvg op_params)
{
	recordf("## Id-Vg\n");
	recordf(" %-10s %-10s %-10s %-10s\n", "BL", "WL", "V_G[V]", "I_DS[A]");
	connect_bls(PORT(0, 9), (const char*)&device_def()->bls_any);
	connect_wls(PORT(0, 2), (const char*)&device_def()->wls_monitor);
	connect_wls(PORT(0, 3), (const char*)&device_def()->wls_special);
	for (size_t bdx = gkmask_begin((const char*)&device_def()->bls_any); bdx != gkmask_end(); bdx = gkmask_next((const char*)&device_def()->bls_any, bdx))
	{
		const gkstr bls_select = make_bls_select(bdx);
		const gkstr bl_name = make_bl_name(bdx);
		connect_bls(PORT(0, 1), (const char*)&bls_select);
		for (size_t wdx = gkmask_begin((const char*)&device_def()->wls_monitor); wdx != gkmask_end(); wdx = gkmask_next((const char*)&device_def()->wls_monitor, wdx))
		{
			const gkstr wls_select = make_wls_select(wdx);
			const gkstr wl_name = make_wl_name(wdx);
			connect_wls(PORT(0, 4), (const char*)&wls_select);

			for (double vg = op_params.vg_begin; vg < op_params.vg_end; vg += op_params.vg_step)
			{
				et_v_apply(PORT(0, 1), op_params.vd);
				et_v_apply(PORT(0, 4), vg);
				const double ids = et_i_measure(PORT(0, 1));
				recordf(" %-10s %-10s %-10.3lf %-10.3e\n", bl_name, wl_name, vg, ids);
			}

			connect_wls(PORT(0, 2), (const char*)&wls_select);
		}
		connect_bls(PORT(0, 9), (const char*)&bls_select);
	}
	recordf("## END\n");
}

/** WRAPPER FOR THE BASE MEASUREMENT: USE OPERATION PARAMETER THAT IS SETUP FOR THE GENERATION. */
void draw_idvg()
{
	draw_idvg_base(device_def()->op_params_idvg);
}