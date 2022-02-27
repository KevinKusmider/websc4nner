#ifndef __LISTE_H__
#define __LISTE_H__

    typedef struct CHAR_ITEM
    {

        char * value;
        char * key;
        struct CHAR_ITEM * next;
        struct CHAR_ITEM * prev;

    } CHAR_ITEM;


    typedef struct CHAR_ITEM_C
    {

        CHAR_ITEM * item;
        struct CHAR_ITEM_C * next;
        struct CHAR_ITEM_C * prev;

    } CHAR_ITEM_C;

    int list_add_char_item(CHAR_ITEM **start, char * key, char * value);
    void list_go_start_char_item(CHAR_ITEM **start);
    int list_clean_char_item(CHAR_ITEM *start);
    void list_show_char_item(CHAR_ITEM *start);

#endif