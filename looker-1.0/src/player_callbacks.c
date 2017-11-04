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

#include "player_callbacks.h"

static gboolean disable_volume_button(GtkWidget *button) ;

void decr_change_chapter(GtkWidget *widget) {

  libvlc_media_player_previous_chapter(media_player) ;

  return ;

}

void incr_change_chapter(GtkWidget *widget) {

  libvlc_media_player_next_chapter(media_player) ;

  return ;

}

void take_snapshot(GtkWidget *widget) {

  if (media == NULL) {
  
    /** No media loaded so no snapshot possible. **/
  
    return ;

  }

  GtkWidget *window = gtk_widget_get_toplevel(widget) ;

  unsigned int px , py ;

  px = py = -1 ;

  if (libvlc_video_get_size(media_player, 0, &px, &py) == -1) {

    display_error_message(window,"Error getting video size") ;

    return ;

  }


  gchar *tmp_filepath = NULL ;

  g_file_open_tmp("snapshot_XXXXXX.png",&tmp_filepath, NULL) ;

  g_remove(tmp_filepath) ;


  gchar *file_basename = g_path_get_basename(tmp_filepath) ;

  g_free(tmp_filepath) ;


  gchar *snapshot_filepath = g_build_filename(snapshot_location, file_basename, NULL) ;

  g_free(file_basename) ;


  libvlc_media_player_set_pause(media_player, 1) ;

  if (libvlc_video_take_snapshot(media_player, 0, snapshot_filepath, px, py) == -1) {

    display_error_message(window,"error taking snapshot !") ;

    return ;

  }


  gchar *msg = g_strdup_printf("Snapshot image created at:\n%s",snapshot_filepath) ;

  g_free(snapshot_filepath) ;

  GtkWidget *msg_widget = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "%s", msg) ;

  gtk_dialog_run(GTK_DIALOG(msg_widget)) ;

  gtk_widget_destroy(msg_widget) ;

  g_free(msg) ;

  libvlc_media_player_set_pause(media_player, 0) ;

  return ;

}

void incr_volume(GtkWidget *widget, GtkWidget *volume_button) {

  #if 1
  /** The shortcuts can fail because the controls are the top-level window.
    * And when the controls are above the main window the shortcut
    * may not work or work ?
    */

  GtkWidget *window = gtk_widget_get_toplevel(volume_button) ;

  gtk_window_set_keep_above(GTK_WINDOW(window), TRUE) ;

  gtk_window_set_focus(GTK_WINDOW(window), volume_button) ;

  gtk_widget_grab_focus(volume_button) ;
  #endif


  GtkWidget *popup = gtk_scale_button_get_popup(GTK_SCALE_BUTTON(volume_button));

  if (! gtk_widget_is_visible(popup)) {

    gtk_widget_show(popup) ;

  }

  gdouble value = gtk_scale_button_get_value( GTK_SCALE_BUTTON(volume_button)) ;

  value += 0.01 ;

  gtk_scale_button_set_value(GTK_SCALE_BUTTON(volume_button), value);

  /** Adding a timeout which hide the volume pop-up because else you must click onto the header bar to hide it. **/
  g_timeout_add_full(G_PRIORITY_DEFAULT, 4000, (GSourceFunc) disable_volume_button, volume_button, NULL) ;

  return ;

}

void decr_volume(GtkWidget *widget, GtkWidget *volume_button) {

  #if 1
  /** The shortcuts can fail because the controls are the top-level window.
    * And when the controls are above the main window the shortcut
    * may not work or work ?
    */

  GtkWidget *window = gtk_widget_get_toplevel(volume_button) ;

  gtk_window_set_keep_above(GTK_WINDOW(window), TRUE) ;

  gtk_window_set_focus(GTK_WINDOW(window), volume_button) ;

  gtk_widget_grab_focus(volume_button) ;
  #endif


  GtkWidget *popup = gtk_scale_button_get_popup(GTK_SCALE_BUTTON(volume_button));

  if (! gtk_widget_is_visible(popup)) {

    gtk_widget_show(popup) ;

  }

  gdouble value = gtk_scale_button_get_value( GTK_SCALE_BUTTON(volume_button)) ;

  value -= 0.01 ;

  gtk_scale_button_set_value(GTK_SCALE_BUTTON(volume_button), value);

  /** Adding a timeout which hide the volume pop-up because else you must click onto the header bar to hide it. **/
  g_timeout_add_full(G_PRIORITY_DEFAULT, 4000, (GSourceFunc) disable_volume_button, volume_button, NULL) ;

  return ;

}

void on_click_play_pause(GtkWidget *widget, gpointer user_data) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GtkWidget *image = gtk_button_get_image(GTK_BUTTON(widget)) ;

  gtk_image_clear(GTK_IMAGE(image)) ;

  if (libvlc_media_player_is_playing(media_player) == 1) {

    libvlc_media_player_pause(media_player) ;

    gtk_image_set_from_file(GTK_IMAGE(image), PATH_TO_ICON_PAUSE) ;

  }
  else {

    libvlc_media_player_play(media_player) ;

    gtk_image_set_from_file(GTK_IMAGE(image), PATH_TO_ICON_PLAY) ;

  }

  return ;

}


void set_fullscreen(GtkWidget *widget, GtkWidget *window) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GtkWidget *image = gtk_button_get_image(GTK_BUTTON(widget)) ;

  gtk_image_clear(GTK_IMAGE(image)) ;

  static gboolean is_fullscreen = FALSE ;

  if (! is_fullscreen) {

    #if 0
    libvlc_set_fullscreen(media_player, 1) ;
    #endif

    gtk_window_fullscreen(GTK_WINDOW(window)) ;

    gtk_image_set_from_file(GTK_IMAGE(image), PATH_TO_ICON_FULLSCREEN) ;


  }
  else {

    #if 0
    libvlc_set_fullscreen(media_player, 0) ;
    #endif

    gtk_window_unfullscreen(GTK_WINDOW(window)) ;

    gtk_window_maximize(GTK_WINDOW(window)) ;

    gtk_image_set_from_file(GTK_IMAGE(image), PATH_TO_ICON_UNFULLSCREEN) ;


  }

  is_fullscreen = ! is_fullscreen ;

  return ;

}


static gboolean disable_volume_button(GtkWidget *button) {

  /** FIXME: I hide the volume button into a single run timeout,
    *        because else it react on every click and so hide in an misfit manner.
    *        On the other side it's a form of auto hiding.
    ******************************************************************************/

  GtkWidget *widget = gtk_scale_button_get_popup(GTK_SCALE_BUTTON(button));

  gtk_widget_hide(widget) ;

  return FALSE ;
}


void set_volume(GtkScaleButton *button, gdouble value) {

  if (media_player == NULL) {

    return ;
  }

  libvlc_audio_set_volume(media_player, (int) (value * 100.0)) ;

  g_timeout_add_full(G_PRIORITY_DEFAULT, 4000, (GSourceFunc) disable_volume_button, button, NULL) ;

  return ;

}

