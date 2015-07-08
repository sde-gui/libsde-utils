/**
 * Copyright (c) 2013-2015 Vadim Ushakov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef __SDE_UTILS__PATHS_H
#define __SDE_UTILS__PATHS_H

#include <stdio.h>
#include <glib.h>

/********************************************************************/

typedef enum _SU_PATH_CONFIG_TYPE
{
    SU_PATH_CONFIG_SYSTEM,
    SU_PATH_CONFIG_USER,
    SU_PATH_CONFIG_USER_W
} SU_PATH_CONFIG_TYPE;

/********************************************************************/

gchar *  su_path_expand_tilda(const char * path);

/********************************************************************/

gchar *  su_path_resolve_agent_id_by_path   (const char * path, const char * default_id);
gchar *  su_path_resolve_agent_id_by_pointer(void * p, const char * default_id);

/********************************************************************/

void     su_path_register_default_agent_prefix(const char * agent_id, const char * prefix);

gchar *  su_path_resolve_resource_va(const char * agent_id, va_list ap);
gchar *  su_path_resolve_resource   (const char * agent_id, ...);
gchar *  su_path_resolve_config_va  (const char * agent_id, SU_PATH_CONFIG_TYPE type,
                                     const char * domain, const char * profile, va_list ap);
gchar *  su_path_resolve_config     (const char * agent_id, SU_PATH_CONFIG_TYPE type,
                                     const char * domain, const char * profile, ...);

FILE  *  su_path_fopen_resource_va(const char * mode, const char * agent_id, va_list ap);
FILE  *  su_path_fopen_resource   (const char * mode, const char * agent_id, ...);
FILE  *  su_path_fopen_config_va  (const char * mode, const char * agent_id, SU_PATH_CONFIG_TYPE type,
                                   const char * domain, const char * profile, va_list ap);
FILE  *  su_path_fopen_config     (const char * mode, const char * agent_id, SU_PATH_CONFIG_TYPE type,
                                   const char * domain, const char * profile, ...);

gchar *  su_path_read_resource_va(const char * agent_id, va_list ap);
gchar *  su_path_read_resource   (const char * agent_id, ...);
gchar *  su_path_read_config_va  (const char * agent_id, SU_PATH_CONFIG_TYPE type,
                                  const char * domain, const char * profile, va_list ap);
gchar *  su_path_read_config     (const char * agent_id, SU_PATH_CONFIG_TYPE type,
                                  const char * domain, const char * profile, ...);

/********************************************************************/

gchar * su_build_filename_va(va_list ap);

#endif
