#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QHBoxLayout>  // Pour QHBoxLayout
#include <QPushButton>  // Pour QPushButton
#include <QWidget>      // Pour QWidget
#include <QSqlQuery>
#include <QSqlError> // Pour afficher les erreurs SQL
#include <QDebug>    // Pour le débogage
#include <QRegularExpression>
#include <QPainter>
#include <QPdfWriter>
#include <QFileDialog>

#include <QDateTime>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   // envoyerRappelSMS();



    connect(ui->btnPredire, &QPushButton::clicked, this, &MainWindow::on_btnPredire_clicked);

    connect(ui->nom_carnet, &QLineEdit::textChanged, this, [=]() {
        validateInput(ui->nom_carnet, ui->nomErrorLabel, QRegularExpression("^[A-Za-zÀ-ÿ\\s-]+$"),
                      "Le nom ne doit contenir que des lettres et espaces.", false, false, -1, -1);
    });

    connect(ui->prenom_carnet, &QLineEdit::textChanged, this, [=]() {
        validateInput(ui->prenom_carnet, ui->prenomErrorLabel, QRegularExpression("^[A-Za-zÀ-ÿ\\s-]+$"),
                      "Le prénom ne doit contenir que des lettres et espaces.", false, false, -1, -1);
    });

    connect(ui->cin, &QLineEdit::textChanged, this, [=]() {
        validateInput(ui->cin, ui->cinErrorLabel, QRegularExpression("^[0-9]{8}$"),
                      "Le CIN doit contenir exactement 8 chiffres.", true, true, -1, -1);
    });

    connect(ui->num, &QLineEdit::textChanged, this, [=]() {
        validateInput(ui->num, ui->numErrorLabel, QRegularExpression("^[0-9]{8}$"),
                      "Le numéro doit contenir exactement 8 chiffres.", true, true, -1, -1);
    });

    connect(ui->poids, &QLineEdit::textChanged, this, [=]() {
        validateInput(ui->poids, ui->poidsErrorLabel, QRegularExpression("^[0-9]{1,3}(\\.[0-9]{1,2})?$"),
                      "Le poids doit être un nombre valide.", false, true, -1, -1);
    });

    connect(ui->age, &QLineEdit::textChanged, this, [=]() {
        validateInput(ui->age, ui->ageErrorLabel, QRegularExpression("^[0-9]{1,2}$"),
                      "L'âge doit être entre 1 et 100.", false, true, 1, 100);
    });


    connect(ui->remarques, &QTextEdit::textChanged, this, [=]() {
        validateTextEdit(ui->remarques, ui->remarquesErrorLabel, QRegularExpression("^.{0,200}$"), "Les remarques doivent contenir au moins 2 mots sans chiffres ni caractères spéciaux .");
    });

    connect(ui->annuler, &QPushButton::clicked, this, &MainWindow::on_annuler_clicked);
    connect(ui->btnGeneratePDF, &QPushButton::clicked, this, &MainWindow::on_btnGeneratePDF_clicked);


    ui->stackedWidget->setCurrentIndex(6);
    ui->frame->setVisible(false);
    connect(ui->carnet, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(2);
        ui->frame->setVisible(true);
        displayCarnet();
    });

    // Function to handle page changes
    auto updateSidebarVisibility = [=](int index) {
        if (index == 6) { //
            ui->frame->setVisible(false);
        } else {
            ui->frame->setVisible(true);
        }
    };


    // Back to login button
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
    // Bouton "Employé" -> Page 0 (pageEmp)
    connect(ui->employe, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(0);
        ui->frame->setVisible(true);
    });

    // Bouton "Produits" -> Page 1 (pageProd)
    connect(ui->produits, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(1);
        ui->frame->setVisible(true);
    });

    // Bouton "Laboratoire" -> Page 2 (pageLabo)
    connect(ui->labo, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(2);
        ui->frame->setVisible(true);
    });

    // Bouton "Vaccin" -> Page 3 (pageVac)
    connect(ui->vaccin, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(3);
        ui->frame->setVisible(true);
    });

    // Bouton "Carnet" -> Page 4 (pageCar)
    connect(ui->carnet, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(5);
    });

    // Bouton "Campagne" -> Page 5 (pageComp)
    connect(ui->compagne, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(4);
        ui->frame->setVisible(true);
    });
    //testSMS();

}

