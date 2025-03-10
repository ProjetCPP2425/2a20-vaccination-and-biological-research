#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

// Constructeur de MainWindow
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(6);
    ui->frame->setVisible(false);

    // Gestion de la navigation
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
    connect(ui->compagne, &QPushButton::clicked, this, &MainWindow::navigateToCampaigns);

    connect(ui->button_supprimer, &QPushButton::clicked, this, &MainWindow::on_Button_supprimer_clicked);


    // Connexion du bouton d'ajout de campagne
   // connect(ui->pushButton_ajouter, &QPushButton::clicked, this, &MainWindow::on_pushButton_ajouter_clicked);


    //connect(ui->button_modifier, &QPushButton::clicked, this, &MainWindow::on_button_modifier_clicked);





    // 🔹 Empêcher la saisie de caractères non numériques dans les doses
    ui->objectif_doses->setValidator(new QIntValidator(1, 999999, this));
    ui->doses_administrees->setValidator(new QIntValidator(0, 999999, this));



}

// Destructeur
MainWindow::~MainWindow()
{
    delete ui;
}

// ---- Navigation vers la page des campagnes ----
void MainWindow::navigateToCampaigns()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->frame->setVisible(true);
    displayCompagne();
}


// ---- Affichage des campagnes ----
void MainWindow::displayCompagne()
{
    QSqlQueryModel *model = compagneTmp.afficher();
    if (model) {
        ui->tableView->setModel(model);
        ui->tableView->resizeColumnsToContents();
    } else {
        QMessageBox::warning(this, "Erreur", "Échec du chargement des campagnes.");
    }
}





bool MainWindow::estValide()
{
    QString nom_campagne = ui->nom_campagne->text().trimmed();
    QString zone_geographique = ui->zone_geographique->text().trimmed();
    int objectif_doses = ui->objectif_doses->text().toInt();
    int doses_administrees = ui->doses_administrees->text().toInt();
    QDate date_debut = ui->date_debut->date();
    QDate date_fin = ui->date_fin->date();

    // 🔹 Vérifier que les champs ne sont pas vides
    if (nom_campagne.isEmpty() || zone_geographique.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le nom de la campagne et la zone géographique ne peuvent pas être vides !");
        return false;
    }

    // 🔹 Vérifier que les doses sont des nombres positifs
    if (objectif_doses <= 0 || doses_administrees < 0) {
        QMessageBox::warning(this, "Erreur", "Les doses doivent être des nombres positifs !");
        return false;
    }

    // 🔹 Vérifier que les doses administrées ne dépassent pas l'objectif
    if (doses_administrees > objectif_doses) {
        QMessageBox::warning(this, "Erreur", "Les doses administrées ne peuvent pas dépasser l'objectif !");
        return false;
    }

    // 🔹 Vérifier que la date de fin est après ou égale à la date de début
    if (date_fin < date_debut) {
        QMessageBox::warning(this, "Erreur", "La date de fin doit être supérieure ou égale à la date de début !");
        return false;
    }

    return true; // ✅ Toutes les conditions sont remplies
}
















void MainWindow::on_button_modifier_clicked()
{
    QString nom = ui->nom_saisie->text().trimmed();

    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un nom de campagne !");
        return;
    }

    if (!compagneTmp.chargerCampagne(nom)) {
        QMessageBox::critical(this, "Erreur", "Aucune campagne trouvée avec ce nom !");
        return;
    }

    modeModification = true;
    nomOriginal = nom; // ✅ Stocke le nom original de la campagne pour la mise à jour

    // 🔹 Pré-remplissage des champs avec les données existantes
    ui->nom_campagne->setText(compagneTmp.getNomCampagne());
    ui->date_debut->setDate(compagneTmp.getDateDebut());
    ui->date_fin->setDate(compagneTmp.getDateFin());
    ui->zone_geographique->setText(compagneTmp.getZoneGeographique());
    ui->objectif_doses->setText(QString::number(compagneTmp.getObjectifDoses()));
    ui->doses_administrees->setText(QString::number(compagneTmp.getDosesAdministrees()));
    ui->vaccins_utilises->setCurrentText(compagneTmp.getVaccinsUtilises());
    ui->statut->setCurrentText(compagneTmp.getStatut());

    ui->Affichage_6->setCurrentWidget(ui->ajoutct_14); // ✅ Redirige vers la page de modification
}







void MainWindow::on_pushButton_ajouter_clicked()
{
    if (!estValide()) {
        return; // ✅ Vérifie que les données saisies sont correctes
    }

    QString nom_campagne = ui->nom_campagne->text().trimmed();
    QDate date_debut = ui->date_debut->date();
    QDate date_fin = ui->date_fin->date();
    QString zone_geographique = ui->zone_geographique->text().trimmed();
    int objectif_doses = ui->objectif_doses->text().toInt();
    int doses_administrees = ui->doses_administrees->text().toInt();
    QString vaccins_utilises = ui->vaccins_utilises->currentText();
    QString statut = ui->statut->currentText();

    // ✅ Création de l'objet campagne
    Compagne compagne(0, nom_campagne, date_debut, date_fin, zone_geographique, objectif_doses, doses_administrees, vaccins_utilises, statut);

    if (modeModification) {
        // 🔹 Mode Modification : Mettre à jour l'enregistrement
        if (compagne.modifier(nomOriginal)) {
            QMessageBox::information(this, "Succès", "Campagne modifiée avec succès !");
            modeModification = false; // ✅ Désactiver le mode modification après mise à jour
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la modification !");
            return;
        }
    } else {
        // 🔹 Mode Ajout : Vérifier si une campagne du même nom existe avant d’ajouter
        if (compagne.ajouter()) {
            QMessageBox::information(this, "Succès", "Campagne ajoutée avec succès.");
        } else {
            QMessageBox::critical(this, "Erreur", "Cette campagne existe déjà !");
            return;
        }
    }

    // 🔹 Rafraîchir la liste des campagnes et revenir à l'affichage
    displayCompagne();
    ui->stackedWidget->setCurrentIndex(4);

    // 🔹 Réinitialisation des champs après Ajout/Modification
    ui->nom_campagne->clear();
    ui->date_debut->setDate(QDate::currentDate());
    ui->date_fin->setDate(QDate::currentDate());
    ui->zone_geographique->clear();
    ui->objectif_doses->clear();
    ui->doses_administrees->clear();
    ui->vaccins_utilises->setCurrentIndex(0);
    ui->statut->setCurrentIndex(0);
}








void MainWindow::on_Button_supprimer_clicked()
{
    QString nom = ui->nom_saisie->text().trimmed();  // 🔹 Récupérer le nom de la campagne à supprimer

    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un nom de campagne valide !");
        return;
    }

    // 🔹 Vérifier si la campagne existe avant de supprimer
    if (!compagneTmp.chargerCampagne(nom)) {
        QMessageBox::critical(this, "Erreur", "Aucune campagne trouvée avec ce nom !");
        return;
    }

    // 🔹 Boîte de confirmation avant suppression
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Voulez-vous vraiment supprimer la campagne \"" + nom + "\" ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (compagneTmp.supprimer(nom)) {
            QMessageBox::information(this, "✅ Succès", "Campagne supprimée avec succès !");

            // 🔹 Effacer les champs après suppression
            ui->nom_saisie->clear();

            // 🔹 Rafraîchir l'affichage après suppression
            displayCompagne();
        } else {
            QMessageBox::critical(this, "❌ Erreur", "Échec de la suppression !");
        }
    }
}





