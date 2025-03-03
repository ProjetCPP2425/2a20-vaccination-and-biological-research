#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "vaccin.h"
#include <QMessageBox>
#include <QHBoxLayout>  // Pour QHBoxLayout
#include <QPushButton>  // Pour QPushButton
#include <QWidget>      // Pour QWidget
#include <QDebug>       // (Facultatif) Pour le debug
#include <QSqlError>  // Ajoute cette ligne pour afficher les erreurs SQL


// Constructeur de MainWindow
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(6);
    ui->frame->setVisible(false);


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
        MedAnalysis *medApi = new MedAnalysis(this);
        connect(medApi, &MedAnalysis::dataFetched, this, &MainWindow::updateCovidStats);
        connect(medApi, &MedAnalysis::errorOccurred, this, &MainWindow::showError);

        // Lancer la récupération des données au démarrage pour un pays (ex: France)
        medApi->fetchCovidData("France");
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
void MainWindow::on_pushButton_ajouter_v_clicked() {
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

    // Vérifier que les champs obligatoires ne sont pas vides
    if (nom.isEmpty() || statut.isEmpty() || certification.isEmpty()) {
        QMessageBox::warning(this, "Champs vides", "Veuillez remplir tous les champs avant de continuer !");
        return;
    }

    // Vérifier que la date d'expiration est après la date de création
    if (date_expiration <= date_creation) {
        QMessageBox::warning(this, "Date invalide", "La date d'expiration doit être après la date de création !");
        return;
    }

    // Créer un objet Vaccin
    Vaccin v(0,nom, type, date_creation, date_expiration, statut, certification);

    if (modeModification) {
        // 🔹 Mode Modification : Mettre à jour l'enregistrement existant
        qDebug() << "Mode Modification - ID :" << idAModifier;
        if (v.modifier(idAModifier)) {
            QMessageBox::information(this, "Succès", "Vaccin modifié avec succès !");
            modeModification = false;  // Désactiver le mode modification
            idAModifier = -1;
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

    // 🔄 Mise à jour de la table après ajout/modification
    ui->tableView->setModel(v.afficher());
    ui->tableView->verticalHeader()->setDefaultSectionSize(35);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(150);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setStyleSheet("QTableView::item { padding: 10px; }");
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    // 🧹 Effacer les champs après ajout/modification
    ui->lineEdit_nom_2->clear();
    ui->lineEdit_typev_2->clear();
    ui->comboBox_status_2->setCurrentIndex(0);
    ui->dateEdit_creation_2->setDate(QDate::currentDate());
    ui->dateEdit_expiration_2->setDate(QDate::currentDate());
    ui->lineEdit_certification_2->clear();
}



void MainWindow::on_pushButton_suppv_clicked() {
    // Récupérer l'ID saisi par l'utilisateur
    int id = ui->lineEdit->text().toInt();

    // Vérifier si l'ID est valide
    if (id <= 0) {
        QMessageBox::warning(this, "ID invalide", "Veuillez entrer un ID valide !");
        return;
    }

    // Demander confirmation avant de supprimer
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment supprimer ce vaccin ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        Vaccin v;
        if (v.supprimer(id)) {  // Appel de la fonction dans `vaccin.cpp`
            QMessageBox::information(this, "Succès", "Vaccin supprimé avec succès !");
            ui->tableView->setModel(v.afficher());  // Mettre à jour l'affichage
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression du vaccin !");
        }
    }
}
void MainWindow::on_pushButton_modifier_2_clicked() {
    int id = ui->lineEdit->text().toInt();  // Récupérer l'ID depuis le champ

    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID valide !");
        return;
    }

    // Appel de la fonction pour remplir les champs
    remplirChampsModification(id);

    // Rediriger vers la page d'ajout
    ui->stackedWidget->setCurrentIndex(3);
}
void MainWindow::remplirChampsModification(int id) {
    QString nom, type, statut, certification;
    QDate date_creation, date_expiration;

    Vaccin v;
    if (v.remplirChampsModification(id, nom, type, date_creation, date_expiration, statut, certification)) {
        // Remplir les champs
        ui->lineEdit_nom_2->setText(nom);
        ui->lineEdit_typev_2->setText(type);
        ui->dateEdit_creation_2->setDate(date_creation);
        ui->dateEdit_expiration_2->setDate(date_expiration);
        ui->comboBox_status_2->setCurrentText(statut);
        ui->lineEdit_certification_2->setText(certification);

        // Stocker l'ID à modifier et activer le mode modification
        idAModifier = id;
        modeModification = true;

        QMessageBox::information(this, "Modification", "Données chargées, vous pouvez modifier !");
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
        qDebug() << "Erreur lors de la vérification des vaccins expirés :" << query.lastError().text();
        return;
    }

    QString message = "Les vaccins suivants sont expirés :\n\n";
    bool hasExpired = false;

    while (query.next()) {
        QString nom = query.value(0).toString();
        QDate date_expiration = query.value(1).toDate();
        message += "📌 " + nom + " - Expiré le : " + date_expiration.toString("dd/MM/yyyy") + "\n";
        hasExpired = true;
    }

    if (hasExpired) {
        QMessageBox::warning(this, "⚠️ Vaccins Expirés", message);
    } else {
        qDebug() << "✅ Aucun vaccin expiré.";
    }
}
void MainWindow::updateCovidStats(QString country, int cases, int deaths, int recovered, int population, double vaccinationRate)
{
    QString message = QString(
                          "📊 Données COVID-19 pour %1 :\n\n"
                          "✅ Cas confirmés : %2\n"
                          "❌ Décès : %3\n"
                          "💪 Guérisons : %4\n"
                          "👥 Population totale : %5\n"
                          "💉 Taux de vaccination : %6%"
                          ).arg(country)
                          .arg(cases)
                          .arg(deaths)
                          .arg(recovered)
                          .arg(population)
                          .arg(vaccinationRate);

    QMessageBox::information(this, "Données Épidémiologiques", message);
}
void MainWindow::showError(QString error)
{
    QMessageBox::warning(this, "Erreur API", "Impossible de récupérer les données : " + error);
}
