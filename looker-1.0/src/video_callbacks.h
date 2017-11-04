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

#ifndef LOOKER_VIDEO_CALLBACKS_HH
#define LOOKER_VIDEO_CALLBACKS_HH

#include "./headers/includes.h"

#include "./headers/defines.h"

#include "./global_vars.h"

#include "./error_message.h"

extern void choose_video(GtkWidget *widget, GtkWidget *controls) ;

extern void open_movie(GtkWindow *window, GFile *file) ;

extern void ask_for_registering(GtkWidget *window, GtkWidget *controls) ;

#endif
