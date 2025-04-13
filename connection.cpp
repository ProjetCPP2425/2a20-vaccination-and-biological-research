#include "connection.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("vacc2a20");//vacc2a20
db.setUserName("smartvacc");//smartvacc
db.setPassword("smartvacc");//smartvacc

if (db.open())
test=true;





    return  test;
}
