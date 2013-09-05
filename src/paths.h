/**
 * Copyright (c) 2013 Vadim Ushakov
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
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
gchar *  su_path_resolve_config_va  (SU_PATH_CONFIG_TYPE type, const char * domain,
                                     const char * profile, va_list ap);
gchar *  su_path_resolve_config     (SU_PATH_CONFIG_TYPE type, const char * domain,
                                     const char * profile, ...);

FILE  *  su_path_fopen_resource_va(const char * mode, const char * agent_id, va_list ap);
FILE  *  su_path_fopen_resource   (const char * mode, const char * agent_id, ...);
FILE  *  su_path_fopen_config_va  (const char * mode, SU_PATH_CONFIG_TYPE type,
                                   const char * domain, const char * profile, va_list ap);
FILE  *  su_path_fopen_config     (const char * mode, SU_PATH_CONFIG_TYPE type,
                                   const char * domain, const char * profile, ...);

gchar *  su_path_read_resource_va(const char * agent_id, va_list ap);
gchar *  su_path_read_resource   (const char * agent_id, ...);
gchar *  su_path_read_config_va  (SU_PATH_CONFIG_TYPE type, const char * domain,
                                     const char * profile, va_list ap);
gchar *  su_path_read_config     (SU_PATH_CONFIG_TYPE type, const char * domain,
                                     const char * profile, ...);

/********************************************************************/

gchar * su_build_filename_va(va_list ap);

#endif
