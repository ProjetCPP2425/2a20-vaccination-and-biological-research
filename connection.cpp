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
db.setDatabaseName("vacc2a20");//inserer le nom de la source de données
db.setUserName("smartvacc");//inserer nom de l'utilisateur
db.setPassword("smartvacc");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;





    return  test;
}
