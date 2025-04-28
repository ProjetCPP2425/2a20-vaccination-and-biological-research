#include "compagne.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

// ---- Constructeurs ----
Compagne::Compagne() {}

Compagne::Compagne(int Id_Compagne, QString Nom_Compagne, QDate Date_Debut, QDate Date_Fin, QString Zone_Geographique,
                   int Objectif_Doses, int Doses_Administrees, QString Vaccins_Utilises, QString Statut ,QString Fournitures, float Cout_Fournitures)
{
    this->Id_Compagne = Id_Compagne;
    this->Nom_Compagne = Nom_Compagne;
    this->Date_Debut = Date_Debut;
    this->Date_Fin = Date_Fin;
    this->Zone_Geographique = Zone_Geographique;
    this->Objectif_Doses = Objectif_Doses;
    this->Doses_Administrees = Doses_Administrees;
    this->Vaccins_Utilises = Vaccins_Utilises;
    this->Statut = Statut;

    this->Fournitures = Fournitures;
    this->Cout_Fournitures = Cout_Fournitures;

}

// ---- Getters ----
int Compagne::getIdCompagne() const { return Id_Compagne; }
QString Compagne::getNomCompagne() const { return Nom_Compagne; }
QDate Compagne::getDateDebut() const { return Date_Debut; }
QDate Compagne::getDateFin() const { return Date_Fin; }
QString Compagne::getZoneGeographique() const { return Zone_Geographique; }
int Compagne::getObjectifDoses() const { return Objectif_Doses; }
int Compagne::getDosesAdministrees() const { return Doses_Administrees; }
QString Compagne::getVaccinsUtilises() const { return Vaccins_Utilises; }
QString Compagne::getStatut() const { return Statut; }

QString Compagne::getFournitures() const { return Fournitures; }
float Compagne::getCoutFournitures() const { return Cout_Fournitures; }

// ---- Setters ----
void Compagne::setIdCompagne(int id) { Id_Compagne = id; }
void Compagne::setNomCompagne(const QString &nom) { Nom_Compagne = nom; }
void Compagne::setDateDebut(const QDate &dateDebut) { Date_Debut = dateDebut; }
void Compagne::setDateFin(const QDate &dateFin) { Date_Fin = dateFin; }
void Compagne::setZoneGeographique(const QString &zone) { Zone_Geographique = zone; }
void Compagne::setObjectifDoses(int objectif) { Objectif_Doses = objectif; }
void Compagne::setDosesAdministrees(int doses) { Doses_Administrees = doses; }
void Compagne::setVaccinsUtilises(const QString &vaccins) { Vaccins_Utilises = vaccins; }
void Compagne::setStatut(const QString &statut) { Statut = statut; }

void Compagne::setFournitures(const QString &fournitures) { Fournitures = fournitures; }
void Compagne::setCoutFournitures(float cout) { Cout_Fournitures = cout; }

// ---- Fonction Ajouter ----
bool Compagne::ajouter()
{
    // Vérifier si une campagne avec le même nom existe déjà
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM COMPAGNE WHERE LOWER(NOM_COMPAGNE) = LOWER(:nom_compagne)");
    checkQuery.bindValue(":nom_compagne", Nom_Compagne);

    if (!checkQuery.exec() || !checkQuery.next()) {
        qDebug() << "Erreur SQL lors de la vérification du nom de la compagne :" << checkQuery.lastError().text();
        return false;
    }

    if (checkQuery.value(0).toInt() > 0) {
        qDebug() << "Ajout impossible : Une compagne avec ce nom existe déjà.";
        return false;
    }

    // Si aucune campagne du même nom n'existe, on ajoute la nouvelle campagne
    QSqlQuery query;
    query.prepare("INSERT INTO COMPAGNE (NOM_COMPAGNE, DATE_DEBUT, DATE_FIN, ZONE_GEOGRAPHIQUE, OBJECTIF_DOSES, "
                  "DOSES_ADMINISTREES, VACCINS_UTILISES, STATUT, FOURNITURES, COUT_FOURNITURES) "
                  "VALUES (:nom_compagne, :date_debut, :date_fin, :zone_geographique, :objectif_doses, :doses_administrees, "
                  ":vaccins_utilises, :statut, :fournitures, :cout_fournitures)");

    query.bindValue(":nom_compagne", Nom_Compagne);
    query.bindValue(":date_debut", Date_Debut);
    query.bindValue(":date_fin", Date_Fin);
    query.bindValue(":zone_geographique", Zone_Geographique);
    query.bindValue(":objectif_doses", Objectif_Doses);
    query.bindValue(":doses_administrees", Doses_Administrees);
    query.bindValue(":vaccins_utilises", Vaccins_Utilises);
    query.bindValue(":statut", Statut);

    query.bindValue(":fournitures", Fournitures);
    query.bindValue(":cout_fournitures", Cout_Fournitures);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout de la compagne :" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ Compagne ajoutée avec succès :" << Nom_Compagne;
    return true;
}


