#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>
#include <mysql/mysql.h>

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

    if((item->key = malloc(strlen(key) * sizeof(char) + 1)) == NULL) {
        fprintf(stderr, "\nImpossible d'allouer la mémoire de la clé de l'item lors de l'ajout de l'item char.\n");
        free(item);
        return 0;
    }
    strcpy(item->key, key);

    if((item->value = malloc(strlen(value) * sizeof(char) + 1)) == NULL) {
        fprintf(stderr, "\nImpossible d'allouer la mémoire de la valeur lors de l'ajout de l'item char.\n");
        free(item->key);
        free(item);
        return 0;
    }
    strcpy(item->value, value);

    item->prev = NULL;
    *start = item;

    return 1;
}

void list_go_start_char_item(CHAR_ITEM **start) {
    CHAR_ITEM *item = *start;

    if(item = NULL) {
        if(config_check("debug", "true"))
            fprintf(stderr, "\nLa liste est NULL\n");
        return ;
    }

    while(item->prev != NULL) {
        item = item->prev;
    }

    *start = item;
}


int list_clean_char_item(CHAR_ITEM *start) {
    CHAR_ITEM *nextItem = NULL;

    if(start == NULL) {
        if(config_check("debug", "true"))
            fprintf(stderr,"\nAucun emplacement mémoire à libérer\n");
        return 1;
    }

    while(start != NULL) {
        if(start->value != NULL)
            free(start->value);
        if(start->key != NULL)
            free(start->key);

        nextItem = start->next;
        start->next = NULL;
        start->prev = NULL;

        if(start != NULL)
            free(start);

        start = nextItem;
    }

    return 1;
}

void list_show_char_item(CHAR_ITEM *start) {
    CHAR_ITEM *item = start;
    int i = 0;

    // printf("\n\nlist_show_char_item\n\n");
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