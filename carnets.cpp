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
    // Vérifier si un carnet avec le même CIN existe déjà
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM CARNETS WHERE CIN = :cin");
    checkQuery.bindValue(":cin", Cin);

    if (!checkQuery.exec() || !checkQuery.next()) {
        qDebug() << "Erreur SQL lors de la vérification du CIN :" << checkQuery.lastError().text();
        return false;
    }

    if (checkQuery.value(0).toInt() > 0) {
        qDebug() << "Ajout impossible : Un carnet avec ce CIN existe déjà.";
        return false;
    }

    // Ajout du carnet
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

    // Définir les en-têtes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Âge"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Sexe"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Poids"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date RDV"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Remarques"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Statut Vaccinal"));

    return model;
}


bool Carnets::supprimer(QString cin)
{
    // Vérifier si le carnet existe avant suppression
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM CARNETS WHERE CIN = :cin");
    checkQuery.bindValue(":cin", cin);

    if (!checkQuery.exec() || !checkQuery.next()) {
        qDebug() << "Erreur SQL lors de la vérification de l'existence du carnet :" << checkQuery.lastError().text();
        return false;
    }

    if (checkQuery.value(0).toInt() == 0) {
        qDebug() << "Suppression impossible : Aucun carnet trouvé avec ce CIN.";
        return false;
    }

    // Suppression du carnet
    QSqlQuery query;
    query.prepare("DELETE FROM CARNETS WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la suppression du carnet :" << query.lastError().text();
        return false;
    }
    return true;
}


bool Carnets::modifier(QString cin, QString nom, QString prenom, int age, QString sexe,
                       QString num, float poids, QDate date_rdv, QString remarques, QString statut_vaccinal)
{
    // Vérifier si le carnet existe avant de modifier
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM CARNETS WHERE CIN = :cin");
    checkQuery.bindValue(":cin", cin);

    if (!checkQuery.exec() || !checkQuery.next()) {
        qDebug() << "Erreur SQL lors de la vérification du carnet :" << checkQuery.lastError().text();
        return false;
    }

    if (checkQuery.value(0).toInt() == 0) {
        qDebug() << "Modification impossible : Aucun carnet trouvé avec ce CIN.";
        return false;
    }

    // Mise à jour du carnet
    QSqlQuery query;
    query.prepare("UPDATE CARNETS SET NOM = :nom, PRENOM = :prenom, AGE = :age, "
                  "SEXE = :sexe, NUM = :num, POIDS = :poids, DATE_RDV = :date_rdv, "
                  "REMARQUES = :remarques, STATUT_VACCINAL = :statut WHERE CIN = :cin");

    query.bindValue(":cin", cin);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":age", age);
    query.bindValue(":sexe", sexe);
    query.bindValue(":num", num);
    query.bindValue(":poids", poids);
    query.bindValue(":date_rdv", date_rdv);
    query.bindValue(":remarques", remarques);
    query.bindValue(":statut", statut_vaccinal);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la modification du carnet :" << query.lastError().text();
        return false;
    }
    return true;
}


bool Carnets::remplirChampsModification(QString cin, QString &nom, QString &prenom, int &age,
                                        QString &sexe, QString &num, float &poids, QDate &date_rdv,
                                        QString &remarques, QString &statut_vaccinal)
{
    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM, AGE, SEXE, NUM, POIDS, DATE_RDV, REMARQUES, STATUT_VACCINAL "
                  "FROM CARNETS WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL lors du chargement du carnet :" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        //Charger les valeurs
        nom = query.value("NOM").toString();
        prenom = query.value("PRENOM").toString();
        age = query.value("AGE").toInt();
        sexe = query.value("SEXE").toString();
        num = query.value("NUM").toString();
        poids = query.value("POIDS").toFloat();
        date_rdv = query.value("DATE_RDV").toDate();
        remarques = query.value("REMARQUES").toString();
        statut_vaccinal = query.value("STATUT_VACCINAL").toString();

        return true;
    } else {
        qDebug() << "⚠️ Aucun carnet trouvé avec CIN :" << cin;
        return false;
    }
}

