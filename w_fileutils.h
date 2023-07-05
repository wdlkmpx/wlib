/*
 * Public Domain
 */

#ifndef __W_FILEUTILS_H__
#define __W_FILEUTILS_H__

#ifdef __cplusplus
extern "C"
{
#endif


char *w_path_get_basename (const char *filename);
char *w_path_get_dirname (const char *filename);
int w_mkdir_with_parents (const char * dir, int mode);
char *w_build_path (const char *separator, const char *first_element, ...);
//char *w_build_filename (const char *first_element, ...);


#ifdef _WIN32
#define W_DIR_SEPARATOR '\\'
#define W_DIR_SEPARATOR_S "\\"
#define W_SEARCHPATH_SEPARATOR ';'
#define W_SEARCHPATH_SEPARATOR_S ";"
#else
#define W_DIR_SEPARATOR '/'
#define W_DIR_SEPARATOR_S "/"
#define W_SEARCHPATH_SEPARATOR ':'
#define W_SEARCHPATH_SEPARATOR_S ":"
#endif

#define w_build_filename(string1, ...) w_build_path(W_DIR_SEPARATOR_S, string1, __VA_ARGS__)

#ifdef W_GLIB_COMPAT
#define g_mkdir_with_parents w_mkdir_with_parents
#define g_path_get_basename w_path_get_basename
#define g_path_get_dirname  w_path_get_dirname
#define g_build_path     w_build_path
#define g_build_filename(string1, ...) w_build_path(W_DIR_SEPARATOR_S, string1, __VA_ARGS__)

#define G_DIR_SEPARATOR W_DIR_SEPARATOR
#define G_DIR_SEPARATOR_S W_DIR_SEPARATOR_S
#define G_SEARCHPATH_SEPARATOR W_SEARCHPATH_SEPARATOR
#define G_SEARCHPATH_SEPARATOR_S W_SEARCHPATH_SEPARATOR_S
#endif


#ifdef __cplusplus
}
#endif

#endif /* __W_FILEUTILS_H__ */
