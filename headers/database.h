#ifndef __DATABASE_H__
#define __DATABASE_H__

    /*** LIBRARIES ***/
    // Imported libraries
    #include <mysql.h>

    /*** PROTOTYPES ***/
    int mysql_check_error();
    int database_connect(MYSQL *mysql);
    void displaySqlResult(MYSQL_RES *result);
    int add_target (char *name, char *url, char *ip, char *country, char *timeDisplay);
    int del_target();
    int select_target();
    void history_write (char *type, char *url, char *inputCommand);
    int history_result();

#endif
