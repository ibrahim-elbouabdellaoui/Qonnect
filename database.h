#ifndef DATABASE_H
#define DATABASE_H
#include <QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QtSql/QSqlError>
#include<QtSql/QSqlDriver>
#include<QDebug>

class DataBase
{
    QSqlDatabase db ;
public:
    DataBase();
    void seedDatabase();

};

#endif  // DATABASE_H