MainWindow::~MainWindow()
{
    delete ui;
}





void MainWindow::envoyerRappelSMS()
{
    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "❌ Base de données non ouverte!";
        return;  // Retourne sans faire la requête
    }

    QSqlQuery query;
    query.prepare("SELECT NUM, DATE_RDV FROM CARNETS");

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL lors de la récupération des RDV :" << query.lastError().text();
        return;
    }

    SmsNotif sms;
    QDateTime maintenant = QDateTime::currentDateTime();
    QDateTime limite = maintenant.addSecs(48 * 3600); // 🔔 48 heures

    while (query.next()) {
        QString brut = query.value(0).toString(); // ce qui vient de la base
        QDateTime dateRdv = query.value(1).toDateTime();

        if (dateRdv > maintenant && dateRdv <= limite) {
            QString numero = brut;

            if (!numero.startsWith("+216")) {
                numero = "+216" + numero;
            }

            qDebug() << "📲 Numéro original :" << brut;
            qDebug() << "📲 Numéro final pour Twilio :" << numero;

            QString message = "📅 Rappel : Votre RDV est prévu le " + dateRdv.toString("dd/MM/yyyy à HH:mm") + ".";
            sms.sendSMS(numero, message);

            qDebug() << "📩 SMS de rappel envoyé à :" << numero;
        }
    }

}





void MainWindow::on_rechercheC_textChanged(const QString &arg1)
{
    qDebug() << "Recherche en cours... Texte saisi :" << arg1; // Debug

    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    QString searchText = arg1.trimmed(); // Enlever les espaces avant et après

    if (searchText.isEmpty()) {
        query.prepare("SELECT * FROM CARNETS");
    }
    else if (searchText.toInt()) { // Vérifie si c'est un CIN
        query.prepare("SELECT * FROM CARNETS WHERE CIN LIKE :val");
        query.bindValue(":val", searchText + "%");
    }
    else if (searchText.compare("vaccine", Qt::CaseInsensitive) == 0 ||
             searchText.compare("non_vaccine", Qt::CaseInsensitive) == 0) {
        query.prepare("SELECT * FROM CARNETS WHERE LOWER(STATUT_VACCINAL) = LOWER(:val)");
        query.bindValue(":val", searchText);
    }
    else { // Recherche par nom ou prénom
        query.prepare("SELECT * FROM CARNETS WHERE LOWER(NOM) LIKE LOWER(:val) OR LOWER(PRENOM) LIKE LOWER(:val)");
        query.bindValue(":val", "%" + searchText + "%");
    }

    if (!query.exec()) {
        qDebug() << "Erreur SQL :" << query.lastError().text(); // Debug erreur
        return;
    }

    model->setQuery(query);
    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0); // 🔹 Masquer la première colonne (ID)

}




