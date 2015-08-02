/*
 * vinagre-applet.c
 *
 * $Id$
 *
 * This file is part of the mate vinagre applet project (c) 2015 distributed
 * under the GNU GPLv3 license and created by Jean Diraison
 * <jean.diraison@ac-rennes.fr> 
 */

/*
 *   gcc `pkg-config --cflags --libs libmatepanelapplet-4.0 libxml-2.0` -o vinagre-applet vinagre-applet.c
 *
 */


#include <string.h>
#include <stdio.h>
#include <libintl.h>

#include <mate-panel-applet.h>
#include <mate-panel-applet-gsettings.h>
#include <gtk/gtklabel.h>

#include <glib/gi18n.h>

#include <libxml/tree.h>
#include <libxml/parser.h>


#include "vinagre-applet.h"


/**
 * Vinagre callback launcher.
 *
 * @param item The menu item selected
 * @param url The url to pass to vinagre in command line
 */
static void
spawn_vinagre_cb (GtkMenuItem *item,
		 gpointer *url)
{
	GError *err = NULL;
	gchar *cmd;

	cmd = g_strdup_printf ((url != NULL) ? "%s %s" : "%s", "/usr/bin/vinagre", (gchar *) url);

	if (!g_spawn_command_line_async (cmd, &err))
		g_error_free (err);

	g_free (cmd);
}


/**
 * Clear bookmark_item table, free allocated strings then nullify pointers.
 *
 * @param bi The bookmark item to clear
 */
static void
clear_bookmark_item (bookmark_item bi)
{
	int i;

	for (i = 0; i < BI_LENGTH; i++) {
		if (bi[i] != NULL) {
			g_free (bi[i]);
			bi[i] = NULL;
		}
	}
}

/**
 * Get usefull content from bookmark item (name, host, port, protocol).
 *
 * @param parent The running xmlNode pointer
 * @param bi The bookmark item table to fill with
 * @param name The name of the current parsed item, to add to main menu
 * @param url The url built with item content, to pass to command launcher
 */
static void
get_bookmark_item (xmlNodePtr parent,
		   bookmark_item bi,
		   gchar **name,
		   gchar **url)
{
	xmlNodePtr node;

	*name = NULL;
	*url = NULL;

	if (parent == NULL || strcmp("item", (char *)parent->name) != 0)
		return;

	for (node = parent->children; node != NULL; node = node->next) {
		if (node->children == NULL || node->children->type != XML_TEXT_NODE)
			continue;
		if (strcmp("protocol", (char *)node->name) == 0)
			bi[BI_PROTOCOL] = g_strdup ((gchar *)node->children->content);
		if (strcmp("name",     (char *)node->name) == 0)
			bi[BI_NAME]     = g_strdup ((gchar *)node->children->content);
		if (strcmp("host",     (char *)node->name) == 0)
			bi[BI_HOST]     = g_strdup ((gchar *)node->children->content);
		if (strcmp("username", (char *)node->name) == 0)
			bi[BI_USERNAME] = g_strdup ((gchar *)node->children->content);
		if (strcmp("port",     (char *)node->name) == 0)
			bi[BI_PORT]     = g_strdup ((gchar *)node->children->content);
	}
	if (bi[BI_PROTOCOL] == NULL || bi[BI_HOST] == NULL || bi[BI_PORT] == NULL)
		return;

	*name = g_strdup(bi[BI_NAME]);
	
	if (bi[BI_USERNAME] != NULL) {
		*url = g_strdup_printf ("%s://%s@%s::%s", bi[BI_PROTOCOL], bi[BI_USERNAME], bi[BI_HOST], bi[BI_PORT]);
	} else {
		*url = g_strdup_printf ("%s://%s::%s", bi[BI_PROTOCOL], bi[BI_HOST], bi[BI_PORT]);
	}
}


/**
 * Complete menu with vinagre xml bookmarks content.
 *
 * @param menu The menu to complete
 */
static void
add_bookmarks_to_main_menu (GtkWidget *menu)
{
	static gchar *bookmarks_filename = NULL;

	if (bookmarks_filename == NULL)
		bookmarks_filename = g_build_filename (g_get_user_data_dir (), "vinagre", "vinagre-bookmarks.xml", NULL);

	xmlDocPtr doc = xmlReadFile (bookmarks_filename, "UTF-8", 0);
	if (doc == NULL)
		return;
	
	xmlNodePtr root = xmlDocGetRootElement (doc);
	if (root == NULL) {
		xmlFreeDoc(doc);
		xmlCleanupParser();
		return;
	}

	bookmark_item bi = { NULL, NULL, NULL, NULL, NULL };

	xmlNodePtr node;

	for (node = root->children; node != NULL; node = node->next) {
		gchar *name, *url;
		get_bookmark_item (node, bi, &name, &url);
		GtkWidget *item, *image;
		if ( g_strcmp0 (bi[BI_PROTOCOL], "ssh") == 0 ) {
			image = gtk_image_new_from_icon_name ("utilities-terminal", GTK_ICON_SIZE_MENU);
		} else {
			image = gtk_image_new_from_icon_name ("application-x-vnc", GTK_ICON_SIZE_MENU);
		}
		item = gtk_image_menu_item_new_with_label (name);
		gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (item), image);
		gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
		g_signal_connect (item, "activate", G_CALLBACK (spawn_vinagre_cb), (gpointer *) url);
		clear_bookmark_item (bi);
	}

	xmlFreeDoc(doc);
	xmlCleanupParser();
}


