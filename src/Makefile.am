libexecdir = $(APPLET_LOCATION)

libexec_PROGRAMS = vinagre-applet

vinagre_applet_CFLAGS = $(GLIB2_CFLAGS) $(GTK3_CFLAGS) $(MATEPANEL_CFLAGS) $(XML2_CFLAGS)
vinagre_applet_LDADD = $(GLIB2_LIBS) $(GTK3_LIBS) $(MATEPANEL_LIBS) $(XML2_LIBS)

vinagre_applet_SOURCES = vinagre-applet.c vinagre-applet.h config.h
