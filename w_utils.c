/*
 * Public Domain
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "w_utils.h"

void w_free (void *mem) // this should go w_mem.h
{
    if (mem) free (mem);
}

// if app is found in $PATH, return a string that must be freed
char * w_find_program_in_path (const char * program)
{
    char *token = NULL;
    char *path = getenv ("PATH");
    char *dup_path = NULL;
    char *rest = NULL;
    char *app_full_path = NULL;
    int found = 0;
    if (!path) {
        path = "/bin:/usr/bin:.";
    } else if (!*path || !program || !*program) {
        return NULL; // do not accept emtpy PATH or app
    }
    app_full_path = malloc (256);
    dup_path = strdup (path);
    rest = dup_path;
    do
    {
        token = strtok_r (rest, ":", &rest); // strtok_r = POSIX.2001
        if (token) {
            snprintf (app_full_path, 255, "%s/%s", token, program);
            //puts (app_full_path);
            if (access (app_full_path, F_OK) == 0) {
                found = 1;
                break;
            }    
        }
    } while (token);
    free (dup_path);
    if (!found) {
        free (app_full_path);
        return NULL;
    } else {
        return app_full_path;
    }
}


const char *w_get_user_config_dir (void)
{
    static char * config_dir = NULL;
    static char config_dir_default[256];
    if (config_dir) {
        return config_dir;
    }
    config_dir = getenv ("XDG_CONFIG_HOME");
#if __MINGW32__
    if (!config_dir) {
        config_dir = getenv ("LOCALAPPDATA"); /* XP */
    }
    if (!config_dir) {
        config_dir = getenv ("APPDATA");
    }
#endif
    if (!config_dir) {
        snprintf (config_dir_default, 255, "%s/.config", w_get_home_dir());
        config_dir = config_dir_default;
    }
    return config_dir;
}


const char *w_get_user_data_dir (void)
{
    char * data_dir = NULL;
    data_dir = getenv ("XDG_DATA_HOME");
#if __MINGW32__
    if (!data_dir) {
        data_dir = getenv ("LOCALAPPDATA"); /* XP */
    }
    if (!data_dir) {
        data_dir = getenv ("APPDATA");
    }
#endif
    return data_dir;
}


const char *w_get_user_cache_dir (void)
{
    return getenv ("XDG_CACHE_HOME");
}


const char *w_get_home_dir (void)
{
    return getenv ("HOME");
}
