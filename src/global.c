/*** LIBRARIES ***/
// GCC Standards
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Imported libraries
#include <curl/curl.h>
#include <mysql/mysql.h>

// Own libraries
#include <global.h>
#include <database.h>


GLOBAL global;


int init_global() {

    global.url = NULL;//https://www.zixem.altervista.org/SQLi/level1.php?id=%27" ; //"https://www.nootica.fr/customer/account/login/";

    if(!init_config())
        fprintf(stderr, "\nProblème lors de l'initialisation du fichier config dans global\n");

    if(!init_mysql())
        fprintf(stderr, "\nProblème lors de l'initialisation de mysql dans global\n");

    return 1;
}

/************************
 * CONFIG FILE
*************************/

/**
 * @brief Transforme les données du fichier config en liste chainee dans la variable global
 * 
 * @return int 
 */
int init_config() {
    FILE *config;
    char fKey[55], *fValue;
    fValue = malloc(55 * sizeof(char));
    
    if(fValue == NULL) {
        fprintf(stderr, "\nImpossible de d'allouer la mémoire dans la fonction config\n");
        return 0;
    }    

    if((config = fopen("config.txt", "r")) == NULL) {
        fprintf(stderr, "\nImpossible de lire le fichier de configuraiton.\n");
        return 0;
    }

    while(fscanf(config, "%s %s", fKey, fValue) != EOF) {
        list_add_char_item(&global.configs, fKey, fValue);
    }

    fclose(config);

    return 1;
}

/**
 * @brief Récupérer la valeur dans le fichier config de la clé passée en paramètre. 
 * 
 * @param key char pointer clé à rechercher
 * @return char* 
 */
char * config(char * key) {
    CHAR_ITEM * item = global.configs;

    if(item == NULL) {
        fprintf(stderr, "\nAucun element dans la liste des configs\n");
        return NULL;
    }
    
    while(item != NULL) {
        if(item->key != NULL && !strcmp(item->key, key)) {
            return item->value;
        }

        item = item->next;
    }

    return NULL;
}

/**
 * @brief Vérifie dans le fichier config si la valeur d'une certaine clé est la même que celle passée en paramètre
 * 
 * @param key 
 * @param value 
 * @return int 
 */
int config_check(char * key, char * value) {
    char * fileValue = config(key);

    if(fileValue == NULL) {
        return 0;
    }

    if(!strcmp(fileValue, value)) {
        return 1;
    } else {
        return 0;
    }
}

/************************
 * MYSQL LIB
*************************/

/**
 * @brief Initliser le pointeur MYSQL global et la connexion a la bdd
 * 
 * @return int 
 */
int init_mysql() {
    global.mysql = mysql_init(NULL);
    if(global.mysql == NULL) {
        if(config_check("debug", "true"))
            fprintf(stderr, "\nImpossible d'initialiser mysql dans global\n");
        return 0;
    }

    if(!database_connect(global.mysql)) {
        mysql_check_error();
        if(config_check("debug", "true"))
            fprintf(stderr, "\nProblèmes lors de la connexion à la bdd\n");
        return 0;
    }

	global.stmt = mysql_stmt_init(global.mysql);
    if(global.stmt == NULL) {
        if(config_check("debug", "true"))
            fprintf(stderr, "\nImpossible d'initialiser stmt dans global\n");
        return 0;
    }

    global.result = NULL;

    return 1;
}



/************************
 * CLEANING
*************************/

/**
 * @brief Libérer tous les éléments de la structure global
 * 
 * @return int 
 */
int global_clean() {
    if(global.configs != NULL)
        list_clean_char_item(global.configs);

    if(global.url != NULL)
        free(global.url);

    if(global.mysql != NULL)
        mysql_close(global.mysql);

    if(global.result != NULL)
        mysql_free_result(global.result);

    if(global.stmt)
        mysql_stmt_close(global.stmt);

    return 1;
}