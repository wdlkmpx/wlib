/*
 * Public Domain
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdarg.h>

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


char *w_build_path (const char *separator, const char *first_element, ...)
{
    // https://docs.gtk.org/glib/func.build_path.html
    size_t len_total;
    size_t len_current;
    size_t seplen = 0;
    va_list args;
    const char * element;
    char * new_str, * pos;
    if (!first_element) {
        return NULL;
    }
    if (separator) {
        seplen = strlen(separator);
    }

    // determine total string length
    len_total = strlen (first_element) + 1;
    va_start (args, first_element);
    for (element = va_arg (args, char*);
         element != NULL;
         element = va_arg (args, char*))
    {
        if (strcmp(element,separator) == 0) {
            continue;  //ignore if element = separator
        }
        len_total += strlen (element) + 1;
        len_total += seplen;
    }
    va_end (args);

    // concatenate strings
    new_str = (char *) malloc (len_total+1);
    len_current = strlen (first_element);
    pos = new_str;
    if (len_current) {
        memcpy (new_str, first_element, len_current+1);
        pos = new_str + len_current;
        if (seplen) {
            memcpy (pos, separator, seplen);
            pos += seplen;
        }
    }
    va_start (args, first_element);
    for (element = va_arg (args, char*);
         element != NULL;
         element = va_arg (args, char*))
    {
        if (strcmp(element,separator) == 0) {
            continue;  //ignore if element = separator
        }
        len_current = strlen (element);
        if (len_current) {
            memcpy (pos, element, len_current+1);
            pos = pos + len_current;
            if (seplen) {
                memcpy (pos, separator, seplen);
                pos += seplen;
            }
        }
    }
    va_end (args);
    // separator must NOT be added at the end, remove it
    if (seplen) {
        new_str[strlen(new_str) - seplen] = '\0';
    }
    return new_str;
}


/*char *w_build_filename (const char *first_element, ...) */
