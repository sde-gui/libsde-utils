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
    while (*string == ' ' || *string == '\t' || *string == '\n')
        string++;
    return *string == 0;
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

gchar ** su_read_lines_from_file(gchar * path, SU_READ_LIST_OPTIONS options)
{
    if (!path)
        return NULL;

    char * data;
    g_file_get_contents(path, &data, NULL, NULL);
    if (!data)
        return NULL;

    /* Split into lines and check each line. */

    gchar ** lines = g_strsplit(data, "\n", 0);
    g_free(data);

    GSList * string_list = NULL;
    int line_count = 0;

    gchar ** l;
    if (lines) for (l = lines; *l; l++)
    {
        gchar * line = *l;
        if (
            ((options & SU_READ_LIST_IGNORE_EMPTY_LINES) && line[0] == 0) ||
            ((options & SU_READ_LIST_IGNORE_WHITESPACES) && su_str_empty(line)) ||
            ((options & SU_READ_LIST_IGNORE_COMMENTS) && line[0] == '#')
           )
        {
            g_free(line);
        }
        else
        {
            string_list = g_slist_prepend(string_list, line);
            line_count++;
        }
    }

    g_free(lines);

    if (!line_count)
        return NULL;

    gchar ** result_vector = g_new(gchar*, line_count + 1);

    result_vector[line_count--] = NULL;
    GSList * slist;
    for (slist = string_list; slist; slist = slist->next)
    {
        g_assert(line_count >= 0);
        result_vector[line_count--] = slist->data;
    }

    g_slist_free(string_list);

    return result_vector;
}
