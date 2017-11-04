/** looker: yet another video player which can remember
  * the position where you stop looking video to reopen it later.
  * Copyright (C) 2017 Brüggemann Eddie
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  ***********************************************************************/

#include "./configuration.h"

static gboolean configuration_file_exists(const gchar *filepath) ;

static void set_default_configuration(GtkWidget *window) ;


void get_configuration(GtkWidget *window) {

  if (! configuration_file_exists(PATH_TO_CONF_FILE)) {

    set_default_configuration(window) ;

    register_at_exit = true ;

    return ;

  }

  /** Program configuration file. **/
  GKeyFile *conf_file = g_key_file_new() ;

  GError *error = NULL ;

  g_key_file_load_from_file(conf_file, PATH_TO_CONF_FILE, G_KEY_FILE_NONE, &error) ;

  if (error != NULL) {

    display_error_message(window, error->message) ;

    g_clear_error(&error)  ;

    return ;
  }

  /** Read the configuration file: getting boolean or integer values. **/
  register_at_exit = (bool) g_key_file_get_boolean(conf_file,  "looker-config", "register_at_exit", NULL) ;
  snapshot_location = g_key_file_get_string(conf_file,  "looker-config", "snapshot_location", NULL) ;
  g_key_file_unref(conf_file) ;

  return ;
}

void set_configuration(void) {

  GKeyFile *conf_file = g_key_file_new() ;

  g_key_file_set_boolean(conf_file,   "looker-config", "register_at_exit", (gboolean) register_at_exit);
  g_key_file_set_string(conf_file,   "looker-config", "snapshot_location", snapshot_location);

  g_key_file_save_to_file(conf_file, PATH_TO_CONF_FILE, NULL);

  g_key_file_unref(conf_file) ;

  return ;
}

static void set_default_configuration(GtkWidget *window) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GKeyFile *conf_file = g_key_file_new() ;

  register_at_exit = true ;

  g_key_file_set_boolean(conf_file,   "looker-config", "register_at_exit", (gboolean) register_at_exit);
  g_key_file_set_string(conf_file,   "looker-config", "snapshot_location", snapshot_location);

  g_key_file_save_to_file(conf_file, PATH_TO_CONF_FILE, &error);

  if (error != NULL) {

    display_error_message(window, error->message) ;

    g_clear_error(&error) ;

    return ;

  }

  g_key_file_unref(conf_file) ;

}

static gboolean configuration_file_exists(const gchar *filepath) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (! g_file_test(filepath, G_FILE_TEST_EXISTS)) {

    /** create the configuration folder if doesn't exist. **/

    gchar *config_dirpath = g_path_get_dirname(filepath) ;

    g_mkdir_with_parents(config_dirpath, 0766) ;

    g_free(config_dirpath) ;

    int fd = g_creat(filepath, 0777) ;

    g_close(fd, NULL) ;

    return FALSE ;

  }

  return TRUE ;

}

