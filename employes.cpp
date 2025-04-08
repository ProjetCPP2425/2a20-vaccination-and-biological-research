
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
    // 🔹 Affectation des valeurs aux attributs de la classe
    this->CIN = cin.trimmed();
    this->ID_EMPLOYE = id;
    this->NOM = nom.trimmed();
    this->PRENOM = prenom.trimmed();
    this->POSTE = poste.trimmed();
    this->SEXE = sexe.trimmed();
    this->SALAIRE = salaire;
    this->CONTACT = contact.trimmed();
    this->DATE_EMBAUCHE = date_embauche;
    this->DISPONIBILITE = disponibilite;
    this->TYPE_ABSENCES = type_absences.trimmed();

    // 🔍 Affichage pour débogage
    qDebug() << "✅ Constructeur Employe appelé avec :"
             << "\n   CIN: " << this->CIN
             << "\n   ID: " << this->ID_EMPLOYE
             << "\n   Nom: " << this->NOM
             << "\n   Prénom: " << this->PRENOM
             << "\n   Poste: " << this->POSTE
             << "\n   Sexe: " << this->SEXE
             << "\n   Salaire: " << this->SALAIRE
             << "\n   Contact: " << this->CONTACT
             << "\n   Date embauche: " << this->DATE_EMBAUCHE.toString("dd/MM/yyyy")
             << "\n   Disponibilité: " << (this->DISPONIBILITE ? "Oui" : "Non")
             << "\n   Type absences: " << this->TYPE_ABSENCES;
}


// 🔹 Getters
QString Employe::getCIN() const { return CIN; }
int Employe::getIdEmploye() const { return ID_EMPLOYE; }
QString Employe::getNom() const { return NOM; }
QString Employe::getPrenom() const { return PRENOM; }
QString Employe::getPoste() const { return POSTE; }
QString Employe::getSexe() const { return SEXE; }
float Employe::getSalaire() const { return SALAIRE; }
QString Employe::getContact() const { return CONTACT; }
QDate Employe::getDateEmbauche() const { return DATE_EMBAUCHE; }
int Employe::getDisponibilite() const { return DISPONIBILITE; }
QString Employe::getTypeAbsences() const { return TYPE_ABSENCES; }

// 🔹 Setters
void Employe::setCIN(const QString &cin) { CIN = cin; }
void Employe::setIdEmploye(int id) { ID_EMPLOYE = id; }
void Employe::setNom(const QString &nom) { NOM = nom; }
void Employe::setPrenom(const QString &prenom) { PRENOM = prenom; }
void Employe::setPoste(const QString &poste) { POSTE = poste; }
void Employe::setSexe(const QString &sexe) { SEXE = sexe; }
void Employe::setSalaire(float salaire) { SALAIRE = salaire; }
void Employe::setContact(const QString &contact) { CONTACT = contact; }
void Employe::setDateEmbauche(const QDate &date) { DATE_EMBAUCHE = date; }
void Employe::setDisponibilite(int disponibilite) { DISPONIBILITE = disponibilite; }
void Employe::setTypeAbsences(const QString &typeAbsences) { TYPE_ABSENCES = typeAbsences; }

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
    model->setQuery("SELECT CIN, NOM, PRENOM, POSTE, SEXE, SALAIRE, CONTACT, DATE_EMBAUCHE, DISPONIBILITE, TYPE_ABSENCES FROM SMARTVACC.EMPLOYES");

    if (model->lastError().isValid()) {
        lastError = model->lastError().text();
        delete model;
        return nullptr;
    }

    return model;
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










// 🔹 Charger un employé pour modification
bool Employe::chargerEmploye(QString cin)
{
    QSqlQuery query;
    query.prepare("SELECT CIN, ID_EMPLOYE, NOM, PRENOM, POSTE, SEXE, SALAIRE, CONTACT, DATE_EMBAUCHE, DISPONIBILITE, TYPE_ABSENCES FROM SMARTVACC.EMPLOYES WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    if (!query.exec()) {
        qDebug() << "❌ ERREUR SQL (chargerEmploye) :" << query.lastError().text();
        return false;
    }

    if (!query.next()) {
        qDebug() << "❌ Aucun employé trouvé avec CIN :" << cin;
        return false;
    }

    // Charger les valeurs depuis la base de données
    CIN = query.value("CIN").toString();  // 🔹 Correction
    ID_EMPLOYE = query.value("ID_EMPLOYE").toInt();  // 🔹 Correction
    NOM = query.value("NOM").toString();
    PRENOM = query.value("PRENOM").toString();
    POSTE = query.value("POSTE").toString();
    SEXE = query.value("SEXE").toString();
    SALAIRE = query.value("SALAIRE").toFloat();
    CONTACT = query.value("CONTACT").toString();
    DATE_EMBAUCHE = query.value("DATE_EMBAUCHE").toDate();
    DISPONIBILITE = query.value("DISPONIBILITE").toInt();
    TYPE_ABSENCES = query.value("TYPE_ABSENCES").toString();

    qDebug() << "✅ Employé chargé :" << CIN << " | ID: " << ID_EMPLOYE;
    return true;
}








