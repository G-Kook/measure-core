#ifndef GKTML
#define GKTML

#include "gk_core.h"
#include "gkstr.h"

void gktml_init_44pin();

struct gktml;
typedef struct gktml gktml;

void gktml_register(const char* tag, size_t idx, const char* padname, int padid);
void gktml_reset();
const gktml* gktml_begin();
const gktml* gktml_end();
const gktml* gktml_next(const gktml* cur);
gkstr gktml_name(const gktml* t);
const gktml* gktml_get(const char* tag, size_t idx);
void gktml_print_pin_info();

/** CONNECTION */
void _connect_tml(int port, const size_t arg_count, ...);
#define connect_tml(port, ...) _connect_tml(port, GET_NARG(__VA_ARGS__), __VA_ARGS__)
void _connect_any(int port, const size_t arg_count, ...);
#define connect_any(port, ...) _connect_any(port, GET_NARG(__VA_ARGS__), __VA_ARGS__)
void _connect_exact(int port, const size_t arg_co9unt, ...);
#define connect_exact(port, ...) _connect_exact(port, GET_NARG(__VA_ARGS__), __VA_ARGS__)
void disable_and_disconnect_all();

#endif
