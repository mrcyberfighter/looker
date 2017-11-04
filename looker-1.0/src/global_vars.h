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

#ifndef LOOKER_GLOBAL_VARS_HH
#define LOOKER_GLOBAL_VARS_HH

#include "./headers/includes.h"


extern libvlc_media_player_t *media_player ;
extern libvlc_instance_t *vlc_inst ;
extern libvlc_media_t *media ;


extern long int duration_min ;
extern long int duration_sec ;

extern GDateTime *datetime ;

extern GBookmarkFile *bookmark ;

extern gchar *media_uri;

extern float factor ;

extern GRecMutex mainloop_ctrl_rec_mutex  ;

extern GtkWidget *timeline_scale ;

#ifdef MAC_INTEGRATION
gint attention ;
#endif

extern GError *error ;

extern const gchar *mimetype[] ;

// These variables is not local to on_key_press for better toggled support.
extern gboolean is_controls_on;
extern gboolean is_controls_key_pressed;

extern bool register_at_exit;

extern gchar *snapshot_location ;

#endif
