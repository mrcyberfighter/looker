#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([looker], [1.0], [mrcyberfighter@gmail.com], , [http://www.open-source-projects.net/looker/looker])
AC_CONFIG_SRCDIR([.])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_LANG([C])

# Checks for programs.
AC_COPYRIGHT([GPLv3])

# COMPILER M4.
AC_PROG_CC
AC_PROG_CC_C_O
AC_PROG_CPP
AC_PROG_CC_STDC
AC_PROG_CC_C99
AC_C_CONST

AM_INIT_AUTOMAKE([foreign -Wall -Werror])

# Checks for programs.
# POSIX programs detection.
AC_PROG_INSTALL
AC_PROG_MKDIR_P
AC_PROG_MAKE_SET
#AC_PROG_SED

# coreutils package
AC_PATH_PROG([CHMOD],       [chmod],      [no])
AC_PATH_PROG([RM],          [rm],         [no])
AC_PATH_PROG([MV],          [mv],         [no])
AC_PATH_PROG([CP],          [cp],         [no])
AC_PATH_PROG([UNLINK],      [unlink],     [no])
AC_PATH_PROG([RMDIR],       [rmdir],      [no])
AC_PATH_PROG([TOUCH],       [touch],      [no])

AC_PATH_PROG([MD5SUM],      [md5sum],      [no])
AC_PATH_PROG([SHA1SUM],     [sha1sum],     [no])
AC_PATH_PROG([SHA256SUM],   [sha256sum],   [no])
AC_PATH_PROG([SHA512SUM],   [sha512sum],   [no])


AM_CONDITIONAL(HAVE_MD5, test ! x"$MD5SUM" = "xno")
AM_CONDITIONAL(HAVE_SHA1, test ! x"$SHA1SUM" = "xno")
AM_CONDITIONAL(HAVE_SHA256, test ! x"$SHA256SUM" = "xno")
AM_CONDITIONAL(HAVE_SHA512, test ! x"$SHA512SUM" = "xno")


if test x"$CHMOD" = xno ; then

  AC_MSG_ERROR(Install the coreutils package (chmod is missing) !, 1)

fi


if test x"$RM" = xno ; then

  AC_MSG_ERROR(Install the coreutils package (rm is missing) !, 1)

fi


if test x"$MV" = xno ; then

  AC_MSG_ERROR(Install the coreutils package (mv is missing) !, 1)

fi

if test x"$UNLINK" = xno ; then

  AC_MSG_ERROR(Install the coreutils package (unlink is missing) !, 1)

fi

if test x"$RMDIR" = xno ; then

  AC_MSG_ERROR(Install the coreutils package (rmdir is missing) !, 1)

fi

if test x"$TOUCH" = xno ; then

  AC_MSG_ERROR(Install the coreutils package (touch is missing) !, 1)

fi


# desktop-file-utils package:
AC_PATH_PROG([UPDATE_DESKTOP_DATABASE],  [update-desktop-database],   [no])
AC_PATH_PROG([DESKTOP_FILE_INSTALL],     [desktop-file-install],      [no])
# xdg-utils package:
AC_PATH_PROG([XDG_DESKTOP_MENU],         [xdg-desktop-menu],          [no])


if test x"$DESKTOP_FILE_INSTALL" = xno ; then
if test x"$XDG_DESKTOP_MENU" = xno ; then

  AC_MSG_WARN(Install either the xdg-utils or desktop-file-utils package !)

  AC_MSG_WARN(This is not absolutely required.)

  AC_MSG_WARN(But recommand to install desktop files.)

fi
fi

destop_file_utils=no

if test x"$DESKTOP_FILE_INSTALL" = xno ; then

  AC_MSG_NOTICE(The desktop-file-install tool is used to install the *.desktop file.)
  AC_MSG_NOTICE(The desktop-file-install tool is used to update the cache database)
  AC_MSG_NOTICE(of the MIME types handled by desktop files)

else

desktop_file_utils=yes

fi

xdg_utils=no

if test x"$XDG_DESKTOP_MENU" = xno ; then

  AC_MSG_NOTICE(The xdg-desktop-menu tool is used to install the *.desktop file.)
  AC_MSG_NOTICE(The xdg-desktop-menu tool is used to update the cache database)
  AC_MSG_NOTICE(of the MIME types handled by desktop files)

else

xdg_utils=yes

update_desktop_database=no ;

fi

if test x"$UPDATE_DESKTOP_DATABASE" = xno ; then

  AC_MSG_NOTICE(This desktop-file-install tool is used to update the cache database)
  AC_MSG_NOTICE(of the MIME types handled by desktop files)

fi

AM_CONDITIONAL(UPDATE_DESKTOP_DATABASE_UTILS, test x"$destop_file_utils" = "xyes")
AM_CONDITIONAL(DESTOP_FILE_UTILS, test x"$destop_file_utils" = "xyes")
AM_CONDITIONAL(XDG_UTILS,         test x"$xdg_utils" = "xyes")

# install-info check.
AC_PATH_PROG([INSTALL_INFO],         [install-info],          [no])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h libintl.h stdlib.h])

# Checks for libraries.
PKG_CHECK_MODULES([GTK], [gtk+-3.0], [ ], AC_MSG_ERROR(   gtk+3.0 library not detected !!!

   ******************** GTK 3 library required for looker *****************
   Install the GTK 3 library from your package manager or from source.

   WARNING: Take care to install the development files from gtk-3 !!!
            Often call: libgtk-3-dev or libgtk-3-devel.

   Then retry to launch the ./configure script...
   ************************************************************************
   , 1))

PKG_CHECK_MODULES([LIBVLC], [libvlc], [ ], AC_MSG_ERROR(   libvlc library not detected !!!

   ******************** libvlc library required for looker *****************
   Install the libvlc library from your package manager or from source.

   WARNING: Take care to install the development files from libvlc !!!
            Often call: libvlc-dev or libvlc-devel.

   Then retry to launch the ./configure script...
   ************************************************************************
   , 1))

PKG_CHECK_MODULES([VLC_PLUGIN], [vlc-plugin], [ ], AC_MSG_ERROR( vlc-plugin not detected !!!

   ****************** vlc-plugin library required for looker **************
   Install the vlc-plugin from your package manager or from source.

   The only required plugin required is this for the video output:
 
   vlc-plugin-video-output on my system or like.

   Then retry to launch the ./configure script...
   ************************************************************************
   , 1))


# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT64_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 data/Makefile
                 man/Makefile
                 texi/Makefile
                 desktop/Makefile
                 doc/Makefile
                 HTML/Makefile])
         
AC_MSG_NOTICE(System check OK !!!)

AC_OUTPUT

echo "
  ************************** ${PACKAGE_STRING} ************************

  ${PACKAGE_NAME} a video player which remember the position at exiting.

  Package dependencies: gtk+-3, libvlc, vlc-plugin.

  Host:     ${host}
  Compiler: ${CC} ${CFLAGS} ${CPPFLAGS}
  Prefix:   ${prefix}

  Author: Brüggemann Eddie <${PACKAGE_BUGREPORT}>.

  Now enter:

  $ make
  $ sudo make install

  To build and install ${PACKAGE_NAME}.

  You can launch the program with the commandline:
  $ looker [filepath]
  or from your desktop menu, the file manager: \"open with\" option works fine.
"
