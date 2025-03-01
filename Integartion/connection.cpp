#include "connection.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlDatabase>
#include <QDebug>


Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
db = QSqlDatabase::addDatabase("QODBC");
    qputenv("QT_DEBUG_PLUGINS", QByteArray("1"));

db.setDatabaseName("vacc2a20");//inserer le nom de la source de données
db.setUserName("smartvacc");//inserer nom de l'utilisateur
db.setPassword("smartvacc");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;





    return  test;
}
