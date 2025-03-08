#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "vaccin.h"
#include <QMessageBox>
#include <QHBoxLayout>  // Pour QHBoxLayout
#include <QPushButton>  // Pour QPushButton
#include <QWidget>      // Pour QWidget
#include <QDebug>       // (Facultatif) Pour le debug
#include <QSqlError>  // Ajoute cette ligne pour afficher les erreurs SQL
#include <QSystemTrayIcon>
#include <QApplication>
#include <QDate>
#include <QSqlQuery>

// Constructeur de MainWindow
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(6);
    ui->frame->setVisible(false);
    ui->labelErrorNom->clear(); // Supprimer le texte
    ui->labelErrorNom->setStyleSheet("color: transparent; background-color: transparent; border: none; font-weight: normal;");
    ui->labelErrorType->setStyleSheet("color: transparent; background-color: transparent; border: none; font-weight: normal;");
    ui->labelErrorCertification->setStyleSheet("color: transparent; background-color: transparent; border: none; font-weight: normal;");

        qDebug() << "🚀 Initialisation de MainWindow. nomAModifier =" << nomAModifier;


    // Connexion des champs à la vérification en temps réel
     connect(ui->lineEdit_nom_2, &QLineEdit::textChanged, this, &MainWindow::verifierNom);
    connect(ui->lineEdit_typev_2, &QLineEdit::textChanged, this, &MainWindow::verifierTypeVaccin);
    connect(ui->lineEdit_certification_2, &QLineEdit::textChanged, this, &MainWindow::verifierCertificationVaccin);

    // 🔹 Initialiser medApi ici pour éviter un accès à un nullptr
    medApi = new MedAnalysis(this);
    connect(medApi, &MedAnalysis::dataFetched, this, &MainWindow::updateCovidStats);
    connect(medApi, &MedAnalysis::errorOccurred, this, &MainWindow::showError);

    // 🔹 Initialisation de la liste des pays
    ui->comboBoxPays->addItems({
        "France", "Tunisie", "Allemagne", "Italie", "Espagne", "États-Unis", "Canada", "Maroc", "Algérie", "Chine"
    });
    // ✅ Appliquer un style moderne avec des bordures arrondies et un fond agréable
    this->setStyleSheet(R"(
    QWidget {
        background-color: #f4f4f4;
        font-family: 'Arial';
    }



    QPushButton {
        background-color: #B22222;
        color: white;
        border-radius: 10px;
        padding: 8px;
        font-size: 14px;
        font-weight: bold;
    }

    QPushButton:hover {
        background-color: #8B0000;
    }

    QComboBox {
        background-color: white;
        border: 1px solid #aaa;
        border-radius: 5px;
        padding: 5px;
    }

    QTableView {
        background-color: white;
        border-radius: 5px;
        border: 1px solid #ddd;
    }

    QHeaderView::section {
        background-color: #B22222;
        color: white;
        padding: 5px;
        font-weight: bold;
    }

    QMessageBox {
        font-size: 14px;
    }
)");

    connect(ui->btnPredire, &QPushButton::clicked, this, &MainWindow::lancerPrediction);
    // Fonction pour gérer l'affichage du sidebar selon la page active
    auto updateSidebarVisibility = [=](int index) {
        if (index == 6) { // Page de connexion
            ui->frame->setVisible(false);
        } else {
            ui->frame->setVisible(true);
        }
    };

    // Bouton retour à la connexion
    connect(ui->pushButton_3, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(6);
        updateSidebarVisibility(6);
    });

    connect(ui->pushButton_6, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(6);
        ui->frame->setVisible(false);
    });

    connect(ui->pushButton_3, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(0);
        ui->frame->setVisible(true);
    });

    // Navigation entre les pages
    connect(ui->employe, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(0);
        ui->frame->setVisible(true);
    });

    connect(ui->produits, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(1);
        ui->frame->setVisible(true);
    });

    connect(ui->labo, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(2);
        ui->frame->setVisible(true);
    });

    connect(ui->vaccin, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(3);
        ui->frame->setVisible(true);
        verifierVaccinsExpires();
        // Mettre à jour immédiatement l'affichage des vaccins
        Vaccin v;
        ui->tableView->setModel(v.afficher());
        // Ajustements d'affichage
        ui->tableView->verticalHeader()->setDefaultSectionSize(35);
        ui->tableView->horizontalHeader()->setDefaultSectionSize(150);
        ui->tableView->resizeColumnsToContents();
        ui->tableView->setStyleSheet("QTableView::item { padding: 10px; }");
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    });


    connect(ui->carnet, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(5);
    });

    connect(ui->compagne, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(4);
        ui->frame->setVisible(true);
    });

    // 🔹 Connecter le bouton "Ajouter" pour insérer un vaccin
    connect(ui->pushButton_ajouter_2, &QPushButton::clicked, this, &MainWindow::on_pushButton_ajouter_v_clicked);




}

