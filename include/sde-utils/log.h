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

#ifndef __SDE_UTILS__LOG_H
#define __SDE_UTILS__LOG_H


#include <stdio.h>
#include <stdarg.h>

/********************************************************************/

typedef enum _SU_LOG_LEVEL {
    SU_LOG_NONE,
    SU_LOG_ERROR,
    SU_LOG_WARNING,
    SU_LOG_INFO,
    SU_LOG_DEBUG,
    SU_LOG_DEBUG_SPAM,
    SU_LOG_DEBUG_2 = SU_LOG_DEBUG_SPAM,
    SU_LOG_ALL
} SU_LOG_LEVEL;

/********************************************************************/

extern int su_log_level;

void su_log_message_va(SU_LOG_LEVEL level, const char * format, va_list ap);
void su_log_message   (SU_LOG_LEVEL level, const char * format, ...);
void su_log_error     (const char * format, ...);
void su_log_warning   (const char * format, ...);
void su_log_info      (const char * format, ...);
void su_log_debug     (const char * format, ...);
void su_log_debug2    (const char * format, ...);

#define SU_LOG_DEBUG(fmt, args...)  do { if (su_log_level >= SU_LOG_DEBUG  ) su_log_debug(fmt, ## args); } while(0)
#define SU_LOG_DEBUG2(fmt, args...) do { if (su_log_level >= SU_LOG_DEBUG_2) su_log_debug2(fmt, ## args); } while(0)

/********************************************************************/

void su_print_error_message(const char *string, ...);

#endif
