/*
 * Public Domain
 */

#ifndef __W_UTILS_H__
#define __W_UTILS_H__

#ifdef __cplusplus
extern "C"
{
#endif

void w_free (void *mem); // this should go w_mem.h
char * w_find_program_in_path (const char * program);

const char *w_get_user_config_dir (void);
const char *w_get_user_data_dir   (void);
const char *w_get_user_cache_dir  (void);
const char *w_get_home_dir (void);

#ifdef W_GLIB_COMPAT
#define g_free w_free
#define g_find_program_in_path w_find_program_in_path
#define g_get_user_config_dir w_get_user_config_dir
#define g_get_user_data_dir   w_get_user_data_dir
#define g_get_user_cache_dir  w_get_user_cache_dir 
#define g_get_home_dir        w_get_home_dir
#endif

#ifdef __cplusplus
}
#endif

#endif /* __W_UTILS_H__ */
