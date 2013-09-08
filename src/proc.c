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

#define _GNU_SOURCE
#include <dlfcn.h>
#include "proc.h"
#include <stdint.h>

/********************************************************************/

const char * su_get_module_path(void * p)
{
    Dl_info  info;
    if (dladdr(p, &info) != 0)
    {
        g_print("pointer 0x%llX identified as module %s\n", (unsigned long long)(uintptr_t)p, info.dli_fname);
        return info.dli_fname;
    }
    return NULL;
}
