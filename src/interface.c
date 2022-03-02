// Interface functions
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <database.h>
#include <interface.h>
#include <global.h>

extern GLOBAL global;

GtkWidget *window;
//GtkLabel *mylabel;
GtkBuilder * builder;
GtkWidget *entry1;
GtkTextBuffer * buffer;
GtkWidget * view;
GtkTextMark * mark;
GtkTextIter iter;

void interface_init(int *argc,char ***argv) {
	
	gtk_init(argc, argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "files/glade/original.glade", NULL);

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
	

	g_signal_connect(G_OBJECT(window), "destroy", (GCallback) gtk_main_quit, NULL);
	

	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(builder);


	gtk_widget_show_all(window);
	gtk_main();
}

void button_clicked(){
	printf("Button clicked \n");
	DISPLAY_STRUCT displays;

	if(mysql_query(global.mysql, "SELECT * FROM history") != 0) {
        if(config_check("debug", "true")) {
             fprintf(stderr, "\nRequête impossible à executer\n");
			 return;
        }
	}

	if(global.result != NULL)
			mysql_free_result(global.result);

	global.result = mysql_store_result(global.mysql); // Store results
	

	displays.data = malloc(sizeof(char)*500);

	MYSQL_ROW row;
	unsigned int i = 1;
	unsigned int num_champs = 0;
	char display[2000];
	char temp[2000];
	char res[2000];
	num_champs = mysql_num_fields(global.result);

	while((row = mysql_fetch_row(global.result))) {
		//On déclare un pointeur long non signé pour y stocker la taille des valeurs
		unsigned long *lengths;
		strcpy(temp, "id : ");
		strcpy(displays.data, "id : ");
		//On stocke ces tailles dans le pointeur
		lengths = mysql_fetch_lengths(global.result);

		//On fait une boucle pour avoir la valeur de chaque champs
		for(i = 0; i < num_champs; i++) {
			//On ecrit toutes les valeurs
			//sprintf(display,"%.*s\n", (int) lengths[i], row[i] ? row[i] : "NULL");
			//printf("[%.*s] ", (int) lengths[i], row[i] ? row[i] : "NULL");
			sprintf(display,"%.*s", (int) lengths[i], row[i] ? row[i] : "NULL");
			strcat(temp, display);
			strcat(temp, "\n");
			
						
		}

		strcat(res, temp);
		strcat(res, "\n");
	}

	
	strcpy(displays.data, res); 
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "files/glade/History.glade", NULL);
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
	displays.display = GTK_WIDGET(gtk_builder_get_object(builder, "buffer_display"));
	view = gtk_text_view_new();
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(displays.display));
	mark = gtk_text_buffer_get_insert(buffer);
	gtk_text_buffer_get_iter_at_mark(buffer, &iter, mark);
	gtk_text_buffer_insert(buffer, &iter, displays.data, -1);

	
	// Libération du jeu de resultat
	mysql_free_result(global.result);
	
	g_signal_connect(G_OBJECT(window), "destroy", (GCallback) gtk_main_quit, NULL);
	

	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(builder);


	gtk_widget_show_all(window);
	gtk_main();

	free(displays.data);
}

void button_clicked2(){
	printf("Button clicked \n");
	DISPLAY_STRUCT displays;

	if(mysql_query(global.mysql, "SELECT * FROM history") != 0) {
        if(config_check("debug", "true")) {
             fprintf(stderr, "\nRequête impossible à executer\n");
			 return;
        }
	}

	if(global.result != NULL)
			mysql_free_result(global.result);

	global.result = mysql_store_result(global.mysql); // Store results
	

	displays.data = malloc(sizeof(char)*500);

	MYSQL_ROW row;
	unsigned int i = 1;
	unsigned int num_champs = 0;
	char display[2000];
	char temp[2000];
	char res[2000];
	num_champs = mysql_num_fields(global.result);

	while((row = mysql_fetch_row(global.result))) {
		//On déclare un pointeur long non signé pour y stocker la taille des valeurs
		unsigned long *lengths;
		strcpy(temp, "id : ");
		strcpy(displays.data, "id : ");
		//On stocke ces tailles dans le pointeur
		lengths = mysql_fetch_lengths(global.result);

		//On fait une boucle pour avoir la valeur de chaque champs
		for(i = 0; i < num_champs; i++) {
			//On ecrit toutes les valeurs
			//sprintf(display,"%.*s\n", (int) lengths[i], row[i] ? row[i] : "NULL");
			//printf("[%.*s] ", (int) lengths[i], row[i] ? row[i] : "NULL");
			sprintf(display,"%.*s", (int) lengths[i], row[i] ? row[i] : "NULL");
			strcat(temp, display);
			strcat(temp, "\n");
			
						
		}

		strcat(res, temp);
		strcat(res, "\n");
	}

	
	strcpy(displays.data, res); 
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "files/glade/History.glade", NULL);
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
	displays.display = GTK_WIDGET(gtk_builder_get_object(builder, "buffer_display"));
	view = gtk_text_view_new();
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(displays.display));
	mark = gtk_text_buffer_get_insert(buffer);
	gtk_text_buffer_get_iter_at_mark(buffer, &iter, mark);
	gtk_text_buffer_insert(buffer, &iter, displays.data, -1);

	
	// Libération du jeu de resultat
	mysql_free_result(global.result);
	
	g_signal_connect(G_OBJECT(window), "destroy", (GCallback) gtk_main_quit, NULL);
	

	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(builder);


	gtk_widget_show_all(window);
	gtk_main();

	free(displays.data);
}



