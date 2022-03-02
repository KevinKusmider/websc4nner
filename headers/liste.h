#ifndef __LISTE_H__
#define __LISTE_H__

    typedef struct CHAR_ITEM
    {

        char * value;
        char * key;
        struct CHAR_ITEM * next;
        struct CHAR_ITEM * prev;

    } CHAR_ITEM;

    typedef struct INPUT_ITEM 
    {
        char * tag;
        CHAR_ITEM * attrs;
        struct INPUT_ITEM * next;
        struct INPUT_ITEM * prev;

    } INPUT_ITEM;


    typedef struct FORM_ITEM
    {
        char * tag;
        CHAR_ITEM * attrs;
        INPUT_ITEM * inputs;
        struct FORM_ITEM * next;
        struct FORM_ITEM * prev;

    } FORM_ITEM;
    
    // CHAR ITEMS
    int list_add_char_item(CHAR_ITEM **start, char * key, char * value);
    void list_go_start_char_item(CHAR_ITEM **start);
    int list_clean_char_item(CHAR_ITEM *start);
    void list_show_char_item(CHAR_ITEM *start);

    // DOM ELEMENTS
    int list_count_form_item(FORM_ITEM * forms);
    void list_go_start_form_item(FORM_ITEM **start);
    int list_clean_input_item(INPUT_ITEM *start);
    int list_clean_form_item(FORM_ITEM *start);

    // DISPLAY FORM
    void list_show_forms(FORM_ITEM * start);
    void list_show_forms_char_items(CHAR_ITEM * attrs);
    void list_show_form_inputs(INPUT_ITEM *inputs);

#endif