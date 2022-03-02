#ifndef __DATABASE_H__
#define __DATABASE_H__
    #include <mysql.h>

    int mysql_check_error();
    int database_connect(MYSQL *mysql);
    void displaySqlResult(MYSQL_RES *result);
    int add_target (char *name, char *url, char *ip, char *country, char *timeDisplay);
    int del_target();
    int select_target();
    int history_result();
    void saveData (char *url, char *inputCommand);

#endif
