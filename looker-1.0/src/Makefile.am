AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = looker

looker_SOURCES =  ./main.c                \
                  ./player_callbacks.c    \
                  ./error_message.c       \
                  ./GtkSmartMenuItem.c    \
                  ./global_vars.c         \
                  ./configuration.c       \
                  ./video_callbacks.c
                 
EXTRA_DIST = ./*.h \
             ./headers/*.h
            
looker_CPPFLAGS = -DPREFIX=\"$(DESTDIR)$(prefix)\"

looker_CFLAGS = -Wno-unused-parameter -Wno-missing-field-initializers -Wno-unused-function $(GTK_CFLAGS) $(LIBVLC_CFLAGS) $(VLC_PLUGIN_CFLAGS)

looker_LDADD = $(GTK_LIBS) $(LIBVLC_LIBS) $(VLC_PLUGIN_LIBS)

install-exec-hook:
	if `test ! -d $(DESTDIR)$(pkgdatadir)/hashing` ; then $(MKDIR_P) "$(DESTDIR)$(pkgdatadir)/hashing" ; fi
if HAVE_MD5
	echo `$(MD5SUM) $(DESTDIR)$(bindir)/$(PACKAGE)$(EXEEXT)` > $(pkgdatadir)/hashing/$(PACKAGE).md5
endif
if HAVE_SHA1
	echo `$(SHA1SUM) $(DESTDIR)$(bindir)/$(PACKAGE)$(EXEEXT)` > $(pkgdatadir)/hashing/$(PACKAGE).sha1
endif
if HAVE_SHA256
	echo `$(SHA256SUM) $(DESTDIR)$(bindir)/$(PACKAGE)$(EXEEXT)` > $(pkgdatadir)/hashing/$(PACKAGE).sha256
endif
if HAVE_SHA512
	echo `$(SHA512SUM) $(DESTDIR)$(bindir)/$(PACKAGE)$(EXEEXT)` > $(pkgdatadir)/hashing/$(PACKAGE).sha512
endif


