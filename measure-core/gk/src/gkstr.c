#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS 1
#endif
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include "gkstr.h"

size_t gkstr_capacity() { return GKSTR_SIZE; }

gkstr gkstr_make(const char* s)
{
   const size_t l = gkstr_len(s);
   assertf(l < GKSTR_SIZE, "<%s> Length of s is %zu. Please increase GKSTR_SIZE from %zu.\ns: %s", __FUNCTION__, l, GKSTR_SIZE, s);
   gkstr gs;
   strcpy((char*)&gs, s);
   return gs;
}

gkstr gkstr_concat(const char* s, size_t arg_count, ...)
{
   gkstr gs = gkstr_make(s);

   va_list arg_ptr;
   va_start(arg_ptr, arg_count);
   size_t adx;
   for (adx = 0; adx < arg_count; adx++)
   {
      const char* cur_s = va_arg(arg_ptr, const char*);
      const size_t l = (gkstr_len((const char*)&gs) + gkstr_len(cur_s));
      assertf(l < GKSTR_SIZE, "<%s> Please increase GKSTR_SIZE from %zu.", __FUNCTION__, GKSTR_SIZE);
      strcat((char*)&gs, cur_s);
   }
   va_end(arg_ptr);
   return gs;
}

gkstr gkstr_printf(const char* format, ...)
{
   gkstr gs;
   va_list args;
   va_start(args, format);
   vsprintf((char*)&gs, format, args);
   va_end(args);
   return gs;
}

size_t gkstr_len(const char* s) { return strlen(s); }
bool gkstr_eq(const char* s, const char* other) { return strcmp(s, other) == 0; }
bool _gkstr_isin(const char* s, size_t arg_count, ...)
{
   bool retval = false;
   va_list arg_ptr;
   va_start(arg_ptr, arg_count);
   size_t idx;
   for (idx = 0; idx < arg_count; idx++)
   {
      if (gkstr_eq(s, va_arg(arg_ptr, const char*)))
      {
         retval = true;
         break;
      }
   }
   va_end(arg_ptr);
   return retval;
}

bool gkstr_issub(const char* c, const char* p) { return strstr(p, c) != NULL; }

gkstr gkstr_setat(const char* s, size_t idx, char c)
{
   gkstr ret = gkstr_make(s);
   ((char*)&ret)[idx] = c;
   return ret;
}

gkstr gkstr_fill(char c, size_t n)
{
   assertf(n < GKSTR_SIZE, "<%s> n is larger than GKSTER_SIZE.\n", __FUNCTION__);
   gkstr ret;
   memset(&ret, (unsigned char) c, n);
   *(char*)(&ret + n) = '\n';
   return ret;
}

gkstr gkstr_strip(const char* s)
{
   while (isspace((unsigned char)*s)) s++;
   gkstr gs = gkstr_make(s);
   strcpy((char*)&gs, s);
   char* cp = ((char*)&gs) + gkstr_len((const char*)&gs) - 1;
   while ((cp >= (char*)&gs) && isspace(*cp))
   {
      *cp = '\0';
      cp--;
   }
   return gs;
}

gkstr gkstr_toupper(const char* src)
{
   gkstr gs = gkstr_make(src);
   char* cp;
   for (cp = (char*)&gs; *cp != '\0'; cp++) *cp = toupper(*cp);
   return gs;
}

gkstr gkstr_tolower(const char* src)
{
   gkstr gs = gkstr_make(src);
   char* cp;
   for (cp = (char*)&gs; *cp != '\0'; cp++) *cp = tolower(*cp);
   return gs;
}

char gkstr_getat(const char* s, size_t idx) { return s[idx]; }
bool gkstr_startswith(const char* s, const char* other) { return strstr(s, other) == s; }
bool gkstr_endswith(const char* s, const char* other) { return strstr(s, other) == s + strlen(s) - strlen(other); }

gkstr gkmask_set(const char* s, size_t idx) { return gkstr_setat(s, idx, __MARK); }
gkstr _gkmask_reset(const char* s, size_t idx) { return gkstr_setat(s, idx, __UNMARK); }
size_t gkmask_begin(const char* s)
{
   size_t idx;
   for (idx = 0; idx < strlen(s); idx++)
      if (s[idx] == __MARK) return idx;
   return gkmask_end();
}
size_t gkmask_end() { return SIZE_MAX; }
size_t gkmask_next(const char* s, size_t cur)
{
   size_t idx;
   for (idx = cur + 1; idx < strlen(s); idx++)
      if (s[idx] == __MARK) return idx;
   return gkmask_end();
}
size_t gkmask_rbegin(const char* s) { return gkmask_rnext(s, strlen(s)); }
size_t gkmask_rend() { return SIZE_MAX; }
size_t gkmask_rnext(const char* s, size_t cur)
{
   while (cur > 0)
   {
      cur--;
      if (s[cur] == __MARK) return cur;
   }
   return gkmask_rend();
}

gkstr _gkmask_union(const char* s1, size_t arg_count, ...)
{
   gkstr retval = gkstr_make(s1);
   va_list arg_ptr;
   va_start(arg_ptr, arg_count);
   size_t adx;
   for (adx = 0; adx < arg_count; adx++)
   {
      char* left = (char*)&retval;
      const char* right = va_arg(arg_ptr, const char*);
      const size_t len = MIN(MAX(strlen(left), strlen(right)), GKSTR_SIZE - 1);
      size_t idx;
      for (idx = 0; idx < len; idx++)
         left[idx] = (((idx < strlen(left)) && (left[idx] == __MARK)) || ((idx < strlen(right)) && (right[idx] == __MARK))) ? __MARK : __UNMARK;
      left[len] = '\0';
   }
   va_end(arg_ptr);
   return retval;
}

gkstr _gkmask_intersect(const char* s1, size_t arg_count, ...)
{
   gkstr retval = gkstr_make(s1);
   va_list arg_ptr;
   va_start(arg_ptr, arg_count);
   size_t adx;
   for (adx = 0; adx < arg_count; adx++)
   {
      char* left = (char*)&retval;
      const char* right = va_arg(arg_ptr, const char*);
      const size_t len = MIN(MAX(strlen(left), strlen(right)), GKSTR_SIZE - 1);
      size_t idx;
      for (idx = 0; idx < len; idx++)
         left[idx] = (((idx < strlen(left)) && (left[idx] == __MARK)) && ((idx < strlen(right)) && (right[idx] == __MARK))) ? __MARK : __UNMARK;
      left[len] = '\0';
   }
   va_end(arg_ptr);
   return retval;
}