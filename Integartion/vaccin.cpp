#include "vaccin.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
// Constructeur par défaut
Vaccin::Vaccin() {}

// Constructeur avec paramètres
Vaccin::Vaccin( int id_vaccination,QString nom, QString type, QDate date_creation, QDate date_expiration, QString statut, QString certification_vaccin) {
    this->id_vaccination = id_vaccination;
    this->nom = nom;
    this->type = type;
    this->date_creation = date_creation;
    this->date_expiration = date_expiration;
    this->statut = statut;
    this->certification_vaccin = certification_vaccin;
}

bool Vaccin::ajouter() {
    QSqlQuery query;

    // 🔹 Vérification de la connexion à la base de données
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Erreur : La base de données n'est pas connectée.";
        return false;
    }

    // 🔹 Préparer la requête SQL avec les corrections
    query.prepare("INSERT INTO SMARTVACC.VACCIN "
                  "( NOM, \"TYPE\", DATE_CREATION, DATE_EXPIRATION, STATUT, CERTIFICATION_VACCIN) "
                  "VALUES ( :nom, :type, TO_DATE(:date_creation, 'YYYY-MM-DD'), TO_DATE(:date_expiration, 'YYYY-MM-DD'), :statut, :certification)");

    // 🔹 Lier les valeurs des champs aux paramètres SQL
    query.bindValue(":nom", nom);
    query.bindValue(":type", type);
    query.bindValue(":date_creation", date_creation.toString("yyyy-MM-dd"));
    query.bindValue(":date_expiration", date_expiration.toString("yyyy-MM-dd"));
    query.bindValue(":statut", statut);
    query.bindValue(":certification", certification_vaccin);

    // 🔹 Exécuter la requête et gérer les erreurs
    if (!query.exec()) {
        qDebug() << "❌ Erreur lors de l'ajout du vaccin :" << query.lastError().text();
        qDebug() << "🔹 Requête SQL exécutée : " << query.lastQuery();
        return false;
    }

    qDebug() << "✅ Vaccin ajouté avec succès.";
    return true;
}
QSqlQueryModel* Vaccin::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    // 🔹 Exécuter la requête SQL pour récupérer tous les vaccins
    model->setQuery("SELECT  NOM, \"TYPE\", DATE_CREATION, DATE_EXPIRATION, STATUT, CERTIFICATION_VACCIN "
                    "FROM SMARTVACC.VACCIN");

    // 🔹 Vérifier s'il y a une erreur dans l'exécution de la requête
    if (model->lastError().isValid()) {
        qDebug() << "❌ Erreur lors de l'affichage des vaccins : " << model->lastError().text();
        return nullptr;
    }

    // 🔹 Définir les en-têtes des colonnes pour l'affichage
   // model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Vaccination"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date de Création"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date d'Expiration"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Certification Vaccin"));

    return model;
}
bool Vaccin::supprimer(QString nomv) {
    QSqlQuery query;
    query.prepare("DELETE FROM VACCIN WHERE nom= :nomv");
    query.bindValue(":nomv", nomv);
    return query.exec();  // Retourne true si la suppression réussit
}
bool Vaccin::remplirChampsModification(QString nomv, QString &nom, QString &type, QDate &date_creation, QDate &date_expiration, QString &statut, QString &certification) {
    QSqlQuery query;
    query.prepare("SELECT NOM, \"TYPE\", DATE_CREATION, DATE_EXPIRATION, STATUT, CERTIFICATION_VACCIN FROM SMARTVACC.VACCIN WHERE LOWER(NOM) = LOWER(:nomv)");
    query.bindValue(":nomv", nomv);

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL lors de la récupération des données :" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        nom = query.value(0).toString();
        type = query.value(1).toString();
        date_creation = query.value(2).toDate();
        date_expiration = query.value(3).toDate();
        statut = query.value(4).toString();
        certification = query.value(5).toString();

        qDebug() << "✅ Données récupérées pour le vaccin : " << nomv;
        qDebug() << "Nom:" << nom << "| Type:" << type << "| Date Création:" << date_creation
                 << "| Date Expiration:" << date_expiration << "| Statut:" << statut
                 << "| Certification:" << certification;

        return true;
    } else {
        qDebug() << "⚠️ Aucun vaccin trouvé pour le nom :" << nomv;
        return false;
    }
}

bool Vaccin::modifier(QString nomv) {
    QSqlQuery query;

    // Vérifier si le nouveau nom existe déjà (sauf s'il est identique à l'ancien)
    query.prepare("SELECT COUNT(*) FROM SMARTVACC.VACCIN WHERE LOWER(NOM) = LOWER(:nom) AND LOWER(NOM) != LOWER(:nomv)");
    query.bindValue(":nom", nom);
    query.bindValue(":nomv", nomv);

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL lors de la vérification du nom : " << query.lastError().text();
        return false;
    }

    query.next();
    int count = query.value(0).toInt();
    if (count > 0) {
        qDebug() << "⚠️ Le nom " << nom << " existe déjà !";
        return false;  // Empêche la modification si le nom existe déjà
    }

    query.prepare("UPDATE SMARTVACC.VACCIN SET \"TYPE\" = :type, DATE_CREATION = :date_creation, DATE_EXPIRATION = :date_expiration, STATUT = :statut, CERTIFICATION_VACCIN = :certification WHERE LOWER(NOM) = LOWER(:nomv)");
    query.bindValue(":type", type);
    query.bindValue(":date_creation", date_creation);
    query.bindValue(":date_expiration", date_expiration);
    query.bindValue(":statut", statut);
    query.bindValue(":certification", certification_vaccin);
    query.bindValue(":nomv", nomv);

    if (!query.exec()) {
        qDebug() << "❌ Erreur lors de la modification :" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ Vaccin " << nomv << " modifié avec succès.";
    return true;
}






