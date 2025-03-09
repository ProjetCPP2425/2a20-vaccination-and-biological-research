#include "employes.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

// Constructeur par défaut
Employe::Employe() {}

// Constructeur avec paramètres
Employe::Employe(QString cin, int id, QString nom, QString prenom, QString poste, QString sexe, float salaire,
                 QString contact, QDate date_embauche, int disponibilite, QString type_absences)
{
    this->CIN = cin;
    this->ID_EMPLOYE = id;
    this->NOM = nom;
    this->PRENOM = prenom;
    this->POSTE = poste;
    this->SEXE = sexe;
    this->SALAIRE = salaire;
    this->CONTACT = contact;
    this->DATE_EMBAUCHE = date_embauche;
    this->DISPONIBILITE = disponibilite;
    this->TYPE_ABSENCES = type_absences;
}

// 🔹 Ajouter un employé
bool Employe::ajouter() {
    QSqlQuery query;


    // Vérification de la connexion
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Erreur : La base de données n'est pas connectée.";
        return false;
    }

    // Préparation de la requête SQL avec correction pour Oracle
    query.prepare("INSERT INTO SMARTVACC.EMPLOYES (CIN, ID_EMPLOYE, NOM, PRENOM, POSTE, SEXE, SALAIRE, CONTACT, DATE_EMBAUCHE, DISPONIBILITE, \"TYPE_ABSENCES\") "
                  "VALUES (:cin, :id, :nom, :prenom, :poste, :sexe, :salaire, :contact, TO_DATE(:date_embauche, 'YYYY-MM-DD'), :disponibilite, :type_absences)");

    // Binding des valeurs
    query.bindValue(":cin", CIN);
    query.bindValue(":id", ID_EMPLOYE);
    query.bindValue(":nom", NOM);
    query.bindValue(":prenom", PRENOM);
    query.bindValue(":poste", POSTE);
    query.bindValue(":sexe", SEXE);
    query.bindValue(":salaire", SALAIRE);
    query.bindValue(":contact", CONTACT);
    query.bindValue(":date_embauche", DATE_EMBAUCHE.toString("yyyy-MM-dd"));  // Format correct pour Oracle
    query.bindValue(":disponibilite", DISPONIBILITE);
    query.bindValue(":type_absences", TYPE_ABSENCES);

    // Exécution de la requête et gestion des erreurs SQL
    if (!query.exec()) {
        qDebug() << "❌ ERREUR SQL : " << query.lastError().text();
        qDebug() << "🔹 Requête SQL : " << query.lastQuery();
        QMessageBox::critical(nullptr, "Erreur SQL", "Impossible d'ajouter l'employé : " + query.lastError().text());
        return false;
    }

    // Succès
    qDebug() << "✅ Employé ajouté avec succès.";
    QMessageBox::information(nullptr, "Succès", "L'employé a été ajouté avec succès !");
    return true;
}


QSqlQueryModel* Employe::afficher() {
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT CIN, ID_EMPLOYE, NOM, PRENOM, POSTE, SEXE, SALAIRE, CONTACT, DATE_EMBAUCHE, DISPONIBILITE, TYPE_ABSENCES FROM SMARTVACC.EMPLOYES");

    if (model->lastError().isValid()) {
        lastError = model->lastError().text();
        delete model;
        return nullptr;
    }

    return model;
}



bool Employe::modifier(int id_employe) {
    // 🔹 Vérification de la connexion
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "❌ Erreur : La base de données n'est pas connectée.";
        return false;
    }

    // 🔹 Préparation de la requête SQL
    QSqlQuery query;
    query.prepare("UPDATE SMARTVACC.EMPLOYES SET "
                  "CIN = :cin, NOM = :nom, PRENOM = :prenom, POSTE = :poste, "
                  "SEXE = :sexe, SALAIRE = :salaire, CONTACT = :contact, "
                  "DATE_EMBAUCHE = TO_DATE(:date_embauche, 'YYYY-MM-DD'), "
                  "DISPONIBILITE = :disponibilite, TYPE_ABSENCES = :type_absences "
                  "WHERE ID_EMPLOYE = :id_employe");

    // 🔹 Binding des valeurs avec les variables de la classe
    query.bindValue(":cin", this->CIN);
    query.bindValue(":id_employe", id_employe);
    query.bindValue(":nom", this->NOM);
    query.bindValue(":prenom", this->PRENOM);
    query.bindValue(":poste", this->POSTE);
    query.bindValue(":sexe", this->SEXE);
    query.bindValue(":salaire", this->SALAIRE);
    query.bindValue(":contact", this->CONTACT);
    query.bindValue(":date_embauche", this->DATE_EMBAUCHE.toString("yyyy-MM-dd"));
    query.bindValue(":disponibilite", this->DISPONIBILITE);
    query.bindValue(":type_absences", this->TYPE_ABSENCES);

    // 🔹 Exécution de la requête et gestion des erreurs
    if (!query.exec()) {
        qDebug() << "❌ ERREUR SQL : " << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur SQL", "Échec de la modification : " + query.lastError().text());
        return false;
    }

    // ✅ Succès
    qDebug() << "✅ Employé modifié avec succès.";
    QMessageBox::information(nullptr, "Succès", "Les informations de l'employé ont été mises à jour !");
    return true;
}


bool Employe::supprimerParCIN(QString cin) {
    QSqlQuery query;
    query.prepare("DELETE FROM SMARTVACC.EMPLOYES WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    if (!query.exec()) {
        lastError = query.lastError().text();
        return false;
    }
    return true;
}
