appletdir        = $(datadir)/mate-panel/applets
applet_in_file   = org.mate.panel.VinagreApplet.mate-panel-applet.in
applet_DATA      = $(applet_in_file:.mate-panel-applet.in=.mate-panel-applet)

servicedir       = $(datadir)/dbus-1/services
service_in_file  = org.mate.panel.applet.VinagreAppletFactory.service.in
service_DATA     = $(service_in_file:.service.in=.service)

$(applet_DATA): $(applet_in_file) Makefile
	$(AM_V_GEN)$(SED) -e "s#\@APPLET_LOCATION\@#$(APPLET_LOCATION)#" $< >$@

$(service_DATA): $(service_in_file) Makefile
	$(AM_V_GEN)$(SED) -e "s#\@APPLET_LOCATION\@#$(APPLET_LOCATION)#" $< >$@

all:	$(applet_DATA) $(service_DATA)

CLEANFILES = $(applet_DATA) $(service_DATA)

EXTRA_DIST = $(applet_in_file) $(service_in_file)
