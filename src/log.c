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

#include <sde-utils/log.h>

#include <glib.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>

int su_log_level = -1;

typedef enum {
    RED,
    GREEN,
    ORANGE,
    BLUE,
    TEAL,
    LIGHT_YELLOW,
    LIGHT_BLUE,
    LIGHT_PURPLE,
    NORMAL
} Color;

static int is_colors_supported(void)
{
    static int supported = -1;

    if (supported < 0)
    {
        const char * s = g_getenv("TERM");
        supported = (
            g_str_has_prefix(s, "xterm")  ||
            g_str_has_prefix(s, "rxvt")   ||
            g_str_has_prefix(s, "Eterm")  ||
            g_str_has_prefix(s, "aterm")  ||
            g_str_has_prefix(s, "kterm")  ||
            g_str_has_prefix(s, "gnome")  ||
            g_str_has_prefix(s, "screen")
        ) ? 1 : 0;
    }

    return supported;
}

static const char * color(Color c, FILE * stream)
{
    if (!is_colors_supported() || !stream || !isatty(fileno(stream)))
        return "";

    switch (c)
    {
        case RED:          return "\033[0;31m";
        case GREEN:        return "\033[0;32m";
        case ORANGE:       return "\033[0;33m";
        case BLUE:         return "\033[0;34m";
        case TEAL:         return "\033[0;36m";
        case LIGHT_YELLOW: return "\033[1;33m";
        case LIGHT_BLUE:   return "\033[1;34m";
        case LIGHT_PURPLE: return "\033[1;35m";

        case NORMAL:
        default:
                           return "\033[0m";
    }
}


void su_log_message_va(SU_LOG_LEVEL level, const char * format, va_list ap)
{
    if (su_log_level < 0)
    {
        const char * s = g_getenv("SDE_UTILS_LOG_LEVEL");
        if (s)
            su_log_level = atoi(s);
        else
            su_log_level = SU_LOG_WARNING;
    }

    if (((int)level) > su_log_level)
        return;

    if (!format || !*format)
        return;

    FILE * stream = stderr;
    const char * modifier = "";
    Color modifier_color;
    switch (level)
    {
        case SU_LOG_ERROR:
        {
            modifier = "ERR"; modifier_color = RED;
            break;
        }
        case SU_LOG_WARNING:
        {
            modifier = "WRN"; modifier_color = ORANGE;
            break;
        }
        case SU_LOG_INFO:
        {
            modifier = "INF"; modifier_color = GREEN;
            break;
        }
        case SU_LOG_DEBUG:
        {
            modifier = "DBG"; modifier_color = TEAL;
            break;
        }
        case SU_LOG_DEBUG_SPAM:
        {
            modifier = "DBG"; modifier_color = BLUE;
            break;
        }
        default:
        {
            modifier = "XXX"; modifier_color = RED;
            break;
        }
    }

    time_t current_time = time(NULL);
    struct tm * local_time = localtime(&current_time);
    char time_buffer[256];
    time_buffer[0] = 0;
    if (local_time != NULL)
        strftime(time_buffer, 256, "%Y-%m-%d %T", local_time);

    const char * program_name = g_get_prgname();
    if (!program_name)
        program_name = "<unknown>";

    fprintf(stream, "%s[%s]%s %s [%s%s%s] ",
        color(modifier_color, stream),
        modifier,
        color(NORMAL, stream),
        time_buffer,
        color(LIGHT_YELLOW, stream),
        program_name,
        color(NORMAL, stream)
    );

    vfprintf(stream, format, ap);

    int format_len = strlen(format);
    if (format_len == 0 || format[format_len - 1] != '\n')
        fprintf(stream, "\n");

}

/********************************************************************/

void su_log_message(SU_LOG_LEVEL level, const char * format, ...)
{
    va_list ap;
    va_start(ap, format);
    su_log_message_va(level, format, ap);
    va_end(ap);
}

/********************************************************************/

void su_log_error(const char * format, ...)
{
    va_list ap;
    va_start(ap, format);
    su_log_message_va(SU_LOG_ERROR, format, ap);
    va_end(ap);
}

void su_log_warning(const char * format, ...)
{
    va_list ap;
    va_start(ap, format);
    su_log_message_va(SU_LOG_WARNING, format, ap);
    va_end(ap);
}

void su_log_info(const char * format, ...)
{
    va_list ap;
    va_start(ap, format);
    su_log_message_va(SU_LOG_INFO, format, ap);
    va_end(ap);
}

void su_log_debug(const char * format, ...)
{
    va_list ap;
    va_start(ap, format);
    su_log_message_va(SU_LOG_DEBUG, format, ap);
    va_end(ap);
}

void su_log_debug2(const char * format, ...)
{
    va_list ap;
    va_start(ap, format);
    su_log_message_va(SU_LOG_DEBUG_SPAM, format, ap);
    va_end(ap);
}

/********************************************************************/

void su_print_error_message(const char *string, ...)
{
    fprintf(stderr, "%s: ", g_get_prgname());

    va_list ap;
    va_start(ap, string);

    vfprintf(stderr, string, ap);

    va_end(ap);
}

