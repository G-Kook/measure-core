#ifndef GKIO
#define GKIO

#include <stdbool.h>
#include "gk_core.h"
#include "gkstr.h"

/** FILE SYSTEM  */
gkstr gkio_fdir_from_fpath(const char* fpath);
gkstr gkio_fname_from_fpath(const char* fpath);
bool gkio_dir_exists(const char* fdir);
bool gkio_provide_directory(const char* fdir);
bool gkio_file_exists(const char* fpath);
bool gkio_provide_file(const char* fpath);
gkstr gkio_current_time_yyyymmdd();
gkstr gkio_current_time_yyyymmdd_hhmmss();

/** PATH CONSTANTS */
gkstr gkio_temppath();
gkstr gkio_cachepath();
gkstr gkio_logpath();

/** PATH GLOBALS */
gkstr gkio_savedir();
void gkio_set_savedir(const char* dir);
gkstr gkio_outdir();
void gkio_set_outdir(const char* dir);
gkstr gkio_outpath();
void gkio_set_outpath(const char* path);

/** OUTPUT FUNCTIONS */
void recordf(const char* format, ...);
void pause_recording();
void resume_recording();
void pause_displaying();
void resume_displaying();
enum ELogChannel
{
    ELogChannel_NONE,
    ELogChannel_FRAMEWORK,
    ELogChannel_MAX
};
void logprintf(enum ELogChannel type, const char* format, ...);
void stop_logging(enum ELogChannel type);
void start_logging(enum ELogChannel type);

#endif
