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

#include <sde-utils/paths.h>
#include <sde-utils/proc.h>
#include <sde-utils/strings.h>
#include <sde-utils/log.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>

#define CONFIG_RESOURCE_PREFIX "sde-config"
#define TEMPLATE_PROFILE "template"
#define AGENT_SPECIFIC_RESOURCE "sde-agents"
#define COMMON_RESOURCE "sde-common"

#define SYSTEM_LOCAL_RESOURCE_PREFIX "/usr/local/"

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

#define RES_USER_DATA_DIR (1 << 0)
#define RES_AUTODETECTED_DIR (1 << 1)

static unsigned _res_flags = 0;
static gboolean _res_flags_initialized = 0;

static unsigned _get_res_flags()
{
    if (_res_flags_initialized)
        return _res_flags;

    gboolean disable_user_data_dir = FALSE;
    gboolean disable_autodetected_dir = FALSE;

    gboolean id0 = (getuid() == 0 || geteuid() == 0 || getgid() == 0 || getegid() == 0);

    disable_user_data_dir |= id0;
    disable_user_data_dir |= (g_strcmp0(g_getenv("SDE_ALLOW_USER_DATA_DIR_RESOURCE_RESOLUTION"), "TRUE") != 0);

    disable_autodetected_dir |= id0;
    disable_autodetected_dir |= (g_strcmp0(g_getenv("SDE_ALLOW_AGENT_PREFIX_AUTODETECTION"), "TRUE") != 0);

    if (!disable_user_data_dir)
        _res_flags = _res_flags | RES_USER_DATA_DIR;
    if (!disable_autodetected_dir)
        _res_flags = _res_flags | RES_AUTODETECTED_DIR;

    _res_flags_initialized = TRUE;

    return _res_flags;
}

/********************************************************************/

static GHashTable * default_agent_prefix_table = NULL;
static GHashTable * autodetected_agent_prefix_table = NULL;

/********************************************************************/

gchar * su_path_resolve_agent_id_by_path(const char * path, const char * default_id)
{
    if (!path)
        return g_strdup(default_id);

    if (!(_get_res_flags() & RES_AUTODETECTED_DIR))
        return g_strdup(default_id);

    const char * pattern = "/" AGENT_SPECIFIC_RESOURCE "/";

    const char * s = g_strstr_len(path, -1, pattern);
    if (s)
    {
        s = s + strlen(pattern);
        const char * end = g_strstr_len(s, -1, "/");
        if (end)
        {
            size_t agent_id_size = end - s;
            size_t prefix_size = end - path;

            gchar * agent_id = g_strndup(s, agent_id_size);
            gchar * prefix = g_strndup(path, prefix_size);

            if (!autodetected_agent_prefix_table)
            {
                autodetected_agent_prefix_table = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
            }

            g_hash_table_insert(autodetected_agent_prefix_table, agent_id, prefix);

            su_log_debug("path %s autodetected as agent %s with prefix %s\n", path, agent_id, prefix);

            return g_strdup(agent_id);
        }
    }

    return g_strdup(default_id);
}

gchar * su_path_resolve_agent_id_by_pointer(void * p, const char * default_id)
{
    return su_path_resolve_agent_id_by_path(su_get_module_path(p), default_id);
}

/********************************************************************/

void su_path_register_default_agent_prefix(const char * agent_id, const char * prefix)
{
    if (!agent_id)
        return;

    if (!default_agent_prefix_table)
    {
        default_agent_prefix_table = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
    }

    g_hash_table_insert(default_agent_prefix_table, g_strdup(agent_id), g_strdup(prefix));

    su_log_debug("agent %s registered with default prefix %s\n", agent_id, prefix);
}

/********************************************************************/

