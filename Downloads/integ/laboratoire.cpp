#include "Laboratoire.h"
#include <QSqlQuery>
#include <QSqlTableModel>  // Changed to QSqlTableModel
#include <QSqlError>
#include <QDebug>

Laboratoire::Laboratoire() {}

Laboratoire::Laboratoire(int id, QString nom, QString adresse, QString type, QString responsable,
                         float depense, int nb_projets, QString statut, int matriels, int personnel, QDate date_creation)
{
    this->Id_laboratoire = id;
    this->Nom_Lab = nom;
    this->Adresse = adresse;
    this->Type = type;
    this->Responsable = responsable;
    this->Dépense = depense;
    this->Nb_Projets = nb_projets;
    this->Statut = statut;
    this->Matriels = matriels;
    this->Personnel = personnel;
    this->Date_creation = date_creation;
}

bool Laboratoire::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO SMARTVACC.LABORATOIRES (NOM_LAB, ADRESSE, RESPONSABLE, TYPE, STATUT, "
                  "NB_PROJETS, MATRIELS, PERSONNEL, DEPONSE, DATE_CREATION) "
                  "VALUES (:nom, :adresse, :responsable, :type, :statut, :nb_projets, :matriels, :personnel, :depense, :date_creation)");

    query.bindValue(":nom", getNomLab());
    query.bindValue(":adresse", getAdresse());
    query.bindValue(":responsable", getResponsable());
    query.bindValue(":type", getType());
    query.bindValue(":statut", getStatut());
    query.bindValue(":nb_projets", getNbProjets());
    query.bindValue(":matriels", getMatriels());
    query.bindValue(":personnel", getPersonnel());
    query.bindValue(":depense", getDepense());
    query.bindValue(":date_creation", getDateCreation());

    if (!query.exec()) {
        qDebug() << "Error de l'ajout d'un laboratoire : " << query.lastError().text();
        return false;
    }
    return true;
}

QSqlTableModel* Laboratoire::afficher()
{
    QSqlTableModel *model = new QSqlTableModel();
    model->setTable("SMARTVACC.LABORATOIRES");
    model->select();

    if (model->lastError().isValid()) {
        qDebug() << "Error fetching laboratories: " << model->lastError().text();
        return nullptr;
    }

    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Responsable"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Nb Projets"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Matériels"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Personnel"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Dépense"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("Date de création"));




    return model;
}


bool Laboratoire::supprimer(const QString nom)
{
    QSqlQuery query;
    query.prepare("DELETE FROM SMARTVACC.LABORATOIRES WHERE NOM_LAB = :nom ");
    query.bindValue(":nom", nom);

    qDebug() << "Deleting laboratoire with NOM_LAB:" << nom;

    if (!query.exec()) {
        qDebug() << "Error a la supression du laboratoire : " << query.lastError().text();
        return false;
    }
    return true;
}

bool Laboratoire::modifier(const QString nom)
{
    QSqlQuery query;
    query.prepare("UPDATE SMARTVACC.LABORATOIRES SET ADRESSE = :adresse, RESPONSABLE = :responsable, "
                  "TYPE = :type, STATUT = :statut, NB_PROJETS = :nb_projets, MATRIELS = :matriels, "
                  "PERSONNEL = :personnel, DEPONSE = :depense, DATE_CREATION = :date_creation "
                  "WHERE NOM_LAB = :nom");

    query.bindValue(":nom", nom);
    query.bindValue(":adresse", getAdresse());
    query.bindValue(":responsable", getResponsable());
    query.bindValue(":type", getType());
    query.bindValue(":statut", getStatut());
    query.bindValue(":nb_projets", getNbProjets());
    query.bindValue(":matriels", getMatriels());
    query.bindValue(":personnel", getPersonnel());
    query.bindValue(":depense", getDepense());
    query.bindValue(":date_creation", getDateCreation());

    if (!query.exec()) {
        qDebug() << "Error updating laboratoire: " << query.lastError().text();
        return false;
    }
    return true;
}
