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

#ifndef __SDE_UTILS__STRINGS_H
#define __SDE_UTILS__STRINGS_H

#include <glib.h>

gboolean su_str_empty(const char * string);
gboolean su_str_empty_nl(const char* string);

void     su_str_get_format_for_bytes_with_suffix(guint64  bytes, const char ** format, guint64 * b1, guint64 * b2);
gchar *  su_str_format_bytes_with_suffix(guint64  bytes);

#endif
