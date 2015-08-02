/*
 * vinagre-applet.h
 *
 * $Id$
 *
 * This file is part of the mate vinagre applet project (c) 2015 distributed
 * under the GNU GPLv3 license and created by Jean Diraison
 * <jean.diraison@ac-rennes.fr> 
 */

#ifndef VINAGRE_APPLET_H_
#define VINAGRE_APPLET_H_ 

#include <glib.h>

#include "config.h"

#ifndef PACKAGE_BUGREPORT
#define PACKAGE_BUGREPORT "Jean Diraison <jean.diraison@ac-rennes.fr>"
#endif

#ifndef VERSION
#define VERSION "0.06"
#endif

#define	GETTEXT_VINAGRE_PACKAGE		"vinagre"
#define LOCALE_DIR_VINAGRE_PACKAGE	"/usr/share/locale"

/*
 *  vinagre bookmark file :    ~/.local/share/vinagre/vinagre-bookmarks.xml
 *
 *  xml format :     <vinagre-bookmarks>
 *                     <item>
 *                       <protocol>...</protocol>
 *                       <name>...</name>
 *                       <host>...</host>
 *                       <port>...</port>
 *                     </item>
 *                     <item>......</item>
 *                   </vinagre-bookmarks>
 *
 *  command line :   vinagre vnc://hostname::5900
 *                   vinagre ssh://admin@hostname
 *                   vinagre protocol://[username@]host[::port]
 *
 */

enum bookmark_item_parameters {
	BI_PROTOCOL	= 0,
	BI_NAME		= 1,
	BI_HOST		= 2,
	BI_USERNAME	= 3,
	BI_PORT		= 4,
	BI_LENGTH	= 5
};

typedef gchar *bookmark_item[BI_LENGTH];

#endif

