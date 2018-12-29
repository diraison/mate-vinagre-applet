#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([vinagre-applet], [0.07], [Jean Diraison <jean.diraison@ac-rennes.fr>])
AM_INIT_AUTOMAKE([-Wall])
AC_CONFIG_SRCDIR([src/vinagre-applet.h])
AC_CONFIG_HEADERS([src/config.h:src/config.h.in])

# Checks for programs.
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_SED

# Checks for libraries.
PKG_CHECK_MODULES([GLIB2], [glib-2.0 >= 2.50])
PKG_CHECK_MODULES([GTK3], [gtk+-3.0])
PKG_CHECK_MODULES([MATEPANEL], [libmatepanelapplet-4.0 >= 1.16])
PKG_CHECK_MODULES([XML2], [libxml-2.0])

# Checks for header files.
AC_CHECK_HEADERS([stdio.h string.h libintl.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

# Applet installation directory
AC_ARG_ENABLE([libexecdir],
	      AS_HELP_STRING([--enable-libexecdir], [Install applet in ${libexecdir} directory (default is \${exec_prefix}/lib/mate-applets).]),
	      [APPLET_LOCATION="${libexecdir}"],
	      [APPLET_LOCATION="\${exec_prefix}/lib/mate-applets"])
AC_SUBST(APPLET_LOCATION)
AC_MSG_WARN([Applet will be installed in ${APPLET_LOCATION} directory.])

# Config files
AC_CONFIG_FILES([Makefile src/Makefile data/Makefile])

AC_OUTPUT
