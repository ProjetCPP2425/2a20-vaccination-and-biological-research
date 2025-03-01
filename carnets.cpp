#include "carnets.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

Carnets::Carnets() {}

Carnets::Carnets(int Id_Carnet, QString Cin, QString Nom, QString Prenom, int Age,
                 QString Sexe, QString Num, float Poids, QDate Date_Rdv,
                 QString Remarques, QString Statut_Vaccinal)
{
    this->Id_Carnet = Id_Carnet;
    this->Cin = Cin;
    this->Nom = Nom;
    this->Prenom = Prenom;
    this->Age = Age;
    this->Sexe = Sexe;
    this->Num = Num;
    this->Poids = Poids;
    this->Date_Rdv = Date_Rdv;
    this->Remarques = Remarques;
    this->Statut_Vaccinal = Statut_Vaccinal;
}

bool Carnets::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO CARNETS (CIN, NOM, PRENOM, AGE, SEXE, NUM, POIDS, DATE_RDV, REMARQUES, STATUT_VACCINAL) "
                  "VALUES (:cin, :nom, :prenom, :age, :sexe, :num, :poids, :date_rdv, :remarques, :statut)");

    query.bindValue(":cin", Cin);
    query.bindValue(":nom", Nom);
    query.bindValue(":prenom", Prenom);
    query.bindValue(":age", Age);
    query.bindValue(":sexe", Sexe);
    query.bindValue(":num", Num);
    query.bindValue(":poids", Poids);
    query.bindValue(":date_rdv", Date_Rdv);
    query.bindValue(":remarques", Remarques);
    query.bindValue(":statut", Statut_Vaccinal);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout du carnet :" << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQueryModel* Carnets::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT CIN, NOM, PRENOM, AGE, SEXE, NUM, POIDS, DATE_RDV, REMARQUES, STATUT_VACCINAL FROM CARNETS");

    if (model->lastError().isValid()) {
        qDebug() << "Erreur lors de l'affichage des carnets :" << model->lastError().text();
        return nullptr;
    }

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom")); // Assure-toi que c'est bien la colonne 2
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Âge"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Sexe"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Poids"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date RDV"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Remarques"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Statut Vaccinal"));

    return model;
}
bool Carnets::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM CARNETS WHERE Id_Carnet= :id");
    query.bindValue(":id", id);
    return query.exec();  // Retourne true si la suppression réussit
}
