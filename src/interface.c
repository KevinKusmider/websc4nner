// Interface functions
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

void print_hello (GtkWidget *widget, gpointer data) {
  g_print ("Hello World\n");
}

void activate (GtkApplication *app, gpointer user_data) {
  	GtkWidget *window;
  	GtkWidget *button;
  	GtkWidget *button2;
  	GtkWidget *button_box;
  	GtkWidget *box;
  	GtkWidget *boxBody;
  	GtkWidget *boxNav;
  	GtkWidget *boxMain;

  	window = gtk_application_window_new (app); // Création d'une nouvelle fenêtre
  	gtk_window_set_title (GTK_WINDOW (window), "Window"); // Définir un nom a la fenetre
  	gtk_window_set_default_size (GTK_WINDOW (window), 1000, 800); // Définir la taille de la fenetre

	boxBody = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        gtk_container_add(GTK_CONTAINER (window), boxBody);
        boxNav = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_container_add(GTK_CONTAINER (boxBody), boxNav);
        boxMain = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_container_add(GTK_CONTAINER (boxBody), boxMain);


	//box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	// button_box = gtk_button_box_new (GTK_ORIENTATION_VERTICAL); // Crétion d'une box en position horizontale
	//gtk_container_add (GTK_CONTAINER (window), box); // AJout de la box

	button = gtk_button_new_with_label ("Hello World");
	g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
	g_signal_connect_swapped (button, "clicked", G_CALLBACK (print_hello), window);
	gtk_container_add (GTK_CONTAINER (boxNav), button);

	button2 = gtk_button_new_with_label ("Hello \n\n\n\nWorld");
	g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
	g_signal_connect_swapped (button2, "clicked", G_CALLBACK (print_hello), window);
	gtk_container_add (GTK_CONTAINER (boxNav), button2);

	gtk_widget_show_all (window);
}
