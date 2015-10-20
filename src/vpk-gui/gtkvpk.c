/*Copyright © October 2015, Psykauze

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
The Software is provided “as is”, without warranty of any kind, express or implied, including but not limited to the warranties of merchantability, fitness for a particular purpose and noninfringement. In no event shall the authors or copyright holders X be liable for any claim, damages or other liability, whether in an action of contract, tort or otherwise, arising from, out of or in connection with the software or the use or other dealings in the Software.
Except as contained in this notice, the name of Psykauze shall not be used in advertising or otherwise to promote the sale, use or other dealings in this Software without prior written authorization from the <copyright holders>.*/

#include        <gtk/gtk.h>

static  void    sigh_print_selection    (GtkWidget      *gtklist,
                                          gpointer       func_data);

gint main (int argc, gchar *argv[])
{                                  
	GtkWidget       *window;
	GtkWidget	*vbox;
	GtkWidget	*scrolled_window;
	GtkWidget       *gtklist;
	GtkWidget       *list_item;
	GList           *dlist;
	gchar           buffer[64];



	/* initialise gtk (et donc gdk) */

	gtk_init(&argc, &argv);

	/* Création d'une fenêtre pour placer tous les widgets.
	 * Connexion de  gtk_main_quit() à l'événement "destroy" de
	 * la fenêtre afin de prendre en charge les événements « fermeture d'une
	 * fenêtre » du gestionnaire de fenêtre. */ 

	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	/* Titre de la fenêtre */
	gtk_window_set_title(GTK_WINDOW(window), "VPKextractGui by Psykauze");
	/* Evenement de destruction */
	gtk_signal_connect(GTK_OBJECT(window),
			"destroy",
			GTK_SIGNAL_FUNC(gtk_main_quit),
			NULL);

     /* À l'intérieur de la fenêtre, on a besoin d'une boîte pour placer 
      * verticalement les widgets. */
 
     vbox=gtk_vbox_new(FALSE, 5);
     gtk_container_border_width(GTK_CONTAINER(vbox), 5);
     gtk_container_add(GTK_CONTAINER(window), vbox);
     gtk_widget_show(vbox);
     /* Fenêtre à défilement pour placer le widget GtkList à l'intérieur. */
 
     scrolled_window=gtk_scrolled_window_new(NULL, NULL);
     gtk_widget_set_usize(scrolled_window, 250, 150);
     gtk_container_add(GTK_CONTAINER(vbox), scrolled_window);
     gtk_widget_show(scrolled_window);
 
     /* Création du widget GtkList
      * Connexion du gestionnaire de signal sigh_print_selection() au signal
      * "selection_changed" du GtkList pour afficher les items sélectionnés
      * à chaque fois que la sélection change. */
 
     gtklist=gtk_list_new();
     gtk_container_add(GTK_CONTAINER(scrolled_window), gtklist);
     gtk_widget_show(gtklist);
/*    gtk_signal_connect(GTK_OBJECT(gtklist),
                        "selection_changed",
                        GTK_SIGNAL_FUNC(sigh_print_selection),
                        NULL);
*/
         list_item=gtk_list_item_new_with_label("Test 1");
         dlist=g_list_prepend(dlist, list_item);
         gtk_widget_show(list_item);
         gtk_object_set_data(GTK_OBJECT(list_item),
                             list_item_data_key,
                             "Item avec label intégré");
     gtk_list_append_items(GTK_LIST(gtklist), dlist);

	/* Affichage de la fenêtre */
	gtk_widget_show(window);

	gtk_main();

	/* On arrive ici après que gtk_main_quit() ait été appelée lorsque
	 * la fenêtre principale a été détruite. */

}
