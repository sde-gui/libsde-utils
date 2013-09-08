/**
 * Copyright (c) 2013 Vadim Ushakov
 * Copyright (c) 2006 LxDE Developers, see the file AUTHORS for details.
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
