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
#include <QTimer>
#include <QTextDocument>
#include <QFileDialog>
#include <QtPrintSupport/QPrinter>
#include <QTextDocument>  // ✅ Correct
#include <QDate>
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

    // ✅ Désactiver la vérification si on est en mode modification
    if (!modeModification && (!ui->labelErrorNom->text().isEmpty() ||
                              !ui->labelErrorType->text().isEmpty() ||
                              !ui->labelErrorCertification->text().isEmpty())) {
        QMessageBox::warning(this, "Erreur", "Veuillez corriger les erreurs avant de continuer !");
        return;
    }

    // 🔴 Vérification que le statut est valide
    if (statut != "effectuer" && statut != "en cour") {
        QMessageBox::warning(this, "Erreur", "Veuillez choisir un statut valide !");
        return;
    }

    // Vérifier que la date d'expiration est après la date de création
    if (date_expiration <= date_creation) {
        QMessageBox::warning(this, "Date invalide", "La date d'expiration doit être après la date de création !");
        return;
    }

    Vaccin v(0, nom, type, date_creation, date_expiration, statut, certification);

    if (modeModification) {

        if (nomAModifier.isEmpty()) {
            qDebug() << "❌ ERREUR: nomAModifier est vide lors de la modification !";
        }
        // 🔹 Mode Modification : Mettre à jour l'enregistrement existant
        if (v.modifier(nomAModifier)) {
            QMessageBox::information(this, "Succès", "Vaccin modifié avec succès !");
            modeModification = false;  // Désactiver le mode modification après enregistrement
            connect(ui->lineEdit_nom_2, &QLineEdit::textChanged, this, &MainWindow::verifierNom); // Réactiver la vérification

        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la modification !");
            return;
        }
    } else {
        // 🔹 Mode Ajout : Ajouter un nouveau vaccin
        qDebug() << "Mode Ajout - Nouveau vaccin";

        if (v.ajouter()) {
            QMessageBox::information(this, "Succès", "Vaccin ajouté avec succès !");
            // ✅ Redirection forcée avec QMetaObject::invokeMethod
            QMetaObject::invokeMethod(this, [=]() {
                qDebug() << "🔄 Forçage de la redirection vers la page d'ajout";
                ui->stackedWidget->setCurrentIndex(3);
                ui->tab->setCurrentIndex(1); // 0 correspond à l'onglet "Ajout"

                QApplication::processEvents(); // ✅ Force l'UI à traiter les événements
                qDebug() << "📌 Vérification après redirection : Page active =" << ui->stackedWidget->currentIndex();

            }, Qt::QueuedConnection);
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de l'ajout !");
            return;
        }
    }

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
    QString nomv = ui->lineEdit->text().trimmed();

    if (nomv.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un NOM valide !");
        return;
    }

    // 🔍 Vérifier si le vaccin existe avant de tenter de le supprimer
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM VACCIN WHERE NOM = :nom");
    query.bindValue(":nom", nomv);

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL lors de la vérification de l'existence du vaccin :" << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Erreur lors de la vérification de l'existence du vaccin !");
        return;
    }

    query.next();
    int count = query.value(0).toInt();

    if (count == 0) {
        QMessageBox::warning(this, "Nom inexistant", "⚠️ Le vaccin avec ce nom n'existe pas !");
        return;
    }

    // 🗑️ Demander confirmation avec "Oui" et "Non"
    QMessageBox confirmationBox;
    confirmationBox.setWindowTitle("Confirmation");
    confirmationBox.setText("Voulez-vous vraiment supprimer ce vaccin ?");
    QPushButton *ouiButton = confirmationBox.addButton("Oui", QMessageBox::YesRole);
    QPushButton *nonButton = confirmationBox.addButton("Non", QMessageBox::NoRole);
    confirmationBox.exec();

    if (confirmationBox.clickedButton() == ouiButton) {
        Vaccin v;
        if (v.supprimer(nomv)) {  // ✅ Suppression si le vaccin existe
            QMessageBox::information(this, "Succès", "Vaccin supprimé avec succès !");
            ui->tableView->setModel(v.afficher());  // 🔄 Mettre à jour l'affichage
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression du vaccin !");
        }
    }
}

    void MainWindow::on_pushButton_modifier_2_clicked() {
        QString nomv = ui->lineEdit->text().trimmed();

        if (nomv.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez entrer un NOM valide !");
            return;
        }

        // 🔍 Vérifier si le vaccin existe avant de permettre la modification
        QSqlQuery query;
        query.prepare("SELECT COUNT(*) FROM VACCIN WHERE NOM = :nom");
        query.bindValue(":nom", nomv);

        if (!query.exec()) {
            qDebug() << "❌ Erreur SQL lors de la vérification de l'existence du vaccin :" << query.lastError().text();
            QMessageBox::critical(this, "Erreur", "Erreur lors de la vérification du vaccin !");
            return;
        }

        query.next();
        int count = query.value(0).toInt();

        if (count == 0) {
            QMessageBox::warning(this, "Nom inexistant", "⚠️ Le vaccin avec ce nom n'existe pas !");
            return;
        }

        // 📌 Si le vaccin existe, on remplit les champs pour modification
        qDebug() << "📢 Tentative de modification pour le vaccin :" << nomv;
        remplirChampsModification(nomv);

        // 🔄 Redirection vers la page d'ajout/modification
        ui->stackedWidget->setCurrentIndex(3);
    }





    void MainWindow::remplirChampsModification(QString nomv) {
        QString nom, type, statut, certification;
        QDate date_creation, date_expiration;

        qDebug() << "🔍 Avant récupération, nomAModifier =" << nomAModifier;
        Vaccin v;
        if (v.remplirChampsModification(nomv, nom, type, date_creation, date_expiration, statut, certification)) {
            disconnect(ui->lineEdit_nom_2, &QLineEdit::textChanged, this, &MainWindow::verifierNom);

            // Remplir les champs
            ui->lineEdit_nom_2->setText(nom);
            ui->lineEdit_typev_2->setText(type);
            ui->dateEdit_creation_2->setDate(date_creation);
            ui->dateEdit_expiration_2->setDate(date_expiration);
            ui->comboBox_status_2->setCurrentText(statut);
            ui->lineEdit_certification_2->setText(certification);
            ui->lineEdit_nom_2->setDisabled(true);
            ancienType = type;
            ancienneDateCreation = date_creation;
            ancienneDateExpiration = date_expiration;
            ancienStatut = statut;
            ancienneCertification = certification;

            nomAModifier = nom;
            modeModification = true;

            qDebug() << "✅ Après récupération, nomAModifier =" << nomAModifier;

            // ✅ Affichage du message d'information
            QMessageBox::information(this, "Modification", "Données chargées, vous pouvez modifier !");

            // ✅ Redirection forcée avec QMetaObject::invokeMethod
            QMetaObject::invokeMethod(this, [=]() {
                qDebug() << "🔄 Forçage de la redirection vers la page d'ajout";
                ui->stackedWidget->setCurrentIndex(3);
                ui->tab->setCurrentIndex(0); // 0 correspond à l'onglet "Ajout"

                QApplication::processEvents(); // ✅ Force l'UI à traiter les événements
                qDebug() << "📌 Vérification après redirection : Page active =" << ui->stackedWidget->currentIndex();

            }, Qt::QueuedConnection);


        } else {
            QMessageBox::critical(this, "Erreur", "Impossible de charger les données du vaccin !");
        }
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

    // ✅ Désactiver la vérification si on est en mode modification
    if (modeModification) {
        qDebug() << "✅ Mode modification actif, validation du nom ignorée.";
        ui->labelErrorNom->clear();
        ui->labelErrorNom->setStyleSheet("color: transparent; background-color: transparent; border: none;");
        return;  // ⛔ Quitter immédiatement la fonction
    }

    if (nom.isEmpty()) {
        ui->labelErrorNom->setText("⚠ Le nom ne peut pas être vide !");
        ui->labelErrorNom->setStyleSheet("color: red; background-color: transparent; border: none;");
    }
    else if (nom.length() < 3) {
        ui->labelErrorNom->setText("⚠ Le nom doit contenir au moins 3 caractères !");
        ui->labelErrorNom->setStyleSheet("color: red; background-color: transparent; border: none;");
    }
    else if (nomExisteDeja(nom)) {
        ui->labelErrorNom->setText("⚠ Ce nom de vaccin existe déjà !");
        ui->labelErrorNom->setStyleSheet("color: red; background-color: transparent; border: none;");
    }
    else {
        ui->labelErrorNom->clear();
        ui->labelErrorNom->setStyleSheet("color: transparent; background-color: transparent; border: none;");
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

void MainWindow::on_pushButton_annuler_2_clicked()
{
    if (modeModification) {
        qDebug() << "🔄 Annulation en mode modification - Restauration des anciennes valeurs.";

        // 🔹 Remettre les anciennes valeurs
        ui->lineEdit_nom_2->setText(nomAModifier);
        ui->lineEdit_typev_2->setText(ancienType);
        ui->dateEdit_creation_2->setDate(ancienneDateCreation);
        ui->dateEdit_expiration_2->setDate(ancienneDateExpiration);
        ui->comboBox_status_2->setCurrentText(ancienStatut);
        ui->lineEdit_certification_2->setText(ancienneCertification);

        QMessageBox::information(this, "Annulation", "Les valeurs avant modification ont été restaurées.");
    } else {
        qDebug() << "🧹 Annulation en mode ajout - Effacement des champs.";

        // 🔹 Vider tous les champs
        ui->lineEdit_nom_2->clear();
        ui->lineEdit_typev_2->clear();
        ui->comboBox_status_2->setCurrentIndex(0);
        ui->dateEdit_creation_2->setDate(QDate::currentDate());
        ui->dateEdit_expiration_2->setDate(QDate::currentDate());
        ui->lineEdit_certification_2->clear();

        QMessageBox::information(this, "Annulation", "Les champs ont été effacés.");
    }
}




void MainWindow::on_pushButton_pdf_2_clicked() {
    QString nomVaccin = ui->lineEdit->text().trimmed();

    if (nomVaccin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un NOM valide pour le vaccin !");
        return;
    }

    // 🔍 Rechercher les détails du vaccin dans la base de données
    QSqlQuery query;
    query.prepare("SELECT NOM, TYPE, DATE_CREATION, DATE_EXPIRATION, CERTIFICATION_VACCIN, STATUT "
                  "FROM SMARTVACC.VACCIN WHERE NOM = :nom");
    query.bindValue(":nom", nomVaccin);

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL lors de la récupération du vaccin :" << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer les informations du vaccin !");
        return;
    }

    if (!query.next()) {
        QMessageBox::warning(this, "Erreur", "Le vaccin avec ce nom n'existe pas !");
        return;
    }

    // ✅ Récupérer les informations
    QString type = query.value("TYPE").toString();
    QDate dateCreation = query.value("DATE_CREATION").toDate();
    QDate dateExpiration = query.value("DATE_EXPIRATION").toDate();
    QString certification = query.value("CERTIFICATION_VACCIN").toString();
    QString statut = query.value("STATUT").toString();

    // 🏥 Définition du statut du vaccin
    QString etatVaccin;
    QString colorVaccin;
    if (dateExpiration < QDate::currentDate()) {
        etatVaccin = "⚠ Expiré ❌";
        colorVaccin = "red";
    } else if (dateExpiration <= QDate::currentDate().addMonths(3)) {
        etatVaccin = "⚠ Expiration Proche";
        colorVaccin = "orange";
    } else {
        etatVaccin = "✅ Valide";
        colorVaccin = "green";
    }

    // 📌 Définition du statut de la certification
    QString etatCertification = certification.isEmpty() ? "Non certifié ❌" : "Certifié ✅";
    QString colorCertification = certification.isEmpty() ? "red" : "green";

    // 📂 Demander où enregistrer le fichier PDF
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer le fichier PDF",
                                                    nomVaccin + "_certification.pdf",
                                                    "Fichiers PDF (*.pdf)");

    if (filePath.isEmpty()) {
        return; // L'utilisateur a annulé
    }

    // 🖨️ Configuration de l'impression vers PDF
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setPageSize(QPageSize(QPageSize::A4));

    // 📝 Création du document PDF
    QTextDocument document;
    QString html = QString(
                       "<h1 style='color:#B22222; text-align:center;'>🩺 Rapport de Certification du Vaccin</h1>"
                       "<hr style='border:1px solid #B22222;'>"
                       "<h2 style='color:#333;'>📌 Détails du Vaccin</h2>"
                       "<p><b>🆔 Nom :</b> %1</p>"
                       "<p><b>🧬 Type :</b> %2</p>"
                       "<p><b>📅 Date de Création :</b> %3</p>"
                       "<p><b>⏳ Date d'Expiration :</b> %4</p>"
                       "<p><b>🩹 Statut :</b> %5</p>"
                       "<h2 style='color:#333;'>🏥 Certification</h2>"
                       "<p><b>📜 Nom du Certificat :</b> %6</p>"
                       "<p style='color:%7;'><b>🔍 État de la Certification :</b> %8</p>"
                       "<h2 style='color:#333;'>🔎 Analyse de l'État du Vaccin</h2>"
                       "<p style='color:%9;'><b>⚠ Statut :</b> %10</p>"
                       "<h2 style='color:#333;'>📊 Recommandations et Avis Médical</h2>"
                       "<p>🔵 Ce vaccin est recommandé pour une utilisation jusqu'à sa date d'expiration.</p>"
                       "<p>⚠ Si le vaccin est expiré, ne pas utiliser et éliminer selon les règles sanitaires.</p>"
                       "<p>🩺 Consultez un professionnel de santé pour toute question sur ce vaccin.</p>"
                       "<hr style='border:1px solid #B22222;'>"
                       "<p style='text-align:center;'>📅 Rapport généré le %11</p>"
                       ).arg(nomVaccin)
                       .arg(type)
                       .arg(dateCreation.toString("dd/MM/yyyy"))
                       .arg(dateExpiration.toString("dd/MM/yyyy"))
                       .arg(statut)
                       .arg(certification.isEmpty() ? "Non disponible" : certification)
                       .arg(colorCertification)
                       .arg(etatCertification)
                       .arg(colorVaccin)
                       .arg(etatVaccin)
                       .arg(QDate::currentDate().toString("dd/MM/yyyy"));

    document.setHtml(html);
    document.print(&printer);

    // 🔔 Message de succès
    QMessageBox::information(this, "Succès", "Le rapport de certification a été généré avec succès !");
}
