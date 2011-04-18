/* See LICENSE file for copyright and license details. */
#include "input.h"

#include <gmodule.h>

static const char* plugin_names[] = {
  "input_sndfile",
  "input_mpg123",
  "input_musepack",
  "input_ffmpeg",
  "input_ffmpeg0.5.2",
  "input_ffmpeg0.6.2",
  NULL
};

extern char* av0;
static const char* plugin_search_dir[] = {
  ".",
  "r128",
  "",
  NULL, /* = g_path_get_dirname(av0); */
  NULL
};

static GSList* g_modules = NULL;
static GSList* plugin_ops = NULL; /*struct input_ops* ops;*/
static GSList* plugin_exts = NULL;

static int plugin_forced = 0;

int input_init(const char* forced_plugin) {
  int plugin_found = 0;
  int plugin_search_dir_index = 0;
  const char** cur_plugin_name = plugin_names;
  struct input_ops* ops;
  char** exts;
  GModule* module;

  char* exe_dir = g_path_get_dirname(av0);
  plugin_search_dir[3] = exe_dir;

  if (forced_plugin) plugin_forced = 1;
  /* Load plugins */
  while (*cur_plugin_name) {
    if (forced_plugin && strcmp(forced_plugin, (*cur_plugin_name) + 6)) {
      ++cur_plugin_name;
      continue;
    }
    ops = NULL;
    exts = NULL;
    module = NULL;
    plugin_search_dir_index = 0;
    while (!module && plugin_search_dir[plugin_search_dir_index]) {
      char* path = g_module_build_path(plugin_search_dir[plugin_search_dir_index],
                                       *cur_plugin_name);
      fprintf(stderr, "%s\n", path);
      module = g_module_open(path,
                             G_MODULE_BIND_LAZY | G_MODULE_BIND_LOCAL);
      g_free(path);
      ++plugin_search_dir_index;
    }
    if (!module) {
      /* fprintf(stderr, "%s\n", g_module_error()); */
    } else {
      if (!g_module_symbol(module, "ip_ops", (gpointer*) &ops)) {
        fprintf(stderr, "%s: %s\n", *cur_plugin_name, g_module_error());
      }
      if (!g_module_symbol(module, "ip_exts", (gpointer*) &exts)) {
        fprintf(stderr, "%s: %s\n", *cur_plugin_name, g_module_error());
      }
    }
    if (ops) {
      ops->init_library();
      plugin_found = 1;
    }
    g_modules = g_slist_append(g_modules, module);
    plugin_ops = g_slist_append(plugin_ops, ops);
    plugin_exts = g_slist_append(plugin_exts, exts);
    ++cur_plugin_name;
  }
  g_free(exe_dir);
  if (!plugin_found) {
    fprintf(stderr, "Warning: no plugins found!\n");
    return 1;
  }
  return 0;
}

int input_deinit() {
  /* unload plugins */
  GSList* ops = plugin_ops;
  GSList* modules = g_modules;
  while (ops && modules) {
    if (ops->data && modules->data) {
      ((struct input_ops*) ops->data)->exit_library();
      if (!g_module_close((GModule*) modules->data)) {
        fprintf(stderr, "%s\n", g_module_error());
      }
    }
    ops = g_slist_next(ops);
    modules = g_slist_next(modules);
  }
  g_slist_free(g_modules);
  g_slist_free(plugin_ops);
  g_slist_free(plugin_exts);
  return 0;
}

struct input_ops* input_get_ops(const char* filename) {
  GSList* ops = plugin_ops;
  GSList* exts = plugin_exts;
  char* filename_ext = strrchr(filename, '.');
  if (filename_ext) {
    ++filename_ext;
  } else {
    return NULL;
  }
  while (ops && exts) {
    if (ops->data && exts->data) {
      const char** cur_exts = exts->data;
      while (*cur_exts) {
        if (!strcmp(filename_ext, *cur_exts) || plugin_forced) {
          return (struct input_ops*) ops->data;
        }
        ++cur_exts;
      }
    }
    ops = g_slist_next(ops);
    exts = g_slist_next(exts);
  }
  return NULL;
}