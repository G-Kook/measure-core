#ifndef DEVICE_BASE
#define DEVICE_BASE

enum ESomeScheme
{
    ESomeScheme_A,
    ESomeScheme_B
};
typedef enum ESomeScheme ESomeScheme;

struct OpParams_Idvg
{
    double vd;
    double vg_begin;
    double vg_end;
    double vg_step;
    double icomp;
};
typedef struct OpParams_Idvg OpParams_Idvg;

struct DeviceDef
{
    ESomeScheme some_scheme;

    const int* BL;
    const int* WL;
    gkstr bls_empty;
    gkstr bls_any;
    gkstr wls_empty;
    gkstr wls_monitor;
    gkstr wls_special;

    OpParams_Idvg op_params_idvg;
};
typedef struct DeviceDef DeviceDef;

void set_device_def(const DeviceDef* def);

/** FOR ADVANCED USERS: ADJUST OPERATION PARAMETERS DIRECTLY */
void draw_idvg_base(OpParams_Idvg op_params);

/** WRAPPER FOR THE BASE MEASUREMENT: USE OPERATION PARAMETER THAT IS SETUP FOR THE GENERATION. */
void draw_idvg();


#endif