// Destructeur
MainWindow::~MainWindow()
{
    delete ui;
}

// Bouton "Ajouter" pour insérer un vaccin
/*void MainWindow::on_pushButton_ajouter_v_clicked() {
    // Récupérer les valeurs saisies dans l'interface
    QString nom = ui->lineEdit_nom_2->text();
    QString type = ui->lineEdit_typev_2->text();
    QDate date_creation = ui->dateEdit_creation_2->date();
    QDate date_expiration = ui->dateEdit_expiration_2->date();
    QString statut = ui->comboBox_status_2->currentText();
    QString certification = ui->lineEdit_certification_2->text();

    // Afficher les valeurs saisies dans la console
    qDebug() << "Nom:" << nom << "| Type:" << type << "| Date Création:" << date_creation
             << "| Date Expiration:" << date_expiration << "| Statut:" << statut
             << "| Certification:" << certification;

    if (!ui->labelErrorNom->text().isEmpty() ||
        !ui->labelErrorType->text().isEmpty() ||
        !ui->labelErrorCertification->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez corriger les erreurs avant de continuer !");
        return;
    }


    // Vérifier que la date d'expiration est après la date de création
    if (date_expiration <= date_creation) {
        QMessageBox::warning(this, "Date invalide", "La date d'expiration doit être après la date de création !");
        return;
    }

    // Créer un objet Vaccin
    Vaccin v(0,nom, type, date_creation, date_expiration, statut, certification);
    qDebug() << "🔍 Avant modification, nomAModifier =" << nomAModifier;
    if (modeModification) {

        if (nomAModifier.isEmpty()) {
            qDebug() << "❌ ERREUR: nomAModifier est vide lors de la modification !";
        }
        // 🔹 Mode Modification : Mettre à jour l'enregistrement existant
        if (v.modifier(nomAModifier)) {
            QMessageBox::information(this, "Succès", "Vaccin modifié avec succès !");
            modeModification = false;  // Désactiver le mode modification
            //nomAModifier ="";
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la modification !");
            return;
        }
        qDebug() << "🔍 Après modification, nomAModifier =" << nomAModifier;
    } else {
        // 🔹 Mode Ajout : Ajouter un nouveau vaccin
        qDebug() << "Mode Ajout - Nouveau vaccin";

        if (v.ajouter()) {
            QMessageBox::information(this, "Succès", "Vaccin ajouté avec succès !");
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de l'ajout !");
            return;
        }
    }

    // 🔄 Mise à jour de la table après ajout/modification
    ui->tableView->setModel(v.afficher());
    ui->tableView->verticalHeader()->setDefaultSectionSize(35);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(150);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setStyleSheet("QTableView::item { padding: 10px; }");
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
   ui->lineEdit_nom_2->setDisabled(false);
    // 🧹 Effacer les champs après ajout/modification
    ui->lineEdit_nom_2->clear();
    ui->lineEdit_typev_2->clear();
    ui->comboBox_status_2->setCurrentIndex(0);
    ui->dateEdit_creation_2->setDate(QDate::currentDate());
    ui->dateEdit_expiration_2->setDate(QDate::currentDate());
    ui->lineEdit_certification_2->clear();
}
*/
void MainWindow::on_pushButton_ajouter_v_clicked() {
    QString nom = ui->lineEdit_nom_2->text();
    QString type = ui->lineEdit_typev_2->text();
    QDate date_creation = ui->dateEdit_creation_2->date();
    QDate date_expiration = ui->dateEdit_expiration_2->date();
    QString statut = ui->comboBox_status_2->currentText();
    QString certification = ui->lineEdit_certification_2->text();

    qDebug() << "🔍 Avant modification, nomAModifier =" << nomAModifier;

    // 🔴 Désactiver la vérification du nom pendant la modification
    disconnect(ui->lineEdit_nom_2, &QLineEdit::textChanged, this, &MainWindow::verifierNom);

    Vaccin v(0, nom, type, date_creation, date_expiration, statut, certification);

    if (modeModification) {
        if (nomAModifier.isEmpty()) {
            qDebug() << "❌ ERREUR: nomAModifier est vide lors de la modification !";
        }
        // 🔹 Mode Modification : Mettre à jour l'enregistrement existant
        if (v.modifier(nomAModifier)) {
            QMessageBox::information(this, "Succès", "Vaccin modifié avec succès !");
            modeModification = false;  // Désactiver le mode modification
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la modification !");
            return;
        }
    } else {
        // 🔹 Mode Ajout : Ajouter un nouveau vaccin
        qDebug() << "Mode Ajout - Nouveau vaccin";
        if (v.ajouter()) {
            QMessageBox::information(this, "Succès", "Vaccin ajouté avec succès !");
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de l'ajout !");
            return;
        }
    }

    // 🔵 Réactiver la validation après modification
    connect(ui->lineEdit_nom_2, &QLineEdit::textChanged, this, &MainWindow::verifierNom);

    // 🔄 Mise à jour de la table après ajout/modification
    ui->tableView->setModel(v.afficher());
    ui->lineEdit_nom_2->setDisabled(false);
    ui->lineEdit_nom_2->clear();
    ui->lineEdit_typev_2->clear();
    ui->comboBox_status_2->setCurrentIndex(0);
    ui->dateEdit_creation_2->setDate(QDate::currentDate());
    ui->dateEdit_expiration_2->setDate(QDate::currentDate());
    ui->lineEdit_certification_2->clear();
}