void MainWindow::on_ajout_carnet_clicked()
{
    if (!estValide()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Erreur");
        msgBox.setText("Veuillez corriger les erreurs avant de valider le carnet.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setButtonText(QMessageBox::Ok, "D'accord");
        msgBox.exec();
        return;
    }


    QString cin = ui->cin->text().trimmed();
    QString nom = ui->nom_carnet->text().trimmed();
    QString prenom = ui->prenom_carnet->text().trimmed();
    int age = ui->age->text().toInt();
    QString num = ui->num->text().trimmed();
    float poids = ui->poids->text().toFloat();
    QDate date_rdv = ui->date_rdv->date();
    QString remarques = ui->remarques->toPlainText().trimmed();
    QString statut_vaccinal = ui->statut_vaccinal->currentText();
    QString sexe;
    if (ui->G->isChecked()) {
        sexe = "Garçon";
    } else if (ui->F->isChecked()) {
        sexe = "Femme";
    } else {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un sexe !");


        return;
    }

    if (cin.isEmpty() || nom.isEmpty() || prenom.isEmpty() || num.isEmpty() || age <= 0) {
        QMessageBox::warning(this, "Champs vides", "Veuillez remplir tous les champs obligatoires.");
        return;
    }

    Carnets carnet(0, cin, nom, prenom, age, sexe, num, poids, date_rdv, remarques, statut_vaccinal);

    if (modeModification) {
        if (idAModifier.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Aucun CIN sélectionné pour modification !");
            return;
        }
        if (!estValide()) {
            QMessageBox::warning(this, "Erreur", "Veuillez corriger les erreurs avant de modifier le carnet.");
            return; //  Bloquer la modification si les champs ne sont pas valides
        }
        qDebug() << "Modification du carnet avec CIN :" << idAModifier;

        carnetTmp.supprimer(idAModifier);

        if (carnet.ajouter()) {
            QMessageBox::information(this, "Succès", "Carnet modifié avec succès !");
            ui->tabs->setCurrentIndex(1);  // Rediriger vers l'onglet Affichage

        } else {
            QMessageBox::critical(this, "Erreur", "Le CIN existe déjà ! Impossible de modifier");
            return;
        }

        modeModification = false;
        idAModifier = "";
    } else {
        if (carnet.ajouter()) {
            QMessageBox::information(this, "Succès", "Carnet ajouté avec succès !");
            ui->tabs->setCurrentIndex(1);  // Rediriger vers l'onglet Affichage

        } else {
            QMessageBox::critical(this, "Erreur", "Le CIN existe déjà ! Impossible d'ajouter");
            return;
        }
    }

    displayCarnet();

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

// 🔹 Afficher la liste des carnets
void MainWindow::displayCarnet()
{
    QSqlQueryModel *model = carnetTmp.afficher();
    if (model) {
        ui->tableView->setModel(model);
        ui->tableView->resizeColumnsToContents();
    } else {
        QMessageBox::warning(this, "Erreur", "Échec du chargement des carnets.");
    }
}

void MainWindow::on_supprimerC_clicked()
{
    QString cin = ui->suppid->text().trimmed(); // Récupérer le CIN

    qDebug() << "🔍 CIN saisi pour suppression :" << cin;  //  Vérification du CIN

    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un CIN valide !");
        return;
    }

    // Vérifier si le CIN existe avant suppression
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM CARNETS WHERE CIN = :cin");
    checkQuery.bindValue(":cin", cin);

    if (!checkQuery.exec() || !checkQuery.next()) {
        qDebug() << "❌ Erreur SQL lors de la vérification de l'existence du carnet :" << checkQuery.lastError().text();
        QMessageBox::critical(this, "Erreur", "Impossible de vérifier l'existence du carnet !");
        return;
    }

    int count = checkQuery.value(0).toInt();
    if (count == 0) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Erreur");
        msgBox.setText("Aucun carnet trouvé avec ce CIN !");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setButtonText(QMessageBox::Ok, "D'accord");
        msgBox.exec();
        return;
    }


    // confirmation avec boutons "Oui" et "Non"
    QMessageBox msgBox;
    msgBox.setWindowTitle("Confirmation");
    msgBox.setText("Voulez-vous vraiment supprimer ce carnet ?");
    QPushButton *btnOui = msgBox.addButton("Oui", QMessageBox::YesRole);
    QPushButton *btnNon = msgBox.addButton("Non", QMessageBox::NoRole);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.exec();

    // Vérifier quel bouton a été cliqué
    if (msgBox.clickedButton() == btnOui) {
        if (carnetTmp.supprimer(cin)) {
            QMessageBox::information(this, "Succès", "Carnet supprimé avec succès !");
            displayCarnet();
        } else {
            qDebug() << "❌ Erreur lors de la suppression :" << checkQuery.lastError().text();
            QMessageBox::critical(this, "Erreur", "Échec de la suppression !");
        }
    }
}


