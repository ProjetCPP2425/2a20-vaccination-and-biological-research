#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QHBoxLayout>  // Pour QHBoxLayout
#include <QPushButton>  // Pour QPushButton
#include <QWidget>      // Pour QWidget
#include <QSqlQuery>
#include <QSqlError> // Pour afficher les erreurs SQL
#include <QDebug>    // Pour le débogage
#include <QRegularExpression>  // ✅ Ajoute cette ligne au début de mainwindow.cpp


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnPredire, &QPushButton::clicked, this, &MainWindow::on_btnPredire_clicked);

    connect(ui->nom_carnet, &QLineEdit::textChanged, this, [=]() {
        validateInput(ui->nom_carnet, ui->nomErrorLabel, QRegularExpression("^[A-Za-zÀ-ÿ\\s-]+$"), "Le nom doit contenir au moins 3 lettres, sans chiffres ni caractères spéciaux.");
    });

    connect(ui->prenom_carnet, &QLineEdit::textChanged, this, [=]() {
        validateInput(ui->prenom_carnet, ui->prenomErrorLabel, QRegularExpression("^[A-Za-zÀ-ÿ\\s-]+$"), "Le prénom doit contenir au moins 3 lettres, sans chiffres ni caractères spéciaux.");
    });


    connect(ui->age, &QLineEdit::textChanged, this, [=]() {
        validateInput(ui->age, ui->ageErrorLabel, QRegularExpression("^[1-9][0-9]?$|^100$"), "L'âge doit être entre 1 et 100.");
    });

    connect(ui->cin, &QLineEdit::textChanged, this, [=]() {
        validateInput(ui->cin, ui->cinErrorLabel, QRegularExpression("^[0-9]{8}$"), "Le CIN doit contenir exactement 8 chiffres.");
    });

    connect(ui->num, &QLineEdit::textChanged, this, [=]() {
        validateInput(ui->num, ui->numErrorLabel, QRegularExpression("^[0-9]{8}$"), "Le numéro doit contenir exactement 8 chiffres.");
    });

    connect(ui->poids, &QLineEdit::textChanged, this, [=]() {
        validateInput(ui->poids, ui->poidsErrorLabel, QRegularExpression("^[0-9]{1,3}(\\.[0-9]{1,2})?$"), "Le poids doit être un nombre valide.");
    });

    connect(ui->remarques, &QTextEdit::textChanged, this, [=]() {
        validateTextEdit(ui->remarques, ui->remarquesErrorLabel, QRegularExpression("^.{0,200}$"), "Les remarques doivent contenir au moins 2 mots sans chiffres ni caractères spéciaux .");
    });

    connect(ui->annuler, &QPushButton::clicked, this, &MainWindow::on_annuler_clicked);


    ui->stackedWidget->setCurrentIndex(6);
    ui->frame->setVisible(false);
    connect(ui->carnet, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(2); // Switch to lab page
        ui->frame->setVisible(true);           // Show sidebar
        displayCarnet();                 // Refresh lab list
    });

    // Function to handle page changes and toggle sidebar
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

}

MainWindow::~MainWindow()
{
    delete ui;
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
}




void MainWindow::on_ajout_carnet_clicked()
{
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
        sexe = "Garçon"; // ✅ "Garçon" devient "G"
    } else if (ui->F->isChecked()) {
        sexe = "Femme"; // ✅ "Femme" devient "F"
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

        qDebug() << "Modification du carnet avec CIN :" << idAModifier;

        carnetTmp.supprimer(idAModifier);

        if (carnet.ajouter()) {
            QMessageBox::information(this, "Succès", "Carnet modifié avec succès !");
            ui->tabs->setCurrentIndex(1);  // 🔄 Rediriger vers l'onglet Affichage (Vérifie l'index de ton `QTabWidget`)

        } else {
            QMessageBox::critical(this, "Erreur", "Le CIN existe déjà ! Impossible de modifier");
            return;
        }

        modeModification = false;
        idAModifier = "";
    } else {
        if (carnet.ajouter()) {
            QMessageBox::information(this, "Succès", "Carnet ajouté avec succès !");
            ui->tabs->setCurrentIndex(1);  // 🔄 Rediriger vers l'onglet Affichage (Vérifie l'index de ton `QTabWidget`)

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
    QString cin = ui->suppid->text().trimmed(); // ✅ Récupérer le CIN depuis l'interface

    qDebug() << "🔍 CIN saisi pour suppression :" << cin;  // 🔍 Vérification du CIN

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
        QMessageBox::warning(this, "Erreur", "Aucun carnet trouvé avec ce CIN !");
        return;
    }

    // ✅ Boîte de confirmation avec boutons "Oui" et "Non"
    QMessageBox msgBox;
    msgBox.setWindowTitle("Confirmation");
    msgBox.setText("Voulez-vous vraiment supprimer ce carnet ?");
    QPushButton *btnOui = msgBox.addButton("Oui", QMessageBox::YesRole);
    QPushButton *btnNon = msgBox.addButton("Non", QMessageBox::NoRole);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.exec();

    // ✅ Vérifier quel bouton a été cliqué
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
    QString cin = ui->suppid->text().trimmed(); // ✅ Récupérer le CIN depuis l'interface

    qDebug() << "🔍 CIN sélectionné pour modification :" << cin;  // 🔍 Vérification de la valeur entrée

    // 🔴 Vérifier si le champ CIN est vide
    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un CIN valide !");
        return;
    }

    // 🔴 Vérifier si le CIN existe dans la base de données avant modification
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

    // ✅ Charger les informations du carnet si le CIN existe
    QSqlQuery query;
    query.prepare("SELECT CIN,NOM, PRENOM, AGE, SEXE, NUM, POIDS, DATE_RDV, REMARQUES, STATUT_VACCINAL FROM CARNETS WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    if (query.exec() && query.next()) {
        // ✅ Remplir les champs avec les valeurs existantes

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

        // ✅ Activer le mode modification
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
void MainWindow::validateInput(QLineEdit *field, QLabel *errorLabel, QRegularExpression regex, const QString &errorMsg)
{
    QString text = field->text().trimmed();  // Supprime les espaces inutiles

    if (regex.match(text).hasMatch()) {
        // ✅ Champ valide → Bordure verte + Message OK (fond transparent)
        field->setStyleSheet("border: 2px solid green; padding: 5px; background: white;");
        errorLabel->setText("✔️ Valide");
        errorLabel->setStyleSheet("color: green; font-weight: bold; background: transparent;");
    } else {
        // ❌ Champ invalide → Bordure rouge + Message d'erreur
        field->setStyleSheet("border: 2px solid red; padding: 5px; background: white;");

        if (field == ui->nom_carnet || field == ui->prenom_carnet) {
            errorLabel->setText("❌ Ne doit contenir que des lettres et espaces (pas de chiffres ni caractères spéciaux).");
        } else {
            errorLabel->setText("❌ " + errorMsg);
        }

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
        // ❌ Texte invalide → Affiche un message d'erreur (sans changer la bordure du champ)
        errorLabel->setText("❌ Le texte doit contenir au moins 2 mots, sans chiffres ni caractères spéciaux (sauf `,`).");
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