void MainWindow::on_pushButton_suppv_clicked() {


    QString nomv = ui->lineEdit->text();

    if (nomv=="") {
        QMessageBox::warning(this, "nom invalide", "Veuillez entrer un nom valide !");
        return;
    }

    // Demander confirmation avant de supprimer
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment supprimer ce vaccin ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        Vaccin v;
        if (v.supprimer(nomv)) {  // Appel de la fonction dans `vaccin.cpp`
            QMessageBox::information(this, "Succès", "Vaccin supprimé avec succès !");
            ui->tableView->setModel(v.afficher());  // Mettre à jour l'affichage
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression du vaccin !");
        }
    }
}
void MainWindow::on_pushButton_modifier_2_clicked() {
    QString nomv = ui->lineEdit->text();

    if (nomv=="") {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un NOM valide !");
        return;
    }
    qDebug() << "📢 Tentative de modification pour le vaccin :" << nomv;
    // Appel de la fonction pour remplir les champs
    remplirChampsModification(nomv);

    // Rediriger vers la page d'ajout
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::remplirChampsModification(QString nomv) {
    QString nom, type, statut, certification;
    QDate date_creation, date_expiration;

    qDebug() << "🔍 Avant récupération, nomAModifier =" << nomAModifier;
    Vaccin v;
    if (v.remplirChampsModification(nomv,nom, type, date_creation, date_expiration, statut, certification)) {
           disconnect(ui->lineEdit_nom_2, &QLineEdit::textChanged, this, &MainWindow::verifierNom);
        // Remplir les champs
        ui->lineEdit_nom_2->setText(nom);
        ui->lineEdit_typev_2->setText(type);
        ui->dateEdit_creation_2->setDate(date_creation);
        ui->dateEdit_expiration_2->setDate(date_expiration);
        ui->comboBox_status_2->setCurrentText(statut);
        ui->lineEdit_certification_2->setText(certification);
        ui->lineEdit_nom_2->setDisabled(true);
        haya=nom;
        nomAModifier = nom;
        modeModification = true;
qDebug() << "✅ Après récupération, nomAModifier =" << nomAModifier;
        connect(ui->lineEdit_nom_2, &QLineEdit::textChanged, this, &MainWindow::verifierNom);
        QMessageBox::information(this, "Modification", "Données chargées, vous pouvez modifier !");
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de charger les données du vaccin !");
    }
    qDebug() << "hedhy variable li zedtha ama f fct lfou9 =" << haya;
}


void MainWindow::on_lineEdit_recherche_2_textChanged(const QString &arg1)
{
    qDebug() << "Recherche en cours... Texte saisi :" << arg1; // Debug

    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    QString searchText = arg1.trimmed(); // Supprime les espaces avant et après

    if (searchText.isEmpty()) {
        // 🔹 Afficher tous les vaccins si aucun texte n'est saisi
        query.prepare("SELECT * FROM VACCIN");
    }
    else {
        // 🔹 Recherche par Nom, Type de vaccin ou Certification
        query.prepare("SELECT * FROM VACCIN WHERE "
                      "LOWER(NOM) LIKE LOWER(:val) OR "
                      "LOWER(TYPE) LIKE LOWER(:val) OR "
                      "LOWER(CERTIFICATION_VACCIN) LIKE LOWER(:val)");
        query.bindValue(":val", "%" + searchText + "%");
    }

    if (!query.exec()) {
        qDebug() << "Erreur SQL :" << query.lastError().text(); // Debug erreur SQL
        return;
    }

    model->setQuery(query);
    ui->tableView->setModel(model);
}

void MainWindow::verifierVaccinsExpires() {
    QSqlQuery query;
    query.prepare("SELECT NOM, DATE_EXPIRATION FROM SMARTVACC.VACCIN WHERE DATE_EXPIRATION < SYSDATE");

    if (!query.exec()) {
        qDebug() << "❌ Erreur lors de la vérification des vaccins expirés :" << query.lastError().text();
        return;
    }

    QString message = "Les vaccins suivants sont expirés :\n";
    bool hasExpired = false;

    while (query.next()) {
        QString nom = query.value(0).toString();
        QDate date_expiration = query.value(1).toDate();
        message += "📌 " + nom + " - Expiré le : " + date_expiration.toString("dd/MM/yyyy") + "\n";
        hasExpired = true;
    }

    if (hasExpired) {
        // 🔹 Créer une notification système
        QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
        // Définir une icône valide pour la barre des tâches
        trayIcon->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
          // Assurez-vous que le fichier existe !
          // Remplacez par votre icône
        trayIcon->show();

        // 🔹 Envoyer la notification
        trayIcon->showMessage("⚠️ Vaccins Expirés", message, QSystemTrayIcon::Warning, 100000);

        qDebug() << "🔔 Notification envoyée pour vaccins expirés.";
    } else {
        qDebug() << "✅ Aucun vaccin expiré.";
    }
}
void MainWindow::updateCovidStats(QString country, int cases, int deaths, int recovered, int population, double vaccinationRate)
{
    QString message = QString(
                          "<div style='background-color: #ffffff; padding: 15px; border-radius: 10px; border: 2px solid #B22222; font-size: 14px; color: #333;'>"
                          "<h3 style='color:#B22222;'>📊 Données COVID-19 pour %1</h3>"
                          "<p><b>✅ Cas confirmés :</b> <span style='color: #008000;'>%2</span></p>"
                          "<p><b>❌ Décès :</b> <span style='color: #FF0000;'>%3</span></p>"
                          "<p><b>💪 Guérisons :</b> <span style='color: #008000;'>%4</span></p>"
                          "<p><b>👥 Population totale :</b> <span style='color: #333;'>%5</span></p>"
                          "<p><b>💉 Taux de vaccination :</b> <span style='color: #B22222;'>%6%</span></p>"
                          "</div>"
                          ).arg(country)
                          .arg(cases)
                          .arg(deaths)
                          .arg(recovered)
                          .arg(population)
                          .arg(vaccinationRate);

    // 🔹 Assurez-vous que predictionLabel supporte HTML
    ui->predictionLabel->setTextFormat(Qt::RichText);
    ui->predictionLabel->setText(message);
    ;

    ui->tab->setCurrentIndex(3);  // ✅ Sélectionne l'onglet Prediction
    ui->predictionLabel->setText(message);  // ✅ Affiche les données dans QLabel

}
void MainWindow::showError(QString error)
{
    QMessageBox::warning(this, "Erreur API", "Impossible de récupérer les données : " + error);
}
void MainWindow::updatePrediction(const QString &predictionResult) {


    ui->tab->setCurrentIndex(3);  // Sélectionne l'onglet "Prediction"
    ui->predictionLabel->setText(predictionResult);  // ✅ Met à jour le texte dans un QLabel

}
void MainWindow::lancerPrediction() {
    QString paysChoisi = ui->comboBoxPays->currentText(); // Récupérer le pays sélectionné
    qDebug() << "📡 Lancement de la prédiction pour : " << paysChoisi;

    // Vérifier si un pays est sélectionné
    if (paysChoisi.isEmpty()) {
        QMessageBox::warning(this, "⚠ Sélectionner un pays", "Veuillez choisir un pays avant de lancer la prédiction.");
        return;
    }

    // Vérifier si `medApi` est bien initialisé avant de l'utiliser
    if (!medApi) {
        qDebug() << "❌ ERREUR : medApi est NULL, impossible de récupérer les données.";
        return;
    }
    // ✅ Correction : Convertir les noms des pays en anglais pour l'API
    QMap<QString, QString> countryMap = {
        {"Tunisie", "Tunisia"}, {"France", "France"}, {"Italie", "Italy"},
        {"Espagne", "Spain"}, {"Allemagne", "Germany"}, {"États-Unis", "USA"},
        {"Canada", "Canada"}, {"Maroc", "Morocco"}, {"Algérie", "Algeria"},
        {"Chine", "China"}
    };

    if (countryMap.contains(paysChoisi)) {
        paysChoisi = countryMap[paysChoisi];  // Convertir en anglais
    }
    qDebug() << "📩 Envoi de la requête à l'API pour : " << paysChoisi;
    medApi->fetchCovidData(paysChoisi); // Envoyer la requête API
}

void MainWindow::verifierNom() {
    QString nom = ui->lineEdit_nom_2->text().trimmed();

    qDebug() << "🔍 Vérification du nom :" << nom;
    qDebug() << "📌 Valeur actuelle de nomAModifier :" << nomAModifier;

    // ✅ Si on est en mode modification et que le nom n'a pas changé, ne rien faire
    if (modeModification) {
        qDebug() << "✅ Mode modification actif, validation du nom ignorée.";
        return;
    }

    if (nom.isEmpty()) {
        ui->labelErrorNom->setText("⚠ Le nom ne peut pas être vide !");
        ui->labelErrorNom->setStyleSheet("color: red; background-color: transparent; border: none; font-weight: normal;");
    }
    else if (nom.length() < 3) {
        ui->labelErrorNom->setText("⚠ Le nom doit contenir au moins 3 caractères !");
        ui->labelErrorNom->setStyleSheet("color: red; background-color: transparent; border: none; font-weight: normal;");
    }
    else if (nomExisteDeja(nom)) {
        ui->labelErrorNom->setText("⚠ Ce nom de vaccin existe déjà !");
        ui->labelErrorNom->setStyleSheet("color: red; background-color: transparent; border: none; font-weight: normal;");
    }
    else {
        ui->labelErrorNom->clear();
        ui->labelErrorNom->setStyleSheet("color: transparent; background-color: transparent; border: none; font-weight: normal;");
    }
}




bool MainWindow::nomExisteDeja(const QString &nom) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM SMARTVACC.VACCIN WHERE LOWER(NOM) = LOWER(:nom)");
    query.bindValue(":nom", nom);

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL lors de la vérification du nom :" << query.lastError().text();
        return false; // En cas d'erreur SQL, on ne bloque pas
    }

    query.next();
    int count = query.value(0).toInt();
    return (count > 0);
}
void MainWindow::verifierTypeVaccin() {
    QString type = ui->lineEdit_typev_2->text().trimmed();

    // 🔴 Si vide
    if (type.isEmpty()) {
        ui->labelErrorType->setText("⚠ Le type de vaccin ne peut pas être vide !");
        ui->labelErrorType->setStyleSheet("color: red; background-color: transparent; border: none; font-weight: normal;");
    }
    // 🔴 Si moins de 3 caractères
    else if (type.length() < 3) {
        ui->labelErrorType->setText("⚠ Le type de vaccin doit contenir au moins 3 caractères !");
        ui->labelErrorType->setStyleSheet("color: red; background-color: transparent; border: none; font-weight: normal;");
    }
    // 🔴 Si contient des caractères interdits (ex: chiffres, symboles)
    else if (!type.contains(QRegularExpression("^[a-zA-ZÀ-ÖØ-öø-ÿ\\s]+$"))) {
        ui->labelErrorType->setText("⚠ Seuls les lettres et espaces sont autorisés !");
        ui->labelErrorType->setStyleSheet("color: red; background-color: transparent; border: none; font-weight: normal;");
    }
    // ✅ Si tout est bon
    else {
        ui->labelErrorType->clear();
        ui->labelErrorType->setStyleSheet("color: transparent; background-color: transparent; border: none; font-weight: normal;");
    }
}
void MainWindow::verifierCertificationVaccin() {
    QString certification = ui->lineEdit_certification_2->text().trimmed();

    // 🔴 Si vide
    if (certification.isEmpty()) {
        ui->labelErrorCertification->setText("⚠ La certification ne peut pas être vide !");
        ui->labelErrorCertification->setStyleSheet("color: red; background-color: transparent; border: none; font-weight: normal;");
    }
    // 🔴 Si moins de 5 caractères
    else if (certification.length() < 5) {
        ui->labelErrorCertification->setText("⚠ La certification doit contenir au moins 5 caractères !");
        ui->labelErrorCertification->setStyleSheet("color: red; background-color: transparent; border: none; font-weight: normal;");
    }
    // 🔴 Si contient des caractères interdits (ex: chiffres, symboles autres que `-`)
    else if (!certification.contains(QRegularExpression("^[a-zA-ZÀ-ÖØ-öø-ÿ\\s-]+$"))) {
        ui->labelErrorCertification->setText("⚠ Seuls les lettres, espaces et tirets sont autorisés !");
        ui->labelErrorCertification->setStyleSheet("color: red; background-color: transparent; border: none; font-weight: normal;");
    }
    // ✅ Si tout est bon
    else {
        ui->labelErrorCertification->clear();
        ui->labelErrorCertification->setStyleSheet("color: transparent; background-color: transparent; border: none; font-weight: normal;");
    }
}


