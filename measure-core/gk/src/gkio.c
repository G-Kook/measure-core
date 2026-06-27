#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS 1
#endif
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "time.h"
#ifdef __linux__
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#elif _WIN32
#include <Windows.h>
#endif
#include "gkio.h"

static gkstr dir_strip(const char* dir)
{
    gkstr out_dir = gkstr_strip(dir);
    char* cp = ((char*)&out_dir) + gkstr_len((const char*)&out_dir) - 1;
    while ((*cp == '/') && (cp >= (char*)&out_dir))
    {
        *cp = '\0';
        cp--;
    }
    return out_dir;
}

gkstr gkio_fdir_from_fpath(const char* fpath)
{
    const char* cp = fpath + strlen(fpath) + 1;
    while ((cp >= fpath) && (*cp != '/')) cp--;
    gkstr gs = gkstr_make(fpath);
    ((char*)&gs)[cp - fpath] = '\0';
    return dir_strip((const char*)&gs);
}

gkstr gkio_fname_from_fpath(const char* fpath)
{
    const char* cp = fpath + strlen(fpath) - 1;
    while ((cp >= fpath) && (*cp != '/')) cp --;
    return gkstr_make(cp + 1);
}

bool gkio_dir_exists(const char* fdir)
{
#ifdef __linux__
    DIR* dir = opendir(fdir);
    if (dir)
    {
        closedir(dir);
        return true;
    }
    return false;
#elif _WIN32
    DWORD attributes = GetFileAttributesA(fdir);
    return (attributes != INVALID_FILE_ATTRIBUTES) && (attributes & FILE_ATTRIBUTE_DIRECTORY);
#endif
}

bool gkio_provide_directory(const char* fdir)
{
    if (!gkio_dir_exists(fdir))
    {
#ifdef __linux__
        struct stat st = { .st_dev = 0 };
        if (stat(fdir, &st) == -1) mkdir(fdir, 0700);
#elif _WIN32
        CreateDirectoryA(fdir, NULL);
#endif
    }
    return gkio_dir_exists(fdir);
}

bool gkio_file_exists(const char* fpath)
{
    FILE* file;
    if ((file = fopen(fpath, "rb")))
    {
        fclose(file);
        return true;
    }
    return false;
}

bool gkio_provide_file(const char* fpath)
{
    if (!gkio_file_exists(fpath))
    {
        const gkstr fdir = gkio_fdir_from_fpath(fpath);
        if (gkio_provide_directory((const char*)&fdir))
        {
            FILE* file = fopen(fpath, "wb");
            if (file)
            {
                fclose(file);
                return true;
            }
            return false;
        }
        return false;
    }
    return true;
}

gkstr gkio_current_time_yyyymmdd()
{
    time_t cur_time;
    time(&cur_time);
    struct tm* time_info = localtime(&cur_time);
    gkstr gs;
    strftime((char*)&gs, gkstr_capacity(), "%Y%m%d", time_info);
    return gs;
}

gkstr gkio_current_time_yyyymmdd_hhmmss()
{
    time_t cur_time;
    time(&cur_time);
    struct tm* time_info = localtime(&cur_time);
    gkstr gs;
    strftime((char*)&gs, gkstr_capacity(), "%Y%m%d_%H%M%S", time_info);
    return gs;
}

gkstr gkio_temppath() { return gkstr_make("D:/src-c/gk-specs/output/tmp_data_c"); }
gkstr gkio_cachepath() { return gkstr_make("D:/src-c/gk-specs/output/gk_cache"); }
gkstr gkio_logpath() { return gkstr_make("D:/src-c/gk-specs/output/gk_log"); }

static gkstr* _savedir()
{
    static bool init;
    static gkstr dat;
    if (!init)
    {
        init = true;
        dat = gkstr_make("D:/src-c/gk-specs/output");
    }
    return &dat;
}
gkstr gkio_savedir() { return *_savedir(); }
void gkio_set_savedir(const char* dir) { *_savedir() = dir_strip(dir); }

static gkstr* _outdir()
{
    static gkstr dat;
    return &dat;
}
gkstr gkio_outdir() { return *_outdir(); }
void gkio_set_outdir(const char* dir) { *_outdir() = dir_strip(dir); }

static gkstr* _outpath()
{
    static gkstr dat;
    return &dat;
}
gkstr gkio_outpath() { return *_outpath(); }
void gkio_set_outpath(const char* s) { *_outpath() = dir_strip(s); }

static size_t* _recording_stop_count() { static size_t dat = 0; return &dat; }
static bool is_recording() { return *_recording_stop_count() == 0; }
void pause_recording() { *_recording_stop_count() = *_recording_stop_count() + 1; }
void resume_recording()
{
    assertf(*_recording_stop_count() > 0, "<%s> called more than pause_recording.\n", __FUNCTION__);
    *_recording_stop_count() = *_recording_stop_count() - 1;
}

static size_t* _displaying_stop_count() { static size_t dat = 0; return &dat; }
static bool is_displaying() { return *_displaying_stop_count() == 0; }
void pause_displaying() { *_displaying_stop_count() = *_displaying_stop_count() + 1; }
void resume_displaying()
{
    assertf(*_displaying_stop_count() > 0, "<%s> called more than pause_displaying.\n", __FUNCTION__);
    *_displaying_stop_count() = *_displaying_stop_count() - 1;
}

void recordf(const char* format, ...)
{
    va_list args;
    if (is_displaying())
    {
        va_start(args, format);
        vfprintf(stdout, format, args);
        va_end(args);
    }
    if (is_recording())
    {
        const gkstr outpath = gkio_outpath();
        FILE* file = fopen((const char*)&outpath, "ab");
        if (file)
        {
            va_start(args, format);
            vfprintf(file, format, args);
            va_end(args);
            fclose(file);
        }
    }
}
static bool* _should_log(enum ELogChannel type) { static bool dat[ELogChannel_MAX]; return &dat[type]; }
void start_logging(enum ELogChannel type) { *_should_log(type) = true; }
void stop_logging(enum ELogChannel type) { *_should_log(type) = false; }
void logprintf(enum ELogChannel type, const char* format, ...)
{
    const gkstr logpath = gkio_logpath();
    if (*_should_log(type) && gkio_provide_file((const char*)&logpath))
    {
        FILE* file = fopen((const char*)&logpath, "ab");
        if (file)
        {
            va_list args;
            va_start(args, format);
            vfprintf(file, format, args);
            va_end(args);
            fclose(file);
        }
    }
}