static gchar * _path_resolve_resource_va(const char * agent_id, unsigned flags, va_list ap)
{
    gchar * result = NULL;
    gchar * resource_id = su_build_filename_va(ap);

    #define RETURN \
        if (g_file_test(result, G_FILE_TEST_EXISTS)) \
            goto end; \
        g_free(result); \
        result = NULL;

    flags = flags & _get_res_flags();

    if (flags & RES_USER_DATA_DIR)
    {
        if (!su_str_empty(agent_id))
        {
            result = g_build_filename(g_get_user_data_dir(), AGENT_SPECIFIC_RESOURCE, agent_id, resource_id, NULL);
            RETURN;
            result = g_build_filename(g_get_user_data_dir(), AGENT_SPECIFIC_RESOURCE, agent_id, "share", resource_id, NULL);
            RETURN;
        }

        result = g_build_filename(g_get_user_data_dir(), COMMON_RESOURCE, resource_id, NULL);
        RETURN;
        result = g_build_filename(g_get_user_data_dir(), COMMON_RESOURCE, "share", resource_id, NULL);
        RETURN;
    }


    if (!su_str_empty(agent_id))
    {
        result = g_build_filename(SYSTEM_LOCAL_RESOURCE_PREFIX, AGENT_SPECIFIC_RESOURCE, agent_id, resource_id, NULL);
        RETURN;
        result = g_build_filename(SYSTEM_LOCAL_RESOURCE_PREFIX, AGENT_SPECIFIC_RESOURCE, agent_id, "share", resource_id, NULL);
        RETURN;
    }

    result = g_build_filename(SYSTEM_LOCAL_RESOURCE_PREFIX, COMMON_RESOURCE, resource_id, NULL);
    RETURN;
    result = g_build_filename(SYSTEM_LOCAL_RESOURCE_PREFIX, COMMON_RESOURCE, "share", resource_id, NULL);
    RETURN;

    if ((flags & RES_AUTODETECTED_DIR) && !su_str_empty(agent_id) && autodetected_agent_prefix_table)
    {
        const char * prefix = g_hash_table_lookup(autodetected_agent_prefix_table, agent_id);

        if (prefix)
        {
            result = g_build_filename(prefix, resource_id, NULL);
            RETURN;
            result = g_build_filename(prefix, "share", resource_id, NULL);
            RETURN;
        }
    }

    if (!su_str_empty(agent_id) && default_agent_prefix_table)
    {
        const char * prefix = g_hash_table_lookup(default_agent_prefix_table, agent_id);

        if (prefix)
        {
            result = g_build_filename(prefix, resource_id, NULL);
            RETURN;
            result = g_build_filename(prefix, "share", resource_id, NULL);
            RETURN;
        }
    }

    #undef RETURN

end:
    su_log_debug("resource %s:%s resolved as \"%s\"\n",
        agent_id ? agent_id : "(NULL)", resource_id, result ? result : "(NULL)");

    g_free(resource_id);
    return result;
}

gchar * su_path_resolve_resource_va(const char * agent_id, va_list ap)
{
    return _path_resolve_resource_va(agent_id, 0xFFFF, ap);
}

gchar * su_path_resolve_resource(const char * agent_id, ...)
{
    va_list ap;
    va_start(ap, agent_id);
    return su_path_resolve_resource_va(agent_id, ap);
    va_end(ap);
}

gchar * su_path_resolve_config_va(const char * agent_id, SU_PATH_CONFIG_TYPE type, const char * domain, const char * profile, va_list ap)
{
    gchar * result = NULL;
    gchar * suffix = su_build_filename_va(ap);

again:

    /* $XDG_CONFIG_HOME */
    if (type == SU_PATH_CONFIG_USER || type == SU_PATH_CONFIG_USER_W)
    {
        if (profile)
            result = g_build_filename(g_get_user_config_dir(), domain, profile, suffix, NULL);
        else
            result = g_build_filename(g_get_user_config_dir(), domain, suffix, NULL);

        if (type == SU_PATH_CONFIG_USER_W)
        {
            gchar * dirname = g_path_get_dirname(result);
            if (!g_file_test(dirname, G_FILE_TEST_EXISTS))
                g_mkdir_with_parents(dirname, 0755);
            g_free(dirname);
            goto end;
        }

        if (g_file_test(result, G_FILE_TEST_EXISTS))
            goto end;

        g_free(result);
        result = NULL;
    }

    /* $XDG_CONFIG_DIRS */
    const gchar * const * dirs = g_get_system_config_dirs();
    for (; *dirs; dirs++)
    {
        if (profile)
            result = g_build_filename(*dirs, domain, profile, suffix, NULL);
        else
            result = g_build_filename(*dirs, domain, suffix, NULL);

        if (g_file_test(result, G_FILE_TEST_EXISTS))
            goto end;

        g_free(result);
        result = NULL;
    }


    {
        gchar * resource_id = NULL;
        if (profile)
            resource_id = g_build_filename(CONFIG_RESOURCE_PREFIX, domain, profile, suffix, NULL);
        else
            resource_id = g_build_filename(CONFIG_RESOURCE_PREFIX, domain, suffix, NULL);

        result = su_path_resolve_resource(agent_id, resource_id, NULL);
        g_free(resource_id);

        if (result && g_file_test(result, G_FILE_TEST_EXISTS))
            goto end;

        g_free(result);
        result = NULL;
    }

    if (profile && strcmp(profile, TEMPLATE_PROFILE) != 0)
    {
        profile = TEMPLATE_PROFILE;
        goto again;
    }

end:
    su_log_debug("config %s:%s:%s:%s resolved as \"%s\"\n", domain, profile ? profile : "", suffix,
        type == SU_PATH_CONFIG_USER_W ? "USER_W" :
        type == SU_PATH_CONFIG_USER ? "USER" :
        "SYSTEM",
        result ? result : "(NULL)");

    g_free(suffix);
    return result;
}

