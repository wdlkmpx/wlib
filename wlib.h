/*
 * Public Domain
 */

#ifndef __W_LIB_H__
#define __W_LIB_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define wpointer void*

#ifdef W_GLIB_COMPAT
#define gpointer void*
#endif

#include "w_fileutils.h"
#include "w_inet.h"
#include "w_strfuncs.h"
#include "w_utils.h"

#ifdef __cplusplus
}
#endif

#endif /* __W_LIB_H__ */