/*void MainWindow::on_pushButton_annuler_2_clicked()
{
        if (modeModification) {
            // 🔹 Si on est en mode modification, remettre les anciennes valeurs
            QSqlQuery query;
            query.prepare("SELECT CIN, NOM, PRENOM, AGE, SEXE, NUM, POIDS, DATE_RDV, REMARQUES, STATUT_VACCINAL FROM CARNETS WHERE CIN = :cin");
            query.bindValue(":nomAModifier", nomAModifier);

            if (query.exec() && query.next()) {
                // ✅ Remettre toutes les anciennes valeurs, y compris le CIN
                ui->lineEdit_nom_2->setText(nom);
                ui->lineEdit_typev_2->setText(type);
                ui->dateEdit_creation_2->setDate(date_creation);
                ui->dateEdit_expiration_2->setDate(date_expiration);
                ui->comboBox_status_2->setCurrentText(statut);
                ui->lineEdit_certification_2->setText(certification);
            }
        } else {
            // 🔹 Si on est en mode ajout, vider les champs
            ui->cin->clear();
            ui->nom_carnet->clear();
            ui->prenom_carnet->clear();
            ui->age->clear();
            ui->num->clear();
            ui->poids->clear();
            ui->date_rdv->setDate(QDate::currentDate());
            ui->remarques->clear();
            ui->statut_vaccinal->setCurrentIndex(0);
        }

}
*/
