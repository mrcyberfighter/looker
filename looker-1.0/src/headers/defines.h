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


#ifndef LOOKER_1_0_DEFINES_HH /** Inclusion guard **/
#define LOOKER_1_0_DEFINES_HH

/** About definition: **/
#define AUTHOR      "Eddie Brüggemann"
#define MAIL        "<mrcyberfighter@gmail.com>"
#define PRGNAME     "looker"
#define VERSION     "1.0"
#define LICENSE     "GPLv3"
#define PRGWEBURL   "http://www.open-source-projects.net/looker/looker"
#define COPYRIGHT   "Copyright (C) 2017 GPLv3 "

/** Conditional Prefix definition. **/
#ifndef PREFIX
#define ROOT
#define PREFIX ".."

/** You can define PREFIX by distributing with a preprocessor directive:
  *
  * $(MAKE) -B CPPFLAGS="-D'PREFIX=\"$(prefix)\"'"
  *
  ***********************************************************************/

#endif


/** Conditional Prefix definition. **/
#ifndef PREFIX
#define ROOT
#define PREFIX ".."
#endif


/** Path definitions: **/
#ifdef ROOT
#define DATADIR                 PREFIX "/"
#else
#define DATADIR                 PREFIX "/share" "/" PRGNAME
#endif


#define PRGPATH                 PREFIX "/bin"   "/" PRGNAME






#define PATH_TO_ICON              DATADIR      "/looker-icon.png"

#define PATH_TO_CONF_FILE         DATADIR "/config" "/configuration.ini"

#define PATH_TO_ICON_PLAY         DATADIR "/icons" "/32x32" "/media-playback-start.png"
#define PATH_TO_ICON_PAUSE        DATADIR "/icons" "/32x32" "/media-playback-pause.png"

#define PATH_TO_PREV_CHAPTER      DATADIR "/icons" "/32x32" "/media-skip-backward.png"
#define PATH_TO_NEXT_CHAPTER      DATADIR "/icons" "/32x32" "/media-skip-forward.png"

#define PATH_TO_ICON_FULLSCREEN   DATADIR "/icons" "/32x32" "/gnome-session-reboot.png"
#define PATH_TO_ICON_UNFULLSCREEN DATADIR "/icons" "/32x32" "/gnome-session-switch.png"


#define PATH_TO_LOAD_VIDEO        DATADIR "/icons" "/16x16" "/folder-video.png"

#define PATH_TO_SNAPSHOT          DATADIR "/icons" "/16x16" "/ksnapshot.png"

#define PATH_TO_PREFERENCES       DATADIR "/icons" "/16x16" "/run-build-configure.png"

#define PATH_TO_ABOUT             DATADIR "/icons" "/16x16" "/help-about.png"

#define PATH_TO_PLUS_VOLUME       DATADIR "/icons" "/16x16" "/audio-volume-high.png"

#define PATH_TO_MINUS_VOLUME      DATADIR "/icons" "/16x16" "/audio-volume-low.png"

#define PATH_TO_FLUSH_BOOKMARK    DATADIR "/icons" "/16x16" "/edit-clear-history.png"

#define PATH_TO_VOLUME_BUTTON     DATADIR "/icons" "/22x22" "/preferences-desktop-sound.png"

#define PATH_TO_MENU_BUTTON       DATADIR "/icons" "/22x22" "/application-default-icon.png"

#define PATH_TO_BOOKMARK          DATADIR "/bookmark" "/bookmark.xbel"

#define PATH_TO_GPL_TEXT          DATADIR "/license" "/gpl.txt"


/** Uncomment to use gettext
#define GETTEXT_SUPPORT */

#ifdef GETTEXT_SUPPORT
#include <libintl.h>

#ifdef ROOT

#ifdef  PACKAGE
#undef  PACKAGE
#endif

#define PACKAGE    "./"   PRGNAME


#ifdef  LOCALEDIR
#undef  LOCALEDIR
#endif

#else

#define PACKAGE  PREFIX "/bin" "/" PRGNAME

#endif /* ROOT */

#define LOCALEDIR  PREFIX "/share" "/locale"

#define _(String) gettext       (String)
#define gettext_noop(String)     String
#define N_(String) gettext_noop (String)

#endif /* GETTEXT_SUPPORT */

#define DEBUG_FUNC_MARK fprintf(stderr,"%s\n", __func__) ;

/** Uncomment to use debug features or use the target: make ddebug
#define DEBUG */

#ifdef DEBUG

#include <stdio.h>

#define DEBUG_PT          fprintf(stderr,"\n%s:%s(...) at line: %d\n", __FILE__, __func__, __LINE__) ;
#define DEBUG_FUNC_MARK   fprintf(stderr,"\n%s:%s(...) called\n",__FILE__, __func__) ;

#define DEBUG_INT(x)      fprintf(stderr,"DEBUG PT N° %d\n",   (x)) ;
#define DEBUG_LINT(x)     fprintf(stderr,"DEBUG PT N° %d\n",   (x)) ;
#define DEBUG_LLINT(x)    fprintf(stderr,"DEBUG PT N° %d\n",   (x)) ;

#define DEBUG_FLOAT(x)    fprintf(stderr,"DEBUG PT N° %f\n",   (x)) ;
#define DEBUG_DOUBLE(x)   fprintf(stderr,"DEBUG PT N° %lf\n",  (x)) ;
#define DEBUG_LDOUBLE(x)  fprintf(stderr,"DEBUG PT N° %llf\n", (x)) ;

#define DEBUG_STR(str)    fprintf(stderr,"%s\n", (str)) ;
#define DEBUG_PTR(ptr)    fprintf(stderr,"%p\n", (ptr)) ;


/** Extended DEBUGGING **/
#define EDEBUG_INT(str, x)      fprintf(stderr,"%s(line:%d): %s %d\n",   __func__, __LINE__, (str), (x)) ;
#define EDEBUG_LINT(str, x)     fprintf(stderr,"%s(line:%d): %s %d\n",   __func__, __LINE__, (str), (x)) ;
#define EDEBUG_LLINT(str, x)    fprintf(stderr,"%s(line:%d): %s %d\n",   __func__, __LINE__, (str), (x)) ;

#define EDEBUG_FLOAT(str, x)    fprintf(stderr,"%s(line:%d): %s %f\n",   __func__, __LINE__, (str), (x)) ;
#define EDEBUG_DOUBLE(str, x)   fprintf(stderr,"%s(line:%d): %s %lf\n",  __func__, __LINE__, (str), (x)) ;
#define EDEBUG_LDOUBLE(str, x)  fprintf(stderr,"%s(line:%d): %s %llf\n", __func__, __LINE__, (str), (x)) ;

#define EDEBUG_STR(str)         fprintf(stderr,"%s(%d): %s\n", __func__, __LINE__, (str)) ;
#define EDEBUG_PTR(ptr)         fprintf(stderr,"%s(%d): %s\n", __func__, __LINE__, (ptr)) ;

#endif




#endif /** Inclusion guard **/
