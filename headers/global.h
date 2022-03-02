#ifndef __GLOBAL_H__
#define __GLOBAL_H__

    // Imported libraries
    #include <mysql/mysql.h>

    // Own libraries
    #include <liste.h>

    typedef struct GLOBAL
    {

        CHAR_ITEM *configs;
        char * url;
        MYSQL *mysql;
        MYSQL_RES *result;
        MYSQL_STMT *stmt;

    } GLOBAL;

    int init_global();

    // FICHER CONFIG
    int init_config();
    char * config(char * key);
    int config_check(char * key, char * value);

    // MYSQL
    int init_mysql();

#endif