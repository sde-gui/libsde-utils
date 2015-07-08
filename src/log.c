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
#include <time.h>

int su_log_level = -1;


#define RED_COLOR               "\033[0;31m"
#define GREEN_COLOR             "\033[0;32m"
#define ORANGE_COLOR            "\033[0;33m"
#define BLUE_COLOR              "\033[0;34m"
#define TEAL_COLOR              "\033[0;36m"
#define LIGHT_YELLOW_COLOR      "\033[1;33m"
#define LIGHT_BLUE_COLOR        "\033[1;34m"
#define LIGHT_PURPLE_COLOR      "\033[1;35m"
#define NORMAL_COLOR            "\033[0m"


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
    switch (level)
    {
            case SU_LOG_ERROR:
            {
                    modifier = RED_COLOR "[ERR] " NORMAL_COLOR;
                    break;
            }
            case SU_LOG_WARNING:
            {
                    modifier = ORANGE_COLOR "[WRN] " NORMAL_COLOR;
                    break;
            }
            case SU_LOG_INFO:
            {
                    modifier = GREEN_COLOR "[INF] " NORMAL_COLOR;
                    break;
            }
            case SU_LOG_DEBUG:
            {
                    modifier = TEAL_COLOR "[DBG] " NORMAL_COLOR;
                    break;
            }
            case SU_LOG_DEBUG_SPAM:
            {
                    modifier = BLUE_COLOR "[DBG] " NORMAL_COLOR;
                    break;
            }
            default:
            {
                    modifier = RED_COLOR "[XXX] " NORMAL_COLOR;
                    break;
            }
    }

    time_t current_time = time(NULL);
    struct tm * local_time = localtime(&current_time);
    char time_buffer[256];
    time_buffer[0] = 0;
    if (local_time != NULL)
    {
            strftime(time_buffer, 256, "%Y-%m-%d %T ", local_time);
    }

    const char * f = "%s%s["LIGHT_YELLOW_COLOR"%s"NORMAL_COLOR"] %s\n";
    int format_len = strlen(format);
    if (format_len > 0 && format[format_len - 1] == '\n')
        f = "%s%s["LIGHT_YELLOW_COLOR"%s"NORMAL_COLOR"] %s";

    const char * program_name = g_get_prgname();
    if (!program_name)
        program_name = "<unknown>";

    int len = strlen(format) + strlen(modifier) + strlen(time_buffer) + strlen(program_name) + 20;

    char * buffer = (char *) malloc(len + 1);

    snprintf(buffer, len, f, modifier, time_buffer, program_name, format);

    buffer[len] = 0;

    vfprintf(stream, buffer, ap);

    free(buffer);
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

