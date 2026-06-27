#ifndef SPECS_DUMMY
#define SPECS_DUMMY

int connect_pin(int port, int pin);
int disable_port_all();
int disconnect_all();
int force_v(int port, double voltage, double range, double compliance);
int force_pg(double count, double period);
int measure_i(int port, double* current, double range);
int set_type_pg(int port, int mode);
int set_level_pg(int port, double a1, double a2, double base, double impedance);
int set_time_pg(int port, double w1, double d1, double le1, double te1, double w2, double d2, double le2, double te2);
int set_time_pg1(int port, double w1, double d1, double le1, double te1);
int sync_th(double wait_time);
int spgu_mode(int mode);
int spgu_alwg_force(double repeat);
int spgu_alwg_load(char* filename);
int spgu_alwg_r(int port, double impedance);
int PORT(int type, int numb);
int set_adc(int adc, int mode, double value, int autozero);
int set_smu_ch(int port, int adc, int filter);

#endif