/**
 * Complete menu with vinagre xml bookmarks content.
 *
 * @param applet The applet
 * @param event  The event to get button pressed and time
 */
static void
setup_main_menu (GtkWidget *applet,
		      GdkEventButton *event)
{
	GtkWidget *menu, *item, *image;

	menu = gtk_menu_new ();

	image = gtk_image_new_from_icon_name ("preferences-desktop-remote-desktop", GTK_ICON_SIZE_MENU);
	item = gtk_image_menu_item_new_with_label (_("Remote Desktop Viewer"));
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (item), image);
	gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
	g_signal_connect (item, "activate", G_CALLBACK (spawn_vinagre_cb), NULL);

	item = gtk_separator_menu_item_new ();
	gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);

	add_bookmarks_to_main_menu (menu);

	gtk_widget_show_all (menu);
	gtk_menu_popup (GTK_MENU (menu), NULL, NULL, NULL, applet, event->button, event->time);
}


/**
 * Click callback checks button pressed to create main menu if required.
 *
 * @param applet_window The applet parent window
 * @param event The event received, used to test pressed button
 * @param user_data
 *
 * @return gboolean If left mouse button pressed the adds main menu and returns true.
 */
static gboolean
click_mouse_cb (GtkWidget *applet_window,
	  GdkEventButton *event,
	  gpointer user_data)
{
	if ((event->type != GDK_BUTTON_PRESS) || (event->button != 1))
		return FALSE;

	setup_main_menu (applet_window, event);
	return TRUE;
}


/**
 * About item callback from default menu.
 *
 * @param action The action
 * @param applet_window The applet parent window
 */
static void
about_menu_cb (GtkAction *action,
		  GtkWidget *applet_window)
{
	static const char *authors[] = { PACKAGE_BUGREPORT, NULL };

	gtk_show_about_dialog (GTK_WINDOW(applet_window),
			       "version", VERSION,
			       "copyright", "Copyright © 2015 Jean Diraison",
			       "authors", authors,
			       "comments", _("Vinagre shows remote Windows, Mac OS X and Linux desktops."),
			       "logo-icon-name", "application-x-remote-connection",
			       NULL);
}


/**
 * Add a default menu (left mouse button) with About item to applet.
 *
 * @param applet The mate panel applet pointer
 */
void
setup_default_applet_menu (MatePanelApplet *applet)
{
	static GtkActionGroup *action_group = NULL;
	static const gchar *xml = "<menuitem name='Item 1' action='About' />";
	static const GtkActionEntry applet_menu_actions [] = {
		{ "About", GTK_STOCK_ABOUT, N_("About this application"), NULL, NULL, G_CALLBACK (about_menu_cb) }
	};

	if (action_group == NULL) {
		action_group = gtk_action_group_new ("Vinagre Applet Actions");
		gtk_action_group_set_translation_domain (action_group, GETTEXT_VINAGRE_PACKAGE);
		gtk_action_group_add_actions (action_group, applet_menu_actions, G_N_ELEMENTS (applet_menu_actions), applet);
		mate_panel_applet_setup_menu (applet, xml, action_group);
	}
}


/**
 * The applet factory init function, creates an icon in mate panel and adds a
 * default menu for user interaction.
 *
 * @return gboolean Returns true if applet can be added, false otherwise
 */
static gboolean 
applet_factory_fill (MatePanelApplet *applet,
		     const char  *iid,
		     gpointer data)
{
	static gboolean registered = FALSE;
 	GtkWidget *image;

	if (strcmp (iid, "VinagreApplet") || registered)
		return FALSE;

	/* set mate-panel applet options */
	mate_panel_applet_set_flags (applet, MATE_PANEL_APPLET_EXPAND_MINOR);
	mate_panel_applet_set_background_widget (applet, GTK_WIDGET (applet));

	/* localization */
	bindtextdomain (GETTEXT_VINAGRE_PACKAGE, LOCALE_DIR_VINAGRE_PACKAGE);
	bind_textdomain_codeset (GETTEXT_VINAGRE_PACKAGE, "UTF-8");
	textdomain (GETTEXT_VINAGRE_PACKAGE);

	/* add icon and menu to panel */
	image = gtk_image_new_from_icon_name ("application-x-vnc", GTK_ICON_SIZE_LARGE_TOOLBAR);
	if (image == NULL)
		return FALSE;
	gtk_container_add (GTK_CONTAINER (applet), image);
	g_signal_connect (applet, "button-press-event", G_CALLBACK (click_mouse_cb), NULL);
	gtk_widget_show_all (GTK_WIDGET (applet));

	setup_default_applet_menu (applet);

	registered = TRUE;

	return TRUE;
}


MATE_PANEL_APPLET_OUT_PROCESS_FACTORY("VinagreAppletFactory", PANEL_TYPE_APPLET, "Vinagre Applet", applet_factory_fill, NULL);


