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

#include "strings.h"
#include <pwd.h>

/********************************************************************/

gchar * su_path_expand_tilda(const char * path)
{
    if (path[0] != '~')
        return g_strdup(path);

    const char * homedir = NULL;

    size_t name_end = 1;
    while (path[name_end] != '/' && path[name_end] != 0)
        name_end++;

    if (name_end == 1)
    {
        homedir = g_getenv("HOME");
        if (!homedir)
            homedir = g_get_home_dir();
    }
    if (name_end > 1)
    {
        gchar * name = g_strndup(path + 1, name_end - 1);
        struct passwd * pw = getpwnam(name);
        if (pw && !su_str_empty(pw->pw_dir))
            homedir = pw->pw_dir;
    }

    if (!homedir)
        homedir = g_get_tmp_dir();

    if (!homedir)
        homedir = "/tmp";

    return g_strdup_printf("%s%s", homedir, path + name_end);
}

/********************************************************************/

