#ifndef __GLOBAL_H__
#define __GLOBAL_H__

    /*** LIBRARIES ***/
    // Imported libraries
    #include <mysql/mysql.h>

    // Own libraries
    #include <liste.h>

    /*** STRUCTURES ***/
    typedef struct GLOBAL
    {

        CHAR_ITEM *configs;
        char * url;
        MYSQL *mysql;
        MYSQL_RES *result;
        MYSQL_STMT *stmt;

    } GLOBAL;

    /*** PROTOTYPES ***/
    int init_global();

    // FICHER CONFIG
    int init_config();
    char * config(char * key);
    int config_check(char * key, char * value);

    // MYSQL
    int init_mysql();

    // CLEANING
    int global_clean();

#endif