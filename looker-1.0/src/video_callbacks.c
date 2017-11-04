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

#include "./video_callbacks.h"

static void register_movie(GtkWindow *window) ;

static void unregister_movie(GtkWidget *window) ;

static void replay_from_timestamp(GtkWindow *window) ;

static gchar *get_title(libvlc_media_t *media) ;

static bool calculate_time(libvlc_media_t *media) ;

void choose_video(GtkWidget *widget, GtkWidget *controls) {

  #if 1
  if (gtk_widget_is_visible(controls)) {

    gtk_widget_hide(controls) ;

    is_controls_on = FALSE ;

  }
  #endif

  GtkWidget *window = gtk_widget_get_toplevel(widget) ;

  GtkWidget *file_chooser = gtk_file_chooser_dialog_new("Choose a video to play" , GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_OPEN, "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_OK, NULL) ;


  gint res = gtk_dialog_run(GTK_DIALOG(file_chooser)) ;

  if (res == GTK_RESPONSE_OK) {

    if (media != NULL) {

      libvlc_media_release(media) ;

      media = NULL ;

    }

    GFile *g_file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(file_chooser)) ;

    open_movie(GTK_WINDOW(window), g_file) ;

  }

  gtk_widget_destroy(file_chooser) ;

  return ;

}

void ask_for_registering(GtkWidget *window, GtkWidget *controls) {

  gtk_widget_destroy(controls) ;

  if (libvlc_media_player_is_playing(media_player)) {

    libvlc_media_player_pause(media_player) ;

  }

  GtkWidget *message_dialog = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "register this movie to continue to watch it later ?") ;

  gint ret = gtk_dialog_run(GTK_DIALOG(message_dialog)) ;

  switch (ret) {

    case GTK_RESPONSE_YES :

      register_movie(GTK_WINDOW(window)) ;
      break ;

    case GTK_RESPONSE_NO :

      unregister_movie(window) ;
      break ;

  }

  gtk_widget_destroy(message_dialog) ;

  return ;
}

void open_movie(GtkWindow *window, GFile *file) {

  g_free(media_uri) ;

  media_uri = g_file_get_uri(file) ;

  g_object_unref(file) ;

  if (media != NULL) {

    libvlc_media_release(media) ;
    media = NULL ;

  }

  media = libvlc_media_new_location(vlc_inst, media_uri);

  libvlc_media_player_set_media(media_player, media);

  if (libvlc_media_player_play(media_player) == -1)  {

    display_error_message(GTK_WIDGET(window), libvlc_errmsg()) ;

    return ;

  }

  libvlc_media_parse(media) ;

  while (! libvlc_media_is_parsed(media)) {

    g_main_context_iteration(NULL, FALSE) ;

  }

  char *title = NULL ;

  title = get_title(media) ;

  if (title != NULL) {

    gtk_window_set_title(GTK_WINDOW(window), title) ;

  }

  g_free(title) ;

  while (! libvlc_media_player_is_playing(media_player)) {

    g_main_context_iteration(NULL, FALSE) ;

  }

  if (! calculate_time(media)) {

    display_error_message(GTK_WIDGET(window), "Error computing timestamps. Try to relaunch the program.") ;

    exit(EXIT_FAILURE) ;

  }

  replay_from_timestamp(GTK_WINDOW(window)) ;

  unsigned int px = 0 ;
  unsigned int py = 0 ;

  if (libvlc_video_get_size(media_player,0,&px, &py) == 0)  {

    gtk_window_resize(window, px, py);

    #ifdef DEBUG
    fprintf(stderr,"px: %d py: %d\n", px, py) ;
    #endif

  }
  else {

    gtk_window_maximize(window) ;

  }

  return ;

}

static void register_movie(GtkWindow *window) {

  g_bookmark_file_add_application(bookmark, media_uri, NULL, NULL);

  gchar *timestamp = g_strdup_printf("%li", libvlc_media_player_get_time(media_player) - 1000) ;

  g_bookmark_file_set_description(bookmark, media_uri, timestamp);

  g_free(timestamp) ;

  g_bookmark_file_to_file(bookmark, PATH_TO_BOOKMARK, &error) ;

  if (error != NULL) {

    display_error_message(GTK_WIDGET(window), error->message) ;

    g_clear_error(&error) ;

  }

  return ;
}

static inline void unregister_movie(GtkWidget *window) {

  if (g_bookmark_file_has_item(bookmark,media_uri)) {

    g_bookmark_file_remove_item(bookmark, media_uri, &error) ;

    if (error != NULL) {

      display_error_message(window, error->message) ;

      g_clear_error(&error) ;

    }

  }

  return ;

}

static void replay_from_timestamp(GtkWindow *window) {

  if (g_bookmark_file_has_item(bookmark, media_uri)) {

    char *timestamp = NULL ;

    timestamp = g_bookmark_file_get_description(bookmark, media_uri, &error) ;

    if (error != NULL) {

      display_error_message(GTK_WIDGET(window), error->message) ;

      g_clear_error(&error) ;

      goto end_of_function ;

    }

    int64_t timestamp_ms = -1 ;

    sscanf(timestamp,"%li", &timestamp_ms) ;

    // Make timestamp one second back.
    timestamp_ms -= 1000 * 60 ;

    g_free(timestamp) ;

    libvlc_media_player_set_time(media_player, timestamp_ms) ;

  }

  end_of_function :

  return ;

}

static inline gchar *get_title(libvlc_media_t *media) {

  char *title =  NULL ;

  title = libvlc_media_get_meta(media, libvlc_meta_ShowName) ;

  if (title == NULL) {

    title = libvlc_media_get_meta(media, libvlc_meta_Title) ;
  }

  return title ;

}

static bool calculate_time(libvlc_media_t *media) {

  GTimeVal *time_val = g_malloc(sizeof(GTimeVal)) ;

  libvlc_time_t duration = libvlc_media_get_duration(media) ;

  if (duration == -1) {

    return false ;
  }

  duration_min = ( duration / 1000.0) / 60 ;

  duration_sec = ((int) (duration / 1000.0))  % 60 ;

  if (duration_min < 0 || duration_sec < 0)  {

    return false ;

  }

  time_val->tv_sec = (duration / 1000.0) ;

  time_val->tv_usec = 0 ;

  datetime = g_date_time_new_from_timeval_utc(time_val);

  if (datetime == NULL) {

    g_free(time_val) ;

    return false ;

  }

  g_free(time_val) ;

  factor = duration_min * 60.0 + duration_sec ;

  return true ;

}


