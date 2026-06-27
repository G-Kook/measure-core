#include "specs_dummy.h"

int connect_pin(int port, int pin) { return 0; }
int disable_port_all() { return 0; }
int disconnect_all() { return 0; }
int force_v(int port, double voltage, double range, double compliance) { return 0; }
int force_pg(double count, double period) { return 0; }
int measure_i(int port, double* current, double range) { return 0; }
int set_type_pg(int port, int mode) { return 0; }
int set_level_pg(int port, double a1, double a2, double base, double impedance) { return 0; }
int set_time_pg(int port, double w1, double d1, double le1, double te1, double w2, double d2, double le2, double te2) { return 0; }
int set_time_pg1(int port, double w1, double d1, double le1, double te1) { return 0; }
int sync_th(double wait_time) { return 0; }
int spgu_mode(int mode) { return 0; }
int spgu_alwg_force(double repeat) { return 0; }
int spgu_alwg_load(char* filename) { return 0; }
int spgu_alwg_r(int port, double impedance) { return 0; }
int PORT(int type, int numb) { return 0; }
int set_adc(int adc, int mode, double value, int autozero) { return 0; }
int set_smu_ch(int port, int adc, int filter) { return 0; }
