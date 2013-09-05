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
#include <string.h>

/********************************************************************/

gboolean su_str_empty(const char* string)
{
    if (!string)
        return TRUE;
    while (*string == ' ' || *string == '\t')
        string++;
    return strlen(string) == 0;
}

/********************************************************************/

gboolean su_str_empty_nl(const char* string)
{
    if (!string)
        return TRUE;
    while (*string == ' ' || *string == '\t' || *string == '\n')
        string++;
    return strlen(string) == 0;
}

/********************************************************************/

void su_str_get_format_for_bytes_with_suffix(guint64  bytes, const char ** format, guint64 * b1, guint64 * b2)
{
    const char * f = NULL;
    if (bytes > 1 << 30)
    {
        bytes = (bytes * 10) / (1 << 30);
        f = "%lld.%lld GiB";
    }
    else if (bytes > 1 << 20)
    {
        bytes = (bytes * 10) / (1 << 20);
        f = "%lld.%lld MiB";
    }
    else if (bytes > 1 << 10)
    {
        bytes = (bytes * 10) / (1 << 10);
        f = "%lld.%lld KiB";
    }
    else
    {
        bytes = bytes * 10;
        f = "%lld B";
    }

    if (format)
        *format = f;
    if (b1)
        *b1 = bytes / 10;
    if (b2)
        *b2 = bytes % 10;
}

gchar * su_str_format_bytes_with_suffix(guint64  bytes)
{
    const char * format = NULL;
    guint64 b1;
    guint64 b2;

    su_str_get_format_for_bytes_with_suffix(bytes, &format, &b1, &b2);

    if (format)
        return g_strdup_printf (format, b1, b2);

    return NULL;
}

/********************************************************************/
