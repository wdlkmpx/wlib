/*
 * Public Domain
 */

#ifndef __W_STRFUNCS_H__
#define __W_STRFUNCS_H__

#ifdef __cplusplus
extern "C"
{
#endif

char * w_strdup (const char *str);
int w_strcmp0 (const char *s1, const char *s2);
char * w_strconcat (const char *string1, ...);

char * w_strchug (char *str);
char * w_strchomp (char *str);
#define w_str_strip_left  w_strchug
#define w_str_strip_right w_strchomp
#define w_strstrip(str) w_strchomp(w_strchug(str))
char * w_strreverse (char *str);

unsigned int w_strv_length (char ** str_array);
void w_strfreev (char **str_array);
int w_strv_contains (const char * const *strv, const char *str);


#ifdef W_GLIB_COMPAT
#define g_strdup     w_strdup
#define g_strcmp0    w_strcmp0
#define g_strconcat  w_strconcat

#define g_strchug    w_strchug
#define g_strchomp   w_strchomp
#define g_strstrip(str) w_strchomp(w_strchug(str))
#define g_strreverse w_strreverse

#define g_strv_length w_strv_length
#define g_strfreev    w_strfreev
#define g_strv_contains w_strv_contains
#endif


#ifdef __cplusplus
}
#endif

#endif /* __W_STRFUNCS_H__ */
