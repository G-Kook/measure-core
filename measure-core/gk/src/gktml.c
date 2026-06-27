#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
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

struct gktml {
    gkstr tag;
    size_t idx;
    gkstr name;
    int padid;
    int pinid;
};

static int padid_to_pinid_44pin(int padid) { return (padid % 2) ? (26 + (int)(padid / 2)) : (24 - (int)(padid / 2)); }
typedef int (*MapperType)(int);
static MapperType* _padid_to_pinid() { static MapperType dat = NULL; return &dat; }
static MapperType padid_to_pinid() { return *_padid_to_pinid(); }
void gktml_init_44pin() { *_padid_to_pinid() = &padid_to_pinid_44pin; }

#define GKTML_CAPACITY_TERMINALS 200

struct gktmls
{
    size_t size;
    gktml ts[GKTML_CAPACITY_TERMINALS];
};
typedef struct gktmls gktmls;

static gktmls* _terminals()
{
   static gktmls dat;
   return &dat;
}

static bool is_index_occupied(const char* tag, size_t idx)
{
   const gktml* t;
   for (t = gktml_begin(); t < gktml_end(); t = gktml_next(t))
   {
      if (gkstr_eq((const char*)&t->tag, tag) && (t->idx == idx)) return true;
   }
   return false;
}

void gktml_register(const char* tag, size_t idx, const char* padname, int padid)
{
   assertf(padid_to_pinid(), "<%s> Pin type must be specified.\n", __FUNCTION__);
   assertf(_terminals()->size < GKTML_CAPACITY_TERMINALS, "<%s> GKTML_CAPACITY_TERMINALS is too small to register more terminals.\n", __FUNCTION__);
   gktml* new_t = _terminals()->ts + _terminals()->size;
   new_t->tag = gkstr_make(tag);
   new_t->idx = idx;
   new_t->name = gkstr_make(padname);
   new_t->padid = padid;
   new_t->pinid = padid_to_pinid()(padid);
   _terminals()->size += 1;
}

void gktml_reset() { _terminals()->size = 0; }
const gktml* gktml_begin() { return _terminals()->ts; }
const gktml* gktml_end() { return _terminals()->ts + _terminals()->size; }
const gktml* gktml_next(const gktml* cur) { return ((cur >= gktml_begin()) && (cur < gktml_end())) ? (cur + 1) : NULL; }
gkstr gktml_name(const gktml* t) { return t->name; }
const gktml* gktml_get(const char* tag, size_t idx)
{
   const gktml* t;
   for (t = gktml_begin(); t != gktml_end(); t = gktml_next(t))
   {
      if (gkstr_eq((const char*)&t->tag, tag) && (t->idx == idx)) return t;
   }
   assertf(false, "<%s> Cannot find any terminal %s[%d].\n", __FUNCTION__, tag, idx);
   abort();
}

void gktml_print_info()
{
   recordf("## PIN INFO\n");
   recordf(" %-10s %-7s %-7s %-7s %-15s\n", "TYPE", "INDEX", "PAD_ID", "PIN_ID", "NAME");
   const gktml* t;
   for (t = gktml_begin(); t < gktml_end(); t = gktml_next(t))
   {
      recordf(" %-10s %-7zu %-7d %-7d %-15s\n", &t->tag, t->idx, t->padid, t->pinid, &t->name);
   }
}

void _connect_tml(int port, const size_t arg_count, ...)
{
   va_list arg_ptr;
   va_start(arg_ptr, arg_count);
   size_t adx; for (adx = 0; adx < arg_count; adx++)
   {
      const gktml* t = va_arg(arg_ptr, const gktml*);
      connect_pin(port, t->pinid);
   }
   va_end(arg_ptr);
}

void _connect_any(int port, const size_t arg_count, ...)
{
   va_list arg_ptr;
   va_start(arg_ptr, arg_count);
   size_t adx;
   for (adx = 0; adx < arg_count; adx++)
   {
      const char* name = va_arg(arg_ptr, const char*);
      const gktml* t;
      for (t = gktml_begin(); t != gktml_end(); t = gktml_next(t))
      {
         const gkstr t_name = gktml_name(t);
         if (gkstr_issub(name, (const char*)&t_name)) connect_tml(port, t);
      }
   }
   va_end(arg_ptr);
}

void _connect_exact(int port, const size_t arg_count, ...)
{
   va_list arg_ptr;
   va_start(arg_ptr, arg_count);
   size_t adx;
   for (adx = 0; adx < arg_count; adx++)
   {
      const char* name = va_arg(arg_ptr, const char*);
      const gktml* t;
      for (t = gktml_begin(); t != gktml_end(); t = gktml_next(t))
      {
         const gkstr t_name = gktml_name(t);
         if (gkstr_eq(name, (const char*)&t_name)) connect_tml(port, t);
      }
   }
   va_end(arg_ptr);
}

void disable_and_disconnect_all()
{
   disable_port_all();
   sync_th(0);
   disconnect_all();
}