gchar * su_path_resolve_config(const char * agent_id, SU_PATH_CONFIG_TYPE type, const char * domain, const char * profile, ...)
{
    va_list ap;
    va_start(ap, profile);
    return su_path_resolve_config_va(agent_id, type, domain, profile, ap);
    va_end(ap);
}

/********************************************************************/

FILE * su_path_fopen_resource_va(const char * mode, const char * agent_id, va_list ap)
{
    FILE * file = NULL;
    gchar * path = su_path_resolve_resource_va(agent_id, ap);
    if (path)
    {
        if (!mode)
            mode = "r";
        file = fopen(path, mode);
        g_free(path);
    }
    return file;
}

FILE * su_path_fopen_resource(const char * mode, const char * agent_id, ...)
{
    va_list ap;
    va_start(ap, agent_id);
    return su_path_fopen_resource_va(mode, agent_id, ap);
    va_end(ap);
}

FILE * su_path_fopen_config_va(const char * mode, const char * agent_id, SU_PATH_CONFIG_TYPE type, const char * domain, const char * profile, va_list ap)
{
    FILE * file = NULL;
    gchar * path = su_path_resolve_config_va(agent_id, type, domain, profile, ap);
    if (path)
    {
        if (!mode)
            mode = "r";
        file = fopen(path, mode);
        g_free(path);
    }
    return file;
}

FILE * su_path_fopen_config(const char * mode, const char * agent_id, SU_PATH_CONFIG_TYPE type, const char * domain, const char * profile, ...)
{
    va_list ap;
    va_start(ap, profile);
    return su_path_fopen_config_va(mode, agent_id, type, domain, profile, ap);
    va_end(ap);
}

/********************************************************************/

gchar * su_path_read_resource_va(const char * agent_id, va_list ap)
{
    gchar * contents = NULL;
    gchar * path = su_path_resolve_resource_va(agent_id, ap);
    if (path)
    {
        g_file_get_contents(path, &contents, NULL, NULL);
        g_free(path);
    }
    return contents;
}

gchar * su_path_read_resource(const char * agent_id, ...)
{
    va_list ap;
    va_start(ap, agent_id);
    return su_path_read_resource_va(agent_id, ap);
    va_end(ap);
}

gchar * su_path_read_config_va(const char * agent_id, SU_PATH_CONFIG_TYPE type, const char * domain, const char * profile, va_list ap)
{
    gchar * contents = NULL;
    gchar * path = su_path_resolve_config_va(agent_id, type, domain, profile, ap);
    if (path)
    {
        g_file_get_contents(path, &contents, NULL, NULL);
        g_free(path);
    }
    return contents;
}

gchar * su_path_read_config(const char * agent_id, SU_PATH_CONFIG_TYPE type, const char * domain, const char * profile, ...)
{
    va_list ap;
    va_start(ap, profile);
    return su_path_read_config_va(agent_id, type, domain, profile, ap);
    va_end(ap);
}

/********************************************************************/

gchar * su_build_filename_va(va_list ap)
{
    gchar * args[256];
    int i = 0;

    gchar * arg;
    do {
        arg = va_arg(ap, gchar *);
        args[i++] = arg;
        if (i >= 255)
            return NULL;
    } while (arg);

    return g_build_filenamev(args);
}

/********************************************************************/
