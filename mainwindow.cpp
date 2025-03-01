#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlQueryModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(6);  // Page par défaut (login)
    ui->frame->setVisible(false);  // Masquer le menu latéral au départ

    // Connexions pour les boutons de navigation entre pages
    connect(ui->carnet, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(2); // Page Laboratoire
        ui->frame->setVisible(true);           // Afficher le menu latéral
        void displayCompagne();  // Slot pour afficher les campagnes
    });

    // Gestion du menu latéral
    auto updateSidebarVisibility = [=](int index) {
        if (index == 6) {  // Page de login
            ui->frame->setVisible(false);
        } else {
            ui->frame->setVisible(true);
        }
    };

    // Connexions et navigation entre autres pages comme dans votre code précédent
    connect(ui->pushButton_3, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(6);
        ui->frame->setVisible(false);
    });
    connect(ui->pushButton_6, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(6);
        ui->frame->setVisible(false);
    });
    connect(ui->pushButton_3, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);  // Retour à la page d'accueil
        ui->frame->setVisible(true);
    });

    // Navigation entre autres pages
    connect(ui->employe, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);  // Page Employé
        ui->frame->setVisible(true);
    });
    connect(ui->produits, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(1);  // Page Produits
        ui->frame->setVisible(true);
    });
    connect(ui->labo, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(2);  // Page Laboratoire
        ui->frame->setVisible(true);
    });
    connect(ui->vaccin, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(3);  // Page Vaccin
        ui->frame->setVisible(true);
    });
    connect(ui->compagne, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(4);  // Page Campagne
        ui->frame->setVisible(true);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Slot pour ajouter une campagne
void MainWindow::on_pushButton_ajouter_clicked()
{
    // Récupération des valeurs des champs de l'interface
    QString nom = ui->nom_campagne->text();
    QDate date_debut = ui->date_debut->date();
    QDate date_fin = ui->date_fin->date();
    QString zone = ui->zone_geographique->text();
    QString objectif = ui->objectif_doses->currentText();
    QString doses = ui->doses_administrees->currentText();
    QString vaccins = ui->vaccins_utilises->currentText();
    QString statut = ui->statut->currentText();

    // Vérification des champs obligatoires
    if (nom.isEmpty() || zone.isEmpty() || objectif.isEmpty() || doses.isEmpty() || vaccins.isEmpty()) {
        QMessageBox::warning(this, "Champs vides", "Veuillez remplir tous les champs obligatoires.");
        return;
    }

    // Créer une instance de la classe Compagne
    Compagne compagne(0, nom, date_debut, date_fin, zone, objectif.toInt(), doses.toInt(), vaccins, statut);

    // Ajouter la campagne à la base de données
    if (compagne.ajouter()) {
        QMessageBox::information(this, "Succès", "Campagne ajoutée avec succès.");
        displayCompagne();  // Rafraîchissement de la liste des campagnes

        // Réinitialisation des champs de l'interface
        ui->nom_campagne->clear();
        ui->zone_geographique->clear();
        ui->objectif_doses->setCurrentIndex(0);
        ui->doses_administrees->setCurrentIndex(0);
        ui->vaccins_utilises->setCurrentIndex(0);
        ui->date_debut->setDate(QDate::currentDate());
        ui->date_fin->setDate(QDate::currentDate());
        ui->statut->setCurrentIndex(0);
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de la campagne.");
    }
}

// Fonction pour afficher les campagnes dans la table
void MainWindow::displayCompagne()
{
    // Récupérer le modèle avec les données des campagnes
    QSqlQueryModel *model = compagneTmp.afficher();

    // Vérifier si le modèle est valide avant de l'afficher
    if (model != nullptr) {
        ui->tableView->setModel(model);  // Afficher le modèle dans le tableView
        ui->tableView->resizeColumnsToContents();  // Ajuster la taille des colonnes
    } else {
        QMessageBox::warning(this, "Erreur", "Échec du chargement des campagnes.");
    }
}