// 🔹 Modifier un employé
bool Employe::modifier(QString cin)
{
    QSqlQuery query;
    query.prepare("UPDATE SMARTVACC.EMPLOYES SET "
                  "NOM = :nom, PRENOM = :prenom, POSTE = :poste, SEXE = :sexe, "
                  "SALAIRE = :salaire, CONTACT = :contact, DATE_EMBAUCHE = TO_DATE(:date_embauche, 'YYYY-MM-DD'), "
                  "DISPONIBILITE = :disponibilite, TYPE_ABSENCES = :type_absences "
                  "WHERE CIN = :old_cin");

    query.bindValue(":old_cin", cin);
    query.bindValue(":nom", NOM);
    query.bindValue(":prenom", PRENOM);
    query.bindValue(":poste", POSTE);
    query.bindValue(":sexe", SEXE);
    query.bindValue(":salaire", SALAIRE);
    query.bindValue(":contact", CONTACT);
    query.bindValue(":date_embauche", DATE_EMBAUCHE.toString("yyyy-MM-dd"));
    query.bindValue(":disponibilite", DISPONIBILITE);
    query.bindValue(":type_absences", TYPE_ABSENCES);

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL lors de la modification de l'employé :" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ Modification réussie pour l'employé :" << cin;
    return true;
}









bool Employe::authentifier(const QString &login, const QString &cin)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM SMARTVACC.EMPLOYES WHERE PRENOM || SUBSTR(CIN, -4) = :login AND CIN = :cin");
    query.bindValue(":login", login);
    query.bindValue(":cin", cin);

    if (!query.exec()) {
        qDebug() << "Erreur SQL (authentifier) :" << query.lastError().text();
        return false;
    }

    return query.next(); // ✅ Authentifié si une ligne existe
}

QString Employe::getPosteFromCIN(const QString &cin)
{
    QSqlQuery query;
    query.prepare("SELECT POSTE FROM SMARTVACC.EMPLOYES WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    if (!query.exec() || !query.next()) {
        qDebug() << "Erreur récupération poste :" << query.lastError().text();
        return "";
    }

    return query.value("POSTE").toString();
}






QSqlQueryModel* Employe::afficherParPoste() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM SMARTVACC.EMPLOYES ORDER BY POSTE ASC");
    return model;
}

QSqlQueryModel* Employe::afficherParAnciennete() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM SMARTVACC.EMPLOYES ORDER BY DATE_EMBAUCHE ASC");
    return model;
}

QSqlQueryModel* Employe::afficherParSalaire() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM SMARTVACC.EMPLOYES ORDER BY SALAIRE ASC");
    return model;
}
QSqlQueryModel* Employe::rechercherParCIN(const QString &cin) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM SMARTVACC.EMPLOYES WHERE CIN = :cin");
    query.bindValue(":cin", cin);
    query.exec();
    model->setQuery(query);
    return model;
}

QSqlQueryModel* Employe::rechercherParContact(const QString &contact) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM SMARTVACC.EMPLOYES WHERE CONTACT = :contact");
    query.bindValue(":contact", contact);
    query.exec();
    model->setQuery(query);
    return model;
}



QString Employe::getPrenomFromCIN(const QString &cin) {
    QSqlQuery query;
    query.prepare("SELECT PRENOM FROM SMARTVACC.EMPLOYES WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }
    return "";
}


QSqlQueryModel* Employe::rechercherParDisponibilite(int dispo) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM SMARTVACC.EMPLOYES WHERE DISPONIBILITE = :dispo");
    query.bindValue(":dispo", dispo);
    query.exec();
    model->setQuery(query);
    return model;
}



