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

#include "./global_vars.h"

libvlc_media_player_t *media_player = NULL ;
libvlc_instance_t *vlc_inst = NULL ;
libvlc_media_t *media = NULL ;


long int duration_min = 0 ;
long int duration_sec = 0 ;

GDateTime *datetime = NULL ;

GBookmarkFile *bookmark = NULL ;

gchar *media_uri = NULL ;

float factor = 0.0f ;

GRecMutex mainloop_ctrl_rec_mutex  ;

GtkWidget *timeline_scale = NULL ;



GError *error = NULL ;

const gchar *mimetype[] = {  "video/avi", "video/x-ms-asf", "video/mpeg", "video/quicktime", "video/ogg", "application/ogg", "video/x-ms-wmv", "video/webm", "video/x-matroska", NULL } ;

// These variables is not local to on_key_press for better toggled support.
gboolean is_controls_on = FALSE ;
gboolean is_controls_key_pressed = FALSE ;

bool register_at_exit = true ;

gchar *snapshot_location = NULL ;
