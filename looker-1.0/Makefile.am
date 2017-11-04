SUBDIRS = . src doc data man texi desktop HTML

nobase_pkgdata_DATA = ./src/main.c               \
                      ./src/player_callbacks.c   \
                      ./src/error_message.h      \
                      ./src/error_message.c      \
                      ./src/global_vars.h        \
                      ./src/configuration.h      \
                      ./src/headers/includes.h   \
                      ./src/headers/defines.h    \
                      ./src/GtkSmartMenuItem.c   \
                      ./src/global_vars.c        \
                      ./src/configuration.c      \
                      ./src/video_callbacks.h    \
                      ./src/video_callbacks.c    \
                      ./src/player_callbacks.h   \
                      ./src/GtkSmartMenuItem.h   \
                      ./Makefile.mk


EXTRA_DIST =  ./Makefile.mk \
              ./user_info.sh \
              ./WHOAMI \
              ./HACKME

install-data-hook:
	$(MV) "$(DESTDIR)$(pkgdatadir)/Makefile.mk" "$(DESTDIR)$(pkgdatadir)/Makefile"
	$(MKDIR_P) "$(DESTDIR)$(pkgdatadir)/bin"
	$(CHMOD) -R a+rwx "$(DESTDIR)$(pkgdatadir)/src"
	$(CHMOD) -R a+rwx "$(DESTDIR)$(pkgdatadir)/bin"
	if `test ! -d "$(DESTDIR)$(pkgdatadir)/config"` ; then $(MKDIR_P) "$(DESTDIR)$(pkgdatadir)/config" ; fi
	$(CHMOD) a+rwx "$(DESTDIR)$(pkgdatadir)/config"
	if `test ! -d "$(DESTDIR)$(pkgdatadir)/bookmark"` ; then $(MKDIR_P) "$(DESTDIR)$(pkgdatadir)/bookmark" ; fi
	$(CHMOD) a+rwx "$(DESTDIR)$(pkgdatadir)/bookmark"
