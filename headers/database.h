#ifndef __DATABASE_H__
#define __DATABASE_H__
    #include <mysql.h>

    int mysql_check_error();
    int database_connect(MYSQL *mysql);
    void displaySqlResult(MYSQL_RES *result);

#endif
