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

#ifndef __SDE_UTILS__STRINGS_H
#define __SDE_UTILS__STRINGS_H

#include <glib.h>

typedef enum _SU_READ_LIST_OPTIONS
{
    SU_READ_LIST_IGNORE_COMMENTS = 1 << 0,
    SU_READ_LIST_IGNORE_EMPTY_LINES = 1 << 1,
    SU_READ_LIST_IGNORE_WHITESPACES = 1 << 2
} SU_READ_LIST_OPTIONS;

gboolean su_str_empty(const char * string);

void     su_str_get_format_for_bytes_with_suffix(guint64  bytes, const char ** format, guint64 * b1, guint64 * b2);
gchar *  su_str_format_bytes_with_suffix(guint64  bytes);

gchar ** su_read_lines_from_file(gchar * path, SU_READ_LIST_OPTIONS options);

#endif
