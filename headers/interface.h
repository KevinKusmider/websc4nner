#ifndef __INTERFACE_H__
#define __INTERFACE_H__

    void print_hello (GtkWidget *widget, gpointer data);

    void activate (GtkApplication *app, gpointer user_data);
    void interface_init(int *argc,char ***argv);

#endif
