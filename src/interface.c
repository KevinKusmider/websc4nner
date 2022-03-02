// Interface functions
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <database.h>

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
	//mylabel = GTK_LABEL(gtk_builder_get_object(builder, "MyLabel"));
	//sqli = GTK_WIDGET(gtk_builder_get_object(builder, "sqli"));

	g_signal_connect(G_OBJECT(window), "destroy", (GCallback) gtk_main_quit, NULL);
	//g_signal_connect(G_OBJECT(sqli), "clicked", (GCallback) gtk_button_clicked, NULL);

	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(builder);


	gtk_widget_show_all(window);
	gtk_main();

	// typedef struct Display_struc
    // {

    //     char * data;
    //     GTK_WIDGET * display;
        

    // } Display_struc;
}

void button_clicked(){
	printf("Button clicked \n");

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "files/glade/History.glade", NULL);

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
	//mylabel = GTK_LABEL(gtk_builder_get_object(builder, "MyLabel"));
	
	g_signal_connect(G_OBJECT(window), "destroy", (GCallback) gtk_main_quit, NULL);
	

	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(builder);


	gtk_widget_show_all(window);
	gtk_main();
}

void show_database(){
	
}

// void display(){
// 	view = gtk_text_view_new();
// 	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
// 	mark = gtk_text_buffer_get_insert(buffer);
// 	gtk_entry_buffer_set_text ("ta mere");
    


// }

