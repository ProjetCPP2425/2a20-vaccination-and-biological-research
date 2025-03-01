#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QHBoxLayout>  // Pour QHBoxLayout
#include <QPushButton>  // Pour QPushButton
#include <QWidget>      // Pour QWidget
#include <QSqlQuery>
#include <QSqlError> // Pour afficher les erreurs SQL
#include <QDebug>    // Pour le débogage

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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




void MainWindow::on_ajout_carnet_clicked()
{
    QString cin = ui->cin->text();
    QString nom = ui->nom_carnet->text();
    QString prenom = ui->prenom_carnet->text(); // Correction ici (vérifie bien dans Qt Designer)
    int age = ui->age->text().toInt();
    QString sexe = ui->G->isChecked() ? "Garçon" : "Femme";
    QString num = ui->num->text();
    float poids = ui->poids->text().toFloat();
    QDate date_rdv = ui->date_rdv->date();
    QString remarques = ui->remarques->toPlainText();
    QString statut_vaccinal = ui->statut_vaccinal->currentText();

    // Vérification des champs obligatoires
    if (cin.isEmpty() || nom.isEmpty() || prenom.isEmpty() || num.isEmpty() || age <= 0) {
        QMessageBox::warning(this, "Champs vides", "Veuillez remplir tous les champs obligatoires.");
        return;
    }

    Carnets carnet(0, cin, nom, prenom, age, sexe, num, poids, date_rdv, remarques, statut_vaccinal);

    if (carnet.ajouter()) {
        QMessageBox::information(this, "Succès", "Carnet ajouté avec succès.");
        ui->tableView->setModel(carnetTmp.afficher()); // Rafraîchissement de la liste des carnets
        displayCarnet();                 // Refresh lab list

        // Réinitialisation des champs
        ui->cin->clear();
        ui->nom_carnet->clear();
        ui->prenom_carnet->clear();
        ui->age->clear();
        ui->num->clear();
        ui->poids->clear(); // Même comportement que nom et prénom
        ui->date_rdv->setDate(QDate::currentDate());
        ui->remarques->clear();
        ui->statut_vaccinal->setCurrentIndex(0);
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout du carnet.");
    }


}
void MainWindow::displayCarnet()
{
    QSqlQueryModel *model = carnetTmp.afficher();
    if (model) {
        ui->tableView->setModel(model);
        ui->tableView->resizeColumnsToContents(); // Optional: Adjust columns
    } else {
        QMessageBox::warning(this, "Erreur", "Échec du chargement des Carnets.");
    }
}

void MainWindow::on_supprimerC_clicked()
{
    // Récupérer l'ID saisi par l'utilisateur
    int id = ui->suppid->text().toInt();

    // Vérifier si l'ID est valide
    if (id <= 0) {
        QMessageBox::warning(this, "ID invalide", "Veuillez entrer un ID valide !");
        return;
    }

    // Demander confirmation avant de supprimer
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment supprimer ce carnet ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        Carnets c;
        if (c.supprimer(id)) {  // Appel de la fonction dans `carnet.cpp`
            QMessageBox::information(this, "Succès", "Carnet supprimé avec succès !");
            ui->tableView->setModel(c.afficher());  // Mettre à jour l'affichage
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression du Carnet !");
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
}