void MainWindow::on_modifierC_clicked()
{

    QString cin = ui->suppid->text().trimmed();

    qDebug() << "🔍 CIN sélectionné pour modification :" << cin;  // 🔍 Vérification de la valeur entrée

    //  Vérifier si le champ CIN est vide
    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un CIN valide !");
        return;
    }

    //  Vérifier si le CIN existe dans la base de données avant modification
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM CARNETS WHERE CIN = :cin");
    checkQuery.bindValue(":cin", cin);

    if (!checkQuery.exec() || !checkQuery.next()) {
        qDebug() << "❌ Erreur SQL lors de la vérification de l'existence du carnet :" << checkQuery.lastError().text();
        QMessageBox::critical(this, "Erreur", "Impossible de vérifier l'existence du carnet !");
        return;
    }

    int count = checkQuery.value(0).toInt();
    if (count == 0) {
        QMessageBox::warning(this, "Erreur", "Aucun carnet trouvé avec ce CIN !");
        return;
    }

    //  Charger les informations du carnet si le CIN existe
    QSqlQuery query;
    query.prepare("SELECT CIN,NOM, PRENOM, AGE, SEXE, NUM, POIDS, DATE_RDV, REMARQUES, STATUT_VACCINAL FROM CARNETS WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    if (query.exec() && query.next()) {
        //  Remplir les champs avec les valeurs existantes

        ui->cin->setText(query.value("CIN").toString());

        ui->nom_carnet->setText(query.value("NOM").toString());
        ui->prenom_carnet->setText(query.value("PRENOM").toString());
        ui->age->setText(query.value("AGE").toString());
        ui->G->setChecked(query.value("SEXE").toString() == "Garçon");
        ui->num->setText(query.value("NUM").toString());
        ui->poids->setText(query.value("POIDS").toString());
        ui->date_rdv->setDate(query.value("DATE_RDV").toDate());
        ui->remarques->setPlainText(query.value("REMARQUES").toString());
        ui->statut_vaccinal->setCurrentText(query.value("STATUT_VACCINAL").toString());

        //  Activer le mode modification
        idAModifier = cin;
        modeModification = true;

        QMessageBox::information(this, "Modification", "Données chargées, vous pouvez modifier !");
        ui->tabs->setCurrentIndex(0);  // 🔄 Rediriger vers l'onglet Ajout

    } else {

        qDebug() << "❌ Erreur SQL lors de la récupération des données :" << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Impossible de charger les données du carnet !");
    }
}



