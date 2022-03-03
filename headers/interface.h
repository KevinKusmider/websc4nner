#ifndef __INTERFACE_H__
#define __INTERFACE_H__

    /*** LIBRARIES ***/
    // Imported Libraries
    #include <gtk/gtk.h>

    /*** STRUCTURES ***/
    typedef struct DISPLAY_STRUCT
    {

        char * data;

        GtkWidget * display;

    } DISPLAY_STRUCT;

    /*** PROTOTYPES ***/
    void print_hello (GtkWidget *widget, gpointer data);

    void activate (GtkApplication *app, gpointer user_data);
    void interface_init(int *argc,char ***argv);

#endif
