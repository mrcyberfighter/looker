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

#include "./error_message.h"


inline void display_error_message(GtkWidget *window, const char *msg) {

  GtkWidget *msg_widget = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "%s", msg) ;

  gtk_dialog_run(GTK_DIALOG(msg_widget)) ;

  gtk_widget_destroy(msg_widget) ;

  return ;

}

