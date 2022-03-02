#ifndef __INTERFACE_H__
#define __INTERFACE_H__
    #include <gtk/gtk.h>

    typedef struct DISPLAY_STRUCT
    {

        char * data;

        GtkWidget * display;

    } DISPLAY_STRUCT;

    void print_hello (GtkWidget *widget, gpointer data);

    void activate (GtkApplication *app, gpointer user_data);
    void interface_init(int *argc,char ***argv);

#endif
