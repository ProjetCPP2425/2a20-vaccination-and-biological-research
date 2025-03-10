#include "compagne.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

// ---- Constructeurs ----
Compagne::Compagne() {}

Compagne::Compagne(int Id_Campagne, QString Nom_Campagne, QDate Date_Debut, QDate Date_Fin, QString Zone_Geographique,
                   int Objectif_Doses, int Doses_Administrees, QString Vaccins_Utilises, QString Statut)
{
    this->Id_Campagne = Id_Campagne;
    this->Nom_Campagne = Nom_Campagne;
    this->Date_Debut = Date_Debut;
    this->Date_Fin = Date_Fin;
    this->Zone_Geographique = Zone_Geographique;
    this->Objectif_Doses = Objectif_Doses;
    this->Doses_Administrees = Doses_Administrees;
    this->Vaccins_Utilises = Vaccins_Utilises;
    this->Statut = Statut;
}

// ---- Getters ----
int Compagne::getIdCampagne() const { return Id_Campagne; }
QString Compagne::getNomCampagne() const { return Nom_Campagne; }
QDate Compagne::getDateDebut() const { return Date_Debut; }
QDate Compagne::getDateFin() const { return Date_Fin; }
QString Compagne::getZoneGeographique() const { return Zone_Geographique; }
int Compagne::getObjectifDoses() const { return Objectif_Doses; }
int Compagne::getDosesAdministrees() const { return Doses_Administrees; }
QString Compagne::getVaccinsUtilises() const { return Vaccins_Utilises; }
QString Compagne::getStatut() const { return Statut; }

// ---- Setters ----
void Compagne::setIdCampagne(int id) { Id_Campagne = id; }
void Compagne::setNomCampagne(const QString &nom) { Nom_Campagne = nom; }
void Compagne::setDateDebut(const QDate &dateDebut) { Date_Debut = dateDebut; }
void Compagne::setDateFin(const QDate &dateFin) { Date_Fin = dateFin; }
void Compagne::setZoneGeographique(const QString &zone) { Zone_Geographique = zone; }
void Compagne::setObjectifDoses(int objectif) { Objectif_Doses = objectif; }
void Compagne::setDosesAdministrees(int doses) { Doses_Administrees = doses; }
void Compagne::setVaccinsUtilises(const QString &vaccins) { Vaccins_Utilises = vaccins; }
void Compagne::setStatut(const QString &statut) { Statut = statut; }

// ---- Fonction Ajouter ----
bool Compagne::ajouter()
{
    // Vérifier si une campagne avec le même nom existe déjà
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM COMPAGNE WHERE LOWER(NOM_CAMPAGNE) = LOWER(:nom_campagne)");
    checkQuery.bindValue(":nom_campagne", Nom_Campagne);

    if (!checkQuery.exec() || !checkQuery.next()) {
        qDebug() << "Erreur SQL lors de la vérification du nom de la campagne :" << checkQuery.lastError().text();
        return false;
    }

    if (checkQuery.value(0).toInt() > 0) {
        qDebug() << "Ajout impossible : Une campagne avec ce nom existe déjà.";
        return false;
    }

    // Si aucune campagne du même nom n'existe, on ajoute la nouvelle campagne
    QSqlQuery query;
    query.prepare("INSERT INTO COMPAGNE (NOM_CAMPAGNE, DATE_DEBUT, DATE_FIN, ZONE_GEOGRAPHIQUE, OBJECTIF_DOSES, "
                  "DOSES_ADMINISTREES, VACCINS_UTILISES, STATUT) "
                  "VALUES (:nom_campagne, :date_debut, :date_fin, :zone_geographique, :objectif_doses, :doses_administrees, :vaccins_utilises, :statut)");

    query.bindValue(":nom_campagne", Nom_Campagne);
    query.bindValue(":date_debut", Date_Debut);
    query.bindValue(":date_fin", Date_Fin);
    query.bindValue(":zone_geographique", Zone_Geographique);
    query.bindValue(":objectif_doses", Objectif_Doses);
    query.bindValue(":doses_administrees", Doses_Administrees);
    query.bindValue(":vaccins_utilises", Vaccins_Utilises);
    query.bindValue(":statut", Statut);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout de la campagne :" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ Campagne ajoutée avec succès :" << Nom_Campagne;
    return true;
}


// ---- Fonction Afficher ----
QSqlQueryModel* Compagne::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM COMPAGNE");

    if (model->lastError().isValid()) {
        qDebug() << "Erreur lors de l'affichage des campagnes :" << model->lastError().text();
        return nullptr;
    }

    return model;
}











bool Compagne::modifier(QString nom)
{
    QSqlQuery query;
    query.prepare("UPDATE COMPAGNE SET DATE_DEBUT = :date_debut, DATE_FIN = :date_fin, "
                  "ZONE_GEOGRAPHIQUE = :zone_geographique, OBJECTIF_DOSES = :objectif_doses, "
                  "DOSES_ADMINISTREES = :doses_administrees, VACCINS_UTILISES = :vaccins_utilises, "
                  "STATUT = :statut WHERE LOWER(NOM_CAMPAGNE) = LOWER(:old_nom)");

    query.bindValue(":old_nom", nom);
    query.bindValue(":date_debut", Date_Debut);
    query.bindValue(":date_fin", Date_Fin);
    query.bindValue(":zone_geographique", Zone_Geographique);
    query.bindValue(":objectif_doses", Objectif_Doses);
    query.bindValue(":doses_administrees", Doses_Administrees);
    query.bindValue(":vaccins_utilises", Vaccins_Utilises);
    query.bindValue(":statut", Statut);

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL lors de la modification de la campagne :" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ Modification réussie pour la campagne :" << nom;
    return true;
}


bool Compagne::chargerCampagne(QString nom)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM COMPAGNE WHERE LOWER(NOM_CAMPAGNE) = LOWER(:nom)");
    query.bindValue(":nom", nom);

    if (!query.exec() || !query.next()) {
        qDebug() << "❌ Aucune campagne trouvée avec le nom :" << nom;
        return false;
    }

    Id_Campagne = query.value(0).toInt();
    Nom_Campagne = query.value(1).toString();
    Date_Debut = query.value(2).toDate();
    Date_Fin = query.value(3).toDate();
    Zone_Geographique = query.value(4).toString();
    Objectif_Doses = query.value(5).toInt();
    Doses_Administrees = query.value(6).toInt();
    Vaccins_Utilises = query.value(7).toString();
    Statut = query.value(8).toString();

    qDebug() << "✅ Campagne trouvée :" << Nom_Campagne;
    return true;
}






bool Compagne::supprimer(QString nom)
{
    QSqlQuery query;
    query.prepare("DELETE FROM COMPAGNE WHERE LOWER(NOM_CAMPAGNE) = LOWER(:nom)");
    query.bindValue(":nom", nom);

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL lors de la suppression :" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ Campagne supprimée avec succès : " << nom;
    return true;
}