void MainWindow::on_btnPredire_clicked()
{
    QString cin = ui->idPredictionInput->text().trimmed(); // ✅ Récupérer le CIN depuis l'interface

    qDebug() << "🔍 CIN sélectionné pour la prédiction :" << cin;  // 🔍 Vérification

    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un CIN valide !");
        return;
    }

    // 🔴 Vérifier si le CIN existe dans la base de données avant la prédiction
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM CARNETS WHERE CIN = :cin");
    checkQuery.bindValue(":cin", cin);

    if (!checkQuery.exec() || !checkQuery.next()) {
        qDebug() << "❌ Erreur SQL lors de la vérification de l'existence du carnet :" << checkQuery.lastError().text();
        QMessageBox::critical(this, "Erreur", "Impossible de vérifier l'existence du carnet !");
        return;
    }

    int count = checkQuery.value(0).toInt();
    if (count == 0) {
        QMessageBox::warning(this, "Erreur", "Aucun carnet trouvé avec ce CIN !");
        return;
    }

    // ✅ Récupérer les informations pour la prédiction
    QString prediction = carnetTmp.analyserRisqueParCIN(cin);  // ✅ Nouvelle méthode basée sur CIN
    ui->predictionResult->setText(prediction);

    // 🔹 Extraire l'alerte vaccinale depuis la prédiction
    QString alerteVaccin = "";
    if (prediction.contains("🔔 Alerte Vaccinale")) {
        int index = prediction.indexOf("🔔 Alerte Vaccinale");
        alerteVaccin = prediction.mid(index); // Extraire le texte de l'alerte
    }

    // 🔥 Afficher une ALERTE avec un QMessageBox selon le niveau de risque
    if (prediction.contains("🚨 Critique")) {
        QMessageBox::critical(this, "🚨 ALERTE MÉDICALE !",
                              "⚠️ Risque critique détecté !\n\n" + alerteVaccin +
                                  "\n\n❗ Consultez un médecin immédiatement !");
    }
    else if (prediction.contains("⚠️ Élevé")) {
        QMessageBox::warning(this, "⚠️ ATTENTION",
                             "🔸 Risque élevé identifié.\n\n" + alerteVaccin +
                                 "\n\n📢 Surveillez votre état de santé de près.");
    }
    else if (prediction.contains("🟡 Modéré")) {
        QMessageBox::information(this, "🟡 Précaution",
                                 "🟡 Risque modéré.\n\n" + alerteVaccin +
                                     "\n\n🔹 Prenez soin de votre santé.");
    }
    else {
        QMessageBox::information(this, "✅ Aucun risque majeur",
                                 "✅ Vous n’avez pas de problème de santé majeur.\n\n" + alerteVaccin);
    }
}
void MainWindow::validateInput(QLineEdit *field, QLabel *errorLabel, QRegularExpression regex,
                               const QString &errorMsg, bool checkZeros, bool allowOnlyNumbers, int minValue, int maxValue)
{
    QString text = field->text().trimmed();

    // 🔴 Blocage immédiat des lettres pour CIN, Numéro, Poids et Âge
    if (allowOnlyNumbers && text.contains(QRegularExpression("[A-Za-z]"))) {
        text.chop(1); // Supprime le dernier caractère saisi
        field->setText(text);
        return;
    }

    // 🔴 Blocage immédiat des chiffres pour Nom et Prénom
    if (!allowOnlyNumbers && text.contains(QRegularExpression("\\d"))) {
        text.chop(1); // Supprime le dernier caractère saisi
        field->setText(text);
        return;
    }

    // 🔴 Vérifie si le champ contient uniquement des "0" (CIN et Numéro)
    if (checkZeros && text == "00000000") {
        field->setStyleSheet("border: 2px solid red; padding: 5px; background: white;");
        errorLabel->setText("❌ Tous les chiffres ne peuvent pas être 0 !");
        errorLabel->setStyleSheet("color: red; font-weight: bold; background: transparent;");
        return;
    }

    // 🔹 Vérifie les valeurs minimales et maximales (ex: Âge)
    if (minValue >= 0 && maxValue > 0) {
        bool isNumber;
        int value = text.toInt(&isNumber);
        if (!isNumber || value < minValue || value > maxValue) {
            field->setStyleSheet("border: 2px solid red; padding: 5px; background: white;");
            errorLabel->setText("❌ " + errorMsg);
            errorLabel->setStyleSheet("color: red; font-weight: bold; background: transparent;");
            return;
        }
    }

    // ✅ Vérification du format via l'expression régulière
    if (regex.match(text).hasMatch()) {
        field->setStyleSheet("border: 2px solid green; padding: 5px; background: white;");
        errorLabel->setText("✔️ Valide");
        errorLabel->setStyleSheet("color: green; font-weight: bold; background: transparent;");
    } else {
        field->setStyleSheet("border: 2px solid red; padding: 5px; background: white;");
        errorLabel->setText("❌ " + errorMsg);
        errorLabel->setStyleSheet("color: red; font-weight: bold; background: transparent;");
    }
}