QString Carnets::analyserRisqueParCIN(QString cin)
{
    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM, AGE, POIDS, REMARQUES, STATUT_VACCINAL FROM CARNETS WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    if (!query.exec() || !query.next()) {
        return "⚠️ Erreur : Impossible de récupérer les données du patient.";
    }

    QString nom = query.value(0).toString();
    QString prenom = query.value(1).toString();
    int age = query.value(2).toInt();
    float poids = query.value(3).toFloat();
    QString remarques = query.value(4).toString();
    QString statutVaccinal = query.value(5).toString();

    // 🔹 Calcul du Score de Risque Médical (SRM)
    int scoreRisque = 0;
    QString categorieRisque = "✅ Faible";

    if (age > 50) scoreRisque += 15;
    if (age > 70) scoreRisque += 25;
    if (poids > 100) scoreRisque += 10;
    if (poids > 120) scoreRisque += 20;
    if (remarques.contains("fumeur", Qt::CaseInsensitive)) scoreRisque += 15;
    if (remarques.contains("alcool", Qt::CaseInsensitive)) scoreRisque += 10;
    if (remarques.contains("diabète", Qt::CaseInsensitive)) scoreRisque += 20;
    if (remarques.contains("hypertension", Qt::CaseInsensitive)) scoreRisque += 15;
    if (remarques.contains("maladie cardiaque", Qt::CaseInsensitive)) scoreRisque += 25;
    if (remarques.contains("asthme", Qt::CaseInsensitive)) scoreRisque += 10;
    if (remarques.contains("immunodéprimé", Qt::CaseInsensitive)) scoreRisque += 30;
    if (statutVaccinal == "Non Vacciné") scoreRisque += 20;

    if (scoreRisque >= 70) categorieRisque = "🚨 Critique";
    else if (scoreRisque >= 50) categorieRisque = "⚠️ Élevé";
    else if (scoreRisque >= 30) categorieRisque = "🟡 Modéré";

    // 📝 Rapport détaillé
    QString prediction = QString("📊 **Évaluation Médicale du Patient : M. %1 %2**\n\n"
                                 "🔹 **Âge** : %3 ans\n"
                                 "🔹 **Poids** : %4 kg\n"
                                 "🔹 **Statut Vaccinal** : %5\n\n"
                                 "➡️ **Score de Risque Médical (SRM) : %6 / 100**\n"
                                 "➡️ **Niveau de Risque Identifié** : %7\n\n")
                             .arg(nom)
                             .arg(prenom)
                             .arg(age)
                             .arg(poids)
                             .arg(statutVaccinal)
                             .arg(scoreRisque)
                             .arg(categorieRisque);




    // 🔹 ALERTE MÉDICALE (Recommandations après vaccination)
    QString alerteVaccin = "\n🔔 **Recommandations Post-Vaccinales :**\n\n";

    if (categorieRisque == "🚨 Critique") {
        prediction += "**🛑 RISQUE CRITIQUE DÉTECTÉ !**\n\n"
                      "Le patient présente un niveau de risque médical **très élevé** en raison de plusieurs facteurs de santé.\n"
                      "Les complications potentielles incluent **des troubles cardiovasculaires graves, un risque accru d'infection sévère, et une réponse immunitaire affaiblie**.\n\n"
                      "👉 **Mesures immédiates recommandées :**\n"
                      "   - **Évitez toute activité physique intense durant les 48 prochaines heures.**\n"
                      "   - **Surveillez tout effet secondaire inhabituel (fièvre prolongée, douleurs intenses, essoufflement).**\n"
                      "   - **Consultez un médecin en urgence en cas d'apparition de symptômes inquiétants.**\n";
        alerteVaccin += "⚠️ **Après la vaccination, le patient doit se reposer strictement et éviter toute prise d'anti-inflammatoires sans avis médical.**\n";
    }
    else if (categorieRisque == "⚠️ Élevé") {
        prediction += "**⚠️ RISQUE ÉLEVÉ IDENTIFIÉ**\n\n"
                      "Ce patient présente des facteurs de risque qui nécessitent une surveillance accrue après la vaccination.\n"
                      "Les effets secondaires peuvent inclure **fatigue excessive, maux de tête prolongés, douleurs musculaires sévères**.\n\n"
                      "👉 **Conseils recommandés :**\n"
                      "   - **Évitez le stress et reposez-vous suffisamment après l’injection.**\n"
                      "   - **Hydratez-vous régulièrement pour prévenir les risques de déshydratation.**\n"
                      "   - **Surveillez la température corporelle pendant les prochaines 48 heures.**\n";
        alerteVaccin += "⚠️ **Évitez l’exposition prolongée au soleil et ne prenez pas d’aspirine immédiatement après la vaccination.**\n";
    }
    else if (categorieRisque == "🟡 Modéré") {
        prediction += "**🟡 RISQUE MODÉRÉ : SUIVI RECOMMANDÉ**\n\n"
                      "Bien que ce patient ne présente pas de risque critique, certaines précautions doivent être prises.\n"
                      "Les symptômes post-vaccination peuvent inclure **légère fatigue, rougeur au site d’injection, ou fièvre modérée**.\n\n"
                      "👉 **Précautions suggérées :**\n"
                      "   - **Évitez les efforts physiques intenses pendant 24 heures.**\n"
                      "   - **Adoptez une alimentation équilibrée pour optimiser la réponse immunitaire.**\n"
                      "   - **En cas de fièvre légère, privilégiez le paracétamol plutôt que les anti-inflammatoires.**\n";
        alerteVaccin += "🔹 **Restez hydraté et surveillez votre état général durant les premiers jours suivant la vaccination.**\n";
    }
    else {
        prediction += "**✅ ÉTAT DE SANTÉ SATISFAISANT**\n\n"
                      "Aucun facteur de risque majeur n’a été détecté chez ce patient.\n"
                      "Toutefois, il est toujours recommandé de suivre quelques précautions générales après la vaccination.\n\n"
                      "👉 **Recommandations générales :**\n"
                      "   - **Surveillez tout effet secondaire éventuel (maux de tête, fatigue).**\n"
                      "   - **Assurez-vous d’avoir un suivi médical régulier.**\n"
                      "   - **Maintenez un mode de vie sain et équilibré.**\n";
        alerteVaccin += "✅ **Aucune précaution particulière à prendre après la vaccination.**\n";
    }

    return prediction + alerteVaccin;
}
