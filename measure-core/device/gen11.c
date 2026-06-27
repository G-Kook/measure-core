#include "gkstr.h"
#include "device_base.h"
#include "gen11.h"

/** USER CONFIGURATION */
static ESomeScheme some_scheme() { return ESomeScheme_B; }
static const int* BL()    { static int BL[] = { 5, 6, 7, 8 }; return BL; }
static const int* WL()    { static int WL[] = { 1, 2, 3, 4, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 }; return WL; }
static gkstr bls_empty()   { return gkstr_make("----"); }
static gkstr bls_any()     { return gkstr_make("xxxx"); }
static gkstr wls_empty()   { return gkstr_make("--------------------"); }
static gkstr wls_monitor() { return gkstr_make("----xxxxxxxxxxxxxxxx"); }
static gkstr wls_special() { return gkstr_make("xxxx----------------"); }
static OpParams_Idvg op_params_idvg()
{
	OpParams_Idvg op_params = {
		.vd = 0.6,
		.vg_begin = -4,
		.vg_end = 5,
		.vg_step = 0.1,
		.icomp = 500e-9
	};
	return op_params;
}

/** DO NOT EDIT BELOW */
static DeviceDef* _device_def()
{
	static DeviceDef def;
	def.some_scheme = some_scheme();
	def.BL = BL();
	def.WL = WL();
	def.bls_empty = bls_empty();
	def.bls_any = bls_any();
	def.wls_empty = wls_empty();
	def.wls_monitor = wls_monitor();
	def.wls_special = wls_special();
	def.op_params_idvg = op_params_idvg();
	return &def;
}

static void setup()
{
	set_device_def(_device_def());
}

void gen11_draw_idvg()
{
	setup();
	draw_idvg();
}