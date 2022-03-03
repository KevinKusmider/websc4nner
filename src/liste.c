/*** LIBRARIES ***/
// GCC Standars
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Imported Libraries
#include <curl/curl.h>
#include <mysql/mysql.h>

// Own Libraries
#include <liste.h>
#include <global.h>


int list_add_char_item(CHAR_ITEM **start, char * key, char * value) {
    CHAR_ITEM *item;

    if((item = malloc(sizeof(CHAR_ITEM))) == NULL) {
        fprintf(stderr, "\nImpossible d'allouer la mémoire de l'item lors de l'ajout de l'item char dans une liste.\n");
        return 0;
    }

    if(*start != NULL ) {
        (*start)->prev = item;
        item->next = *start;
    } else {
        item->next = NULL;
    }

    if(key != NULL && strlen(key) != 0) {
        if((item->key = malloc(strlen(key) * sizeof(char) + 1)) == NULL) {
            fprintf(stderr, "\nImpossible d'allouer la mémoire de la clé de l'item lors de l'ajout de l'item char.\n");
            free(item);
            return 0;
        }

        strcpy(item->key, key);
    } else {
        item->key = NULL;
    }

    if(value != NULL && strlen(value) != 0) {
        if((item->value = malloc(strlen(value) * sizeof(char) + 1)) == NULL) {
            fprintf(stderr, "\nImpossible d'allouer la mémoire de la valeur lors de l'ajout de l'item char.\n");
            if(item->key != NULL)
                free(item->key);
            free(item);
            return 0;
        }
        strcpy(item->value, value);
    } else {
        item->value = NULL;
    }

    item->prev = NULL;
    *start = item;

    return 1;
}

int list_clean_char_item(CHAR_ITEM *start) {
    CHAR_ITEM *nextItem = NULL;

    if(start == NULL) {
        if(config_check("debug", "true"))
            fprintf(stderr,"\nAucun emplacement mémoire à libérer\n");
        return 1;
    }

    while(start != NULL) {
        if(start->value != NULL) {
            free(start->value);
        }

        if(start->key != NULL) {
            free(start->key);
        }

        nextItem = start->next;
        start->next = NULL;
        start->prev = NULL;

        if(start != NULL) {
            free(start);
        }

        start = nextItem;
    }

    return 1;
}

void list_show_char_item(CHAR_ITEM *start) {
    CHAR_ITEM *item = start;
    int i = 0;

    while(item != NULL) {
        printf("\nCHAR_ITEM n %d\n",i);
        printf(" address : %p\n", item);
        printf(" value : %s\n", item->value);
        printf(" key : %s\n", item->key);
        printf(" next : %p\n", item->next);
        printf(" prev : %p\n", item->prev);

        item = item->next;
        i++;
    }
}



// DOM ELEMENTS


int list_count_form_item(FORM_ITEM * forms) {
    FORM_ITEM *form = forms;
    int i = 0;

    while(form != NULL) {
        i++;
        form = form->next;
    }

    return i;
}

void list_go_start_form_item(FORM_ITEM **start) {
    FORM_ITEM *item = *start;

    if(item == NULL) {
        if(config_check("debug", "true"))
            fprintf(stderr, "\nLa liste est NULL\n");
        return ;
    }

    while(item->prev != NULL) {
        item = item->prev;
    }

    *start = item;
}

int list_clean_input_item(INPUT_ITEM *start) {
    INPUT_ITEM *nextItem = NULL;

    if(start == NULL) {
        if(config_check("debug", "true"))
            fprintf(stderr,"\nAucun emplacement mémoire à libérer\n");
        return 1;
    }

    while(start != NULL) {
        if(start->tag != NULL)
            free(start->tag);
        if(start->attrs != NULL)
            list_clean_char_item(start->attrs);

        nextItem = start->next;
        start->next = NULL;
        start->prev = NULL;

        if(start != NULL)
            free(start);

        start = nextItem;
    }

    return 1;
}

int list_clean_form_item(FORM_ITEM *start) {
    FORM_ITEM  *nextItem = NULL;

    if(start == NULL) {
        if(config_check("debug", "true"))
            fprintf(stderr,"\nAucun emplacement mémoire à libérer\n");
        return 1;
    }

    while(start != NULL) {
        if(start->tag != NULL)
            free(start->tag);
        if(start->attrs != NULL)
            list_clean_char_item(start->attrs);
        if(start->inputs != NULL)
            list_clean_input_item(start->inputs);

        nextItem = start->next;
        start->next = NULL;
        start->prev = NULL;

        if(start != NULL)
            free(start);

        start = nextItem;
    }

    return 1;
}


void list_show_forms(FORM_ITEM * start) {
    FORM_ITEM *form = start;
    int i = 1;

    while(form != NULL) {
        printf("\n  +------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
        printf("  | %-6s %-141d |\n", "FORM n", i);
        printf("  +------------------------------------------------------------------------------------------------------------------------------------------------------+\n");

        printf("  | | TAG : %-139s  |\n", form->tag);
        printf("  | |%-148s|\n", "");
        printf("  | | ATTRS : %-138s |\n", "");
        list_show_forms_char_items(form->attrs);
        printf("  | |%-148s|\n", "");
        printf("  | | INPUTS : %-137s |\n", "");
        list_show_form_inputs(form->inputs);
        printf("  +------------------------------------------------------------------------------------------------------------------------------------------------------+\n");

        form = form->next;
        i++;
    }
}

void list_show_forms_char_items(CHAR_ITEM *attrs) {
    CHAR_ITEM *attr = attrs;

    while(attr != NULL) {
        printf("  | | |%-146s|\n", "");
        printf("  | | | Attribute : %-132s |\n", attr->key);
        printf("  | | | Value : %-136s |\n", attr->value);

        attr = attr->next;
    }
}

void list_show_form_inputs(INPUT_ITEM *inputs) {
    INPUT_ITEM *input = inputs;

    while(input != NULL) {
        printf("  | | |%-146s|\n", "");
        printf("  | | | Tag : %-138s |\n", input->tag);

        input = input->next;
    }
}