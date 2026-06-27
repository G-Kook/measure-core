#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS 1
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <float.h>
#include <limits.h>
#ifdef __linux__
#include <unistd.h>
#elif _WIN32
#include <windows.h>
#endif
#include "gk_core.h"

void assertf(bool statement, const char* format, ...)
{
    if (!statement)
    {
        va_list args;
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
    }
    assert(statement);
}

static int _dround(double x) { return (x < 0.) ? (int)(x - 0.5) : (int)(x + 0.5); }
double dround(double v, size_t d)
{
    int e = 1;
    size_t i;
    for (i = 0; i < d; i++) e *= 10;
    return ((double)_dround(v * e)) / e;
}

#ifdef __linux__
void sleep_sec(double sec)
{
    const double _sec = MAX(sec, 0);
    sleep(_sec);
} 
#elif _WIN32
void sleep_sec(double sec)
{
    const double _sec = MAX(sec, 0);
    Sleep(_sec * 1000);
}
#else
void sleep_sec(double sec) {}
#endif