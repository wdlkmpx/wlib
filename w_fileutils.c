/*
 * Public Domain
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#include "w_fileutils.h"

int w_mkdir_with_parents (const char * dir, int mode)
{
    if (!dir || !*dir) {
        return -1;
    }
    char * str = strdup (dir);
    char * p = NULL;
    for (p = str+1; *p; p++)
    {
        if (*p == W_DIR_SEPARATOR) {
            *p = 0;
            if (access (str, F_OK) != 0) {
                if (mkdir (str, mode) == -1) {
                    printf ("Unable to create %s\n", str);
                    free (str);
                    return -1;
                }
            }
            *p = W_DIR_SEPARATOR;
        }
    }
    if (access (str, F_OK) != 0) {
        if (mkdir (str, mode) == -1) {
            printf ("Unable to create %s\n", str);
            free (str);
            return -1;
        }
    }
    free (str);
    return 0;
}


char *w_path_get_basename (const char *filename)
{
    char *bname = NULL;
    char *p;
    if (filename) {
        p = strrchr (filename, W_DIR_SEPARATOR);
        if (p) {
            bname = strdup (p+1);
        } else if (!*filename) {
            bname = strdup ("."); // can't be empty, glib2 returns '.'
        } else {
            bname = strdup (filename);
        }
    }
    return bname;
}


char *w_path_get_dirname (const char *filename)
{
    char *dname = NULL;
    char *p;
    if (filename) {
        p = strrchr (filename, W_DIR_SEPARATOR);
        if (p) {
            dname = strdup (filename);
            p = strrchr (dname, W_DIR_SEPARATOR);
            *p = '\0';
        } else { // not a valid path, return '.' (glib2 behavior)
            dname = strdup (".");
        }
    }
    return dname;
}

