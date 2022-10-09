/*
 * Public Domain
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include "w_strfuncs.h"

char * w_strdup (const char *str)
{
    if (str) {
        return strdup(str);
    } else {
        return NULL;
    }
}


int w_strcmp0 (const char *s1, const char *s2)
{
    if (!s1 && !s2) {
        return 0;
    } else if (!s1 && s2) {
        return 1;
    } else if (s1 && !s2) {
        return -1;
    } else {
        return strcmp (s1, s2);
    }
}


char * w_strconcat (const char *string1, ...)
{
    size_t tlen, len;
    va_list args;
    const char * element;
    char * new_str, * pos;
    if (!string1) {
        return NULL;
    }
    // determine total string length
    tlen = strlen (string1) + 1;
    va_start (args, string1);
    for (element = va_arg (args, char*);
         element;
         element = va_arg (args, char*))
    {
        tlen += strlen (element) + 1;
    }
    va_end (args);
    // concatenate strings
    new_str = (char *) malloc (tlen+1);
    len = strlen (string1);
    pos = new_str;
    if (len) {
        memcpy (new_str, string1, len+1);
        pos = new_str + len;
    }
    va_start (args, string1);
    for (element = va_arg (args, char*);
         element;
         element = va_arg (args, char*))
    {
        len = strlen (element);
        if (!len) {
            continue;
        }
        memcpy (pos, element, len+1);
        pos = pos + len;
    }
    va_end (args);
    return new_str;
}


int w_str_has_prefix (const char *str, const char *prefix)
{
    if (!str || !prefix) {
        return 0; /* false */
    }
    if (strncmp (str, prefix, strlen(prefix)) == 0) {
        return 1; /* true */
    }
    return 0; /* false */
}


int w_str_has_suffix (const char *str, const char *suffix)
{
    size_t len1, len2;
    if (!str || !suffix) {
        return 0; /* false */
    }
    len1 = strlen (str);
    len2 = strlen (suffix);
    if (len2 > len1) {
        return 0; /* false */
    }
    if (strcmp (str + len1 - len2, suffix) == 0) {
        return 1; /* true */
    }
    return 0; /* false */
}


int w_strv_contains (const char * const *strv, const char *str)
{
    int i;
    for (i = 0; strv[i]; i++) {
        if (w_strcmp0 (strv[i], str) == 0) {
            return 1; /* true */
        }
    }
    return 0; /* false */
}


void w_strfreev (char **str_array)
{
    int i;
    if (!str_array) {
        return;
    }
    for (i = 0; str_array[i]; i++) {
        free (str_array[i]);
    }
    free (str_array);
    *str_array = NULL;
}


unsigned int w_strv_length (char ** str_array)
{
    int i;
    if (!str_array) {
        return 0;
    }
    for (i = 0; str_array[i]; i++);
    return i;
}


char * w_strreverse (char *str) // returns the same str
{
    int i, j;
    char temp;
    if (!str) {
        return NULL;
    }
    i = strlen(str) - 1;
    j = 0;
    while (i > j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i--;
        j++;
    }
    return str;
}


char * w_strchug (char *str) // remove leading spaces
{
    int i = 0;
    int j = 0;
    if (!str) {
        return NULL;
    }
    // find the first char that is not a space
    while (str[i] && isspace(str[i])) {
        i++;
    }
    if (i > j) {
        // move chars to the left
        while (str[i]) {
            str[j] = str[i];
            i++;
            j++;
        }
        str[j] = '\0';
    }
    return str;
}


char * w_strchomp (char *str) // remove trailing spaces
{
    int i = 0;
    if (!str) {
        return NULL;
    }
    // find end of string
    while (str[i]) i++;
    i--;
    // convert spaces to 0s until the first non-space char
    while (i >= 0 && isspace(str[i])) {
        str[i] = '\0';
        i--;
    }
    return str;
}
