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

	boxBody = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);// creer le Body
        gtk_container_add(GTK_CONTAINER (window), boxBody);//ajouter le Body a la fenetre
        boxNav = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);// creer la Nav 
        gtk_container_add(GTK_CONTAINER (boxBody), boxNav);//ajouter la Nav au Body
        boxMain = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);//creer le boxmain
        gtk_container_add(GTK_CONTAINER (boxBody), boxMain);// ajouter la boxmain au Body


	//box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	// button_box = gtk_button_box_new (GTK_ORIENTATION_VERTICAL); // Crétion d'une box en position horizontale
	//gtk_container_add (GTK_CONTAINER (window), box); // AJout de la box

	button = gtk_button_new_with_label ("Hello World"); //Ajouter du text au button
	g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);//lors du click active la print_hello
	g_signal_connect_swapped (button, "clicked", G_CALLBACK (print_hello), window);
	gtk_container_add (GTK_CONTAINER (boxNav), button);//ajouter le button a la Nav

	button2 = gtk_button_new_with_label ("Hello \n\n\n\nWorld");//Ajouter du text au button 2
	g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);//lors du click active la print_hello
	g_signal_connect_swapped (button2, "clicked", G_CALLBACK (print_hello), window);
	gtk_container_add (GTK_CONTAINER (boxNav), button2);//Ajouter le button 2 a la Nav

	gtk_widget_show_all (window);
}
