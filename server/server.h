#ifndef SERVER_H
#define SERVER_H
#include <mysql/mysql.h>
extern MYSQL *conn;
void extract_error(MYSQL *conn);
MYSQL_RES *make_query(char query[]);
#endif