void MainWindow::validateTextEdit(QTextEdit *field, QLabel *errorLabel, QRegularExpression regex, const QString &errorMsg)
{
    QString text = field->toPlainText().trimmed();  // ✅ Supprime les espaces inutiles

    // 🔹 Vérifie que le texte contient au moins 2 mots (séparés par un espace)
    QRegularExpression wordCheckRegex(R"(^[A-Za-zÀ-ÿ\s,]+$)");
    QStringList words = text.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

    if (regex.match(text).hasMatch() && wordCheckRegex.match(text).hasMatch() && words.size() >= 2) {
        // ✅ Texte valide (fond transparent)
        errorLabel->setText("✔️ Valide");
        errorLabel->setStyleSheet("color: green; font-weight: bold; background: transparent;");
    } else {
        // ❌ Texte invalide → Affiche un message d'erreur
        errorLabel->setText("❌ Le texte doit contenir au moins 2 mots, sans chiffres .");
        errorLabel->setStyleSheet("color: red; font-weight: bold; background: transparent;");
    }
}


void MainWindow::on_annuler_clicked()
{
    if (modeModification) {
        // 🔹 Si on est en mode modification, remettre les anciennes valeurs
        QSqlQuery query;
        query.prepare("SELECT CIN, NOM, PRENOM, AGE, SEXE, NUM, POIDS, DATE_RDV, REMARQUES, STATUT_VACCINAL FROM CARNETS WHERE CIN = :cin");
        query.bindValue(":cin", idAModifier);

        if (query.exec() && query.next()) {
            // ✅ Remettre toutes les anciennes valeurs, y compris le CIN
            ui->cin->setText(query.value("CIN").toString());
            ui->nom_carnet->setText(query.value("NOM").toString());
            ui->prenom_carnet->setText(query.value("PRENOM").toString());
            ui->age->setText(query.value("AGE").toString());
            ui->G->setChecked(query.value("SEXE").toString() == "Garçon");
            ui->num->setText(query.value("NUM").toString());
            ui->poids->setText(query.value("POIDS").toString());
            ui->date_rdv->setDate(query.value("DATE_RDV").toDate());
            ui->remarques->setPlainText(query.value("REMARQUES").toString());
            ui->statut_vaccinal->setCurrentText(query.value("STATUT_VACCINAL").toString());
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
bool MainWindow::estValide()
{
    bool valide = true;
    QString styleErreur = "color: red; font-weight: bold; background: transparent;";
    QString styleValide = "color: green; font-weight: bold; background: transparent;";

    // 🔹 Vérification du nom (lettres uniquement)
    if (!QRegularExpression("^[A-Za-zÀ-ÿ\\s-]+$").match(ui->nom_carnet->text().trimmed()).hasMatch()) {
        ui->nomErrorLabel->setText("❌ Le nom doit contenir uniquement des lettres.");
        ui->nomErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->nomErrorLabel->setText("✔️ Valide");
        ui->nomErrorLabel->setStyleSheet(styleValide);
    }

    // 🔹 Vérification du prénom
    if (!QRegularExpression("^[A-Za-zÀ-ÿ\\s-]+$").match(ui->prenom_carnet->text().trimmed()).hasMatch()) {
        ui->prenomErrorLabel->setText("❌ Le prénom doit contenir uniquement des lettres.");
        ui->prenomErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->prenomErrorLabel->setText("✔️ Valide");
        ui->prenomErrorLabel->setStyleSheet(styleValide);
    }

    // 🔹 Vérification de l'âge (entre 1 et 100)
    if (!QRegularExpression("^[1-9][0-9]?$|^100$").match(ui->age->text().trimmed()).hasMatch()) {
        ui->ageErrorLabel->setText("❌ L'âge doit être entre 1 et 100.");
        ui->ageErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->ageErrorLabel->setText("✔️ Valide");
        ui->ageErrorLabel->setStyleSheet(styleValide);
    }

    // 🔹 Vérification du CIN (exactement 8 chiffres)
    if (!QRegularExpression("^[0-9]{8}$").match(ui->cin->text().trimmed()).hasMatch()) {
        ui->cinErrorLabel->setText("❌ Le CIN doit contenir exactement 8 chiffres.");
        ui->cinErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->cinErrorLabel->setText("✔️ Valide");
        ui->cinErrorLabel->setStyleSheet(styleValide);
    }

    // 🔹 Vérification du numéro de téléphone (exactement 8 chiffres)
    if (!QRegularExpression("^[0-9]{8}$").match(ui->num->text().trimmed()).hasMatch()) {
        ui->numErrorLabel->setText("❌ Le numéro doit contenir exactement 8 chiffres.");
        ui->numErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->numErrorLabel->setText("✔️ Valide");
        ui->numErrorLabel->setStyleSheet(styleValide);
    }

    // 🔹 Vérification du poids (nombre valide avec max 2 décimales)
    if (!QRegularExpression("^[0-9]{1,3}(\\.[0-9]{1,2})?$").match(ui->poids->text().trimmed()).hasMatch()) {
        ui->poidsErrorLabel->setText("❌ Le poids doit être un nombre valide.");
        ui->poidsErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->poidsErrorLabel->setText("✔️ Valide");
        ui->poidsErrorLabel->setStyleSheet(styleValide);
    }

    // 🔹 Vérification des remarques (min 2 mots, max 200 caractères)
    QString remarques = ui->remarques->toPlainText().trimmed();
    QStringList words = remarques.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    if (remarques.isEmpty() || words.size() < 2 || remarques.length() > 200) {
        ui->remarquesErrorLabel->setText("❌ Les remarques doivent contenir au moins 2 mots et max 200 caractères.");
        ui->remarquesErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->remarquesErrorLabel->setText("✔️ Valide");
        ui->remarquesErrorLabel->setStyleSheet(styleValide);
    }

    // 🔹 Vérification du sexe (obligatoire)
    if (!ui->G->isChecked() && !ui->F->isChecked()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un sexe !");
        valide = false;
    }

    return valide;
}

void MainWindow::on_btnGeneratePDF_clicked()
{
    // 🔹 Récupérer le CIN depuis `suppid`
    QString cin = ui->suppid->text().trimmed();
    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un CIN valide !");
        return;
    }

    // 🔹 Récupération des informations du patient depuis la base de données
    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM, AGE, SEXE, NUM, POIDS, STATUT_VACCINAL, REMARQUES FROM CARNETS WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "Erreur", "Aucun patient trouvé avec ce CIN !");
        return;
    }

    // 📌 Récupération des données
    QString nom = query.value("NOM").toString();
    QString prenom = query.value("PRENOM").toString();
    QString age = query.value("AGE").toString();
    QString sexe = query.value("SEXE").toString();
    QString num = query.value("NUM").toString();
    QString poids = query.value("POIDS").toString();
    QString statutVaccinal = query.value("STATUT_VACCINAL").toString();
    QString remarques = query.value("REMARQUES").toString();

    // 📂 Boîte de dialogue pour enregistrer le fichier
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer le carnet", "", "PDF Files (*.pdf)");
    if (filePath.isEmpty()) return;

    // 📄 Création du PDF
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setPageOrientation(QPageLayout::Landscape);
    pdfWriter.setResolution(300);

    QPainter painter(&pdfWriter);
    if (!painter.isActive()) {
        qDebug() << "Erreur : Impossible d'initialiser le QPainter.";
        return;
    }

    // 🎨 Définition des polices et couleurs
    QFont titleFont("Arial", 26, QFont::Bold);
    QFont headerFont("Arial", 16, QFont::Bold);
    QFont normalFont("Arial", 12);
    QPen bluePen(Qt::darkBlue);
    QPen blackPen(Qt::black);

    // 📌 Position et dimensions ajustées
    int leftMargin = 60;
    int topMargin = 150;
    int tableWidth = (pdfWriter.width() / 2) - 120;
    int tableHeight = pdfWriter.height() - 400;  // 🔹 Taille du tableau réduite

    // 🎯 Titre principal (corrigé)
    painter.setFont(titleFont);
    painter.setPen(bluePen);
    painter.drawText(QRect(0, 80, pdfWriter.width(), 60), Qt::AlignCenter, "🔹 CARNET DE SUIVI MÉDICAL 🔹");

    // 🏥 Encadrement des sections
    painter.setFont(headerFont);
    painter.setPen(blackPen);

    // 🔹 Tableau 1 : Informations Patient
    painter.drawRect(leftMargin, topMargin, tableWidth, tableHeight);
    painter.drawText(leftMargin + 20, topMargin - 10, "📌 INFORMATIONS DU PATIENT");

    painter.setFont(normalFont);
    int textY = topMargin + 40;
    int textSpacing = 40;  // 🔹 Espacement optimisé entre chaque ligne

    painter.drawText(leftMargin + 20, textY, "🆔 CIN : " + cin);
    painter.drawText(leftMargin + 20, textY += textSpacing, "👤 Nom : " + nom);
    painter.drawText(leftMargin + 20, textY += textSpacing, "🧑 Prénom : " + prenom);
    painter.drawText(leftMargin + 20, textY += textSpacing, "🎂 Âge : " + age + " ans");
    painter.drawText(leftMargin + 20, textY += textSpacing, "⚧ Sexe : " + sexe);
    painter.drawText(leftMargin + 20, textY += textSpacing, "📞 Téléphone : " + num);
    painter.drawText(leftMargin + 20, textY += textSpacing, "⚖️ Poids : " + poids + " kg");
    painter.drawText(leftMargin + 20, textY += textSpacing, "💉 Statut Vaccinal : " + statutVaccinal);
    painter.drawText(leftMargin + 20, textY += textSpacing, "📝 Remarques : " + remarques);

    // 🔹 Tableau 2 : Historique des Rendez-vous
    int rightMargin = leftMargin + tableWidth + 80;
    painter.drawRect(rightMargin, topMargin, tableWidth, tableHeight);
    painter.drawText(rightMargin + 20, topMargin - 10, "📅 HISTORIQUE DES RENDEZ-VOUS");

    painter.setFont(normalFont);
    int rowY = topMargin + 40;

    // 📌 Récupération des rendez-vous
    QSqlQuery queryRDV;
    queryRDV.prepare("SELECT DATE_RDV FROM CARNETS WHERE CIN = :cin");
    queryRDV.bindValue(":cin", cin);

    if (queryRDV.exec()) {
        int rowCount = 0;
        while (queryRDV.next() && rowCount < 10) {
            QDateTime dateRDV = queryRDV.value("DATE_RDV").toDateTime();

            // 🔹 Affichage de la date RDV à gauche du texte "Date RDV"
            painter.drawText(rightMargin + 20, rowY, dateRDV.toString("dd/MM/yyyy") + "  📆 Date RDV");
            rowY += textSpacing;
            rowCount++;
        }
    } else {
        qDebug() << "Erreur SQL : " << queryRDV.lastError().text();
    }

    // 📌 Section Cachet & Signature (réduite et repositionnée)
    int cachetX = pdfWriter.width() - 280;
    int cachetY = pdfWriter.height() - 100;
    painter.setFont(headerFont);
    painter.drawText(cachetX, cachetY, "🔹 Cachet & Signature :");
    painter.drawRect(cachetX, cachetY + 30, 220, 70);

    // 📌 Fin du rendu
    painter.end();

    // 📌 Confirmation de la génération du PDF
    QMessageBox msgBox;
    msgBox.setWindowTitle("✅ PDF généré");
    msgBox.setText("📄 Le carnet du patient a été enregistré avec succès !");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setButtonText(QMessageBox::Ok, "D'accord");
    msgBox.exec();
}
void MainWindow::testSMS()
{
    SmsNotif sms;
    QString numero = "+21654430709";
    QString message = "📅 Rappel : Votre RDV est prévu dans les prochaines 48 heures. Merci de confirmer la réception.";

    bool sent = sms.sendSMS(numero, message);

    if (sent) {
        QMessageBox::information(this, "SMS ", "📩 Votre Message a été envoyé avec succes.");
    } else {
        QMessageBox::critical(this, "Erreur SMS", "❌ La requête n'a pas pu être envoyée.");
    }
}