// ---- Fonction Afficher ----
QSqlQueryModel* Compagne::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(R"(
    SELECT
        NOM_COMPAGNE,
        TO_CHAR(DATE_DEBUT, 'DD/MM/YYYY') AS DATE_DEBUT,
        TO_CHAR(DATE_FIN, 'DD/MM/YYYY') AS DATE_FIN,
        ZONE_GEOGRAPHIQUE,
        OBJECTIF_DOSES,
        DOSES_ADMINISTREES,
        VACCINS_UTILISES,
        STATUT,
        FOURNITURES,
        COUT_FOURNITURES
    FROM COMPAGNE
)");


    if (model->lastError().isValid()) {
        qDebug() << "Erreur lors de l'affichage des compagnes :" << model->lastError().text();
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
                  "STATUT = :statut, FOURNITURES = :fournitures, COUT_FOURNITURES = :cout_fournitures "
                  "WHERE LOWER(NOM_COMPAGNE) = LOWER(:old_nom)");

    query.bindValue(":old_nom", nom);
    query.bindValue(":date_debut", Date_Debut);
    query.bindValue(":date_fin", Date_Fin);
    query.bindValue(":zone_geographique", Zone_Geographique);
    query.bindValue(":objectif_doses", Objectif_Doses);
    query.bindValue(":doses_administrees", Doses_Administrees);
    query.bindValue(":vaccins_utilises", Vaccins_Utilises);
    query.bindValue(":statut", Statut);


    query.bindValue(":fournitures", Fournitures);
    query.bindValue(":cout_fournitures", Cout_Fournitures);

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL lors de la modification de la campagne :" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ Modification réussie pour la campagne :" << nom;
    return true;
}


bool Compagne::chargerCompagne(QString nom)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM COMPAGNE WHERE LOWER(NOM_COMPAGNE) = LOWER(:nom)");
    query.bindValue(":nom", nom);

    if (!query.exec() || !query.next()) {
        qDebug() << "❌ Aucune compagne trouvée avec le nom :" << nom;
        return false;
    }

    Id_Compagne = query.value(0).toInt();
    Nom_Compagne = query.value(1).toString();
    Date_Debut = query.value(2).toDate();
    Date_Fin = query.value(3).toDate();
    Zone_Geographique = query.value(4).toString();
    Objectif_Doses = query.value(5).toInt();
    Doses_Administrees = query.value(6).toInt();
    Vaccins_Utilises = query.value(7).toString();
    Statut = query.value(8).toString();

    Fournitures = query.value(9).toString();
    Cout_Fournitures = query.value(10).toFloat();

    qDebug() << "✅ Compagne trouvée :" << Nom_Compagne;
    return true;
}







bool Compagne::supprimer(QString nom)
{
    // Étape 1 : Obtenir l'ID de la compagne à partir de son nom
    QSqlQuery getId;
    getId.prepare("SELECT ID_COMPAGNE FROM COMPAGNE WHERE LOWER(NOM_COMPAGNE) = LOWER(:nom)");
    getId.bindValue(":nom", nom);

    if (!getId.exec() || !getId.next()) {
        qDebug() << "❌ Erreur pour obtenir l'ID de la compagne :" << getId.lastError().text();
        return false;
    }

    int id_compagne = getId.value(0).toInt();

    // Étape 2 : Supprimer d'abord les contributions liées à cette compagne
    QSqlQuery deleteContrib;
    deleteContrib.prepare("DELETE FROM CONTRIBUER WHERE ID_COMPAGNE = :id");
    deleteContrib.bindValue(":id", id_compagne);

    if (!deleteContrib.exec()) {
        qDebug() << "❌ Erreur suppression des contributions :" << deleteContrib.lastError().text();
        return false;
    }

    // Étape 3 : Supprimer la compagne (par nom)
    QSqlQuery deleteCompagne;
    deleteCompagne.prepare("DELETE FROM COMPAGNE WHERE LOWER(NOM_COMPAGNE) = LOWER(:nom)");
    deleteCompagne.bindValue(":nom", nom);

    if (!deleteCompagne.exec()) {
        qDebug() << "❌ Erreur SQL lors de la suppression de la compagne :" << deleteCompagne.lastError().text();
        return false;
    }

    qDebug() << "✅ Compagne supprimée avec succès : " << nom;
    return true;
}



