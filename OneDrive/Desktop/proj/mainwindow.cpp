#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include "laboratoire.h"
#include <QSqlQuery>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(6);
    ui->frame->setVisible(false);

    connect(ui->pushButton_37, &QPushButton::clicked, this, &MainWindow::on_pushButton_37_clicked);
   // connect(ui->comboBox_Tri, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_comboBox_Tri_currentIndexChanged);


    // Function to handle page changes and toggle sidebar
    auto updateSidebarVisibility = [=](int index) {
        if (index == 6) { // Login page
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
        displayLaboratoires();
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

// Ajouter Laboratoire
void MainWindow::on_pushButton_32_clicked()
{
    int id = ui->lineEdit_44->text().toInt(); // ID from modification input
    QString nom = ui->lineEdit_NomLab->text();
    QString adresse = ui->lineEdit_Adresse->text();
    QString responsable = ui->lineEdit_Responsable->text();
    QString type = ui->lineEdit_Type->text();
    QString statut = ui->comboBox_Statut->currentText();
    int nb_projets = ui->spinBox_NbProjets->value();
    int matriels = ui->spinBox_NbProjets_2->value();
    int personnel = ui->spinBox_NbProjets_3->value();
    float depense = ui->doubleSpinBox_7->value();
    QDate date_creation = ui->dateEdit_7->date();

    if (nom.isEmpty() || adresse.isEmpty() || responsable.isEmpty() || type.isEmpty()) {
        QMessageBox::warning(this, "Champs vides", "Veuillez remplir tous les champs obligatoires.");
        return;
    }

    // Check if it's an update
    if (id != 0) {
        Laboratoire lab(id, nom, adresse, type, responsable, depense, nb_projets, statut, matriels, personnel, date_creation);
        if (lab.modifier(id)) {
            QMessageBox::information(this, "Succès", "Laboratoire modifié avec succès.");
            ui->tableView->setModel(labTmp.afficher());
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la modification.");
        }
    } else {
        // Normal Add
        Laboratoire lab(0, nom, adresse, type, responsable, depense, nb_projets, statut, matriels, personnel, date_creation);
        if (lab.ajouter()) {
            QMessageBox::information(this, "Succès", "Laboratoire ajouté avec succès.");
            ui->tableView->setModel(labTmp.afficher());
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de l'ajout.");
        }
    }

    // Clear fields
    ui->lineEdit_NomLab->clear();
    ui->lineEdit_Adresse->clear();
    ui->lineEdit_Responsable->clear();
    ui->lineEdit_Type->clear();
    ui->comboBox_Statut->setCurrentIndex(0);
    ui->spinBox_NbProjets->setValue(0);
    ui->spinBox_NbProjets_2->setValue(0);
    ui->spinBox_NbProjets_3->setValue(0);
    ui->doubleSpinBox_7->setValue(0.00);
    ui->dateEdit_7->setDate(QDate::currentDate());
}

void MainWindow::displayLaboratoires()
{
    QSqlQueryModel *model = labTmp.afficher();

    if (model) {
        ui->tableView->setModel(model);
        ui->tableView->resizeColumnsToContents();
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    } else {
        QMessageBox::warning(this, "Erreur", "Échec du chargement des laboratoires.");
    }
}
//supprimer un labo

void MainWindow::on_pushButton_37_clicked()
{
    // Convert the input text to an integer
    int id = ui->lineEdit_44->text().toInt();

    // Debug: Check the ID
    qDebug() << "ID entered for deletion:" << id;

    // Call supprimer() from Etmp
    bool test = labTmp.supprimer(id);

    if (test) {
        QMessageBox::information(nullptr, QObject::tr("OK"),QObject::tr("Suppression effectuée\n" "Click Cancel to exit."),QMessageBox::Cancel);
        ui->lineEdit_44->clear();
        // Refresh table view
        ui->tableView->setModel(labTmp.afficher());
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"), QObject::tr("Suppression non effectuée.\n" "Click Cancel to exit."), QMessageBox::Cancel);
    }
}



//modifier un labo
void MainWindow::on_pushButton_34_clicked()
{
    int id = ui->lineEdit_44->text().toInt();
    QSqlQuery query;
    query.prepare("SELECT * FROM SMARTVACC.LABORATOIRES WHERE ID_LABORATOIRE = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        // Fill the form with existing data
        ui->lineEdit_NomLab->setText(query.value("NOM_LAB").toString());
        ui->lineEdit_Adresse->setText(query.value("ADRESSE").toString());
        ui->lineEdit_Responsable->setText(query.value("RESPONSABLE").toString());
        ui->lineEdit_Type->setText(query.value("TYPE").toString());
        ui->comboBox_Statut->setCurrentText(query.value("STATUT").toString());
        ui->spinBox_NbProjets->setValue(query.value("NB_PROJETS").toInt());
        ui->spinBox_NbProjets_2->setValue(query.value("MATRIELS").toInt());
        ui->spinBox_NbProjets_3->setValue(query.value("PERSONNEL").toInt());
        ui->doubleSpinBox_7->setValue(query.value("DEPONSE").toFloat());
        ui->dateEdit_7->setDate(query.value("DATE_CREATION").toDate());


        QMessageBox::information(this, "Modification", "Tu peux modifier maintenant !");
        // Switch to the "Ajouter" tab
        ui->stackedWidget->setCurrentIndex(2);
        ui->tableView->setModel(labTmp.afficher());


    } else {
        QMessageBox::warning(this, "Erreur", "Laboratoire introuvable.");
    }
}

void MainWindow::on_rechercherLabo_textChanged(const QString &arg1)
{
    qDebug() << "Recherche en cours... Texte saisi :" << arg1; // Debug

    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    QString searchText = arg1.trimmed(); // Remove leading/trailing spaces

    if (searchText.isEmpty()) {
            query.prepare("SELECT * FROM SMARTVACC.LABORATOIRES");
        }
        else {
            query.prepare("SELECT * FROM SMARTVACC.LABORATOIRES WHERE "
                          "LOWER(NOM_LAB) LIKE LOWER(:val) OR "
                          "LOWER(RESPONSABLE) LIKE LOWER(:val) OR "
                          "LOWER(STATUT) LIKE LOWER(:val)");
            query.bindValue(":val", "%" + searchText + "%");
        }

        if (!query.exec()) {
            qDebug() << "Erreur SQL :" << query.lastError().text(); // Debug erreur
            return;
        }
        model->clear();
        model->setQuery(query);
        ui->tableView->setModel(model);
}

/*
void MainWindow::on_comboBox_Tri_currentIndexChanged(int index)
{
    QString orderBy;

    switch (index) {
    case 0:  // Tri by number of projects (from most to least)
        orderBy = "NB_PROJETS DESC";  // DESC for descending order (most to least)
        break;
    case 1:  // Tri by functional status (Fonctionnel > Maintenon > Non fonctionnel)
        orderBy = "CASE STATUT "
                  "WHEN 'Fonctionnel' THEN 1 "
                  "WHEN 'Maintenon' THEN 2 "
                  "WHEN 'Non fonctionnel' THEN 3 "
                  "ELSE 4 END ASC"; // Custom sorting for STATUT
        break;
    case 2:  // Tri by laboratory type (A to Z)
        orderBy = "TYPE COLLATE NOCASE ASC ";  // ASC for alphabetical order (A to Z)
        break;
    default:
        return;
    }

    // Update the query with the new orderBy clause
    QSqlQuery query;
    query.prepare("SELECT * FROM SMARTVACC.LABORATOIRES ORDER BY " + orderBy);

    QSqlQueryModel *model = new QSqlQueryModel();
    if (query.exec()) {
        model->setQuery(query);
        ui->tableView->setModel(model);
    } else {
        qDebug() << "Erreur SQL: " << query.lastError().text();
        QMessageBox::warning(this, "Erreur", "Erreur lors du tri des laboratoires.");
    }
}

*/
