#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include "laboratoire.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(6);
    ui->frame->setVisible(false);


    // Function to handle page changes and toggle sidebar
    auto updateSidebarVisibility = [=](int index) {
        if (index == 6) { // Login page
            ui->frame->setVisible(false);
        } else {
            ui->frame->setVisible(true);
        }
    };

/*
    connect(ui->lineEdit_NomLab, &QLineEdit::textChanged, this, &MainWindow::validateFields);
    connect(ui->lineEdit_Adresse, &QLineEdit::textChanged, this, &MainWindow::validateFields);
    connect(ui->lineEdit_Responsable, &QLineEdit::textChanged, this, &MainWindow::validateFields);
    connect(ui->lineEdit_Type, &QLineEdit::textChanged, this, &MainWindow::validateFields);

*/
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

    // Check if the laboratory with this name exists
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM SMARTVACC.LABORATOIRES WHERE NOM_LAB = :nom");
    query.bindValue(":nom", nom);
    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la vérification du laboratoire.");
        return;
    }

    query.next();
    int count = query.value(0).toInt();

    // Create the Laboratoire object
    Laboratoire lab(0, nom, adresse, type, responsable, depense, nb_projets, statut, matriels, personnel, date_creation);

    if (count > 0) {
        if (lab.modifier(nom)) {
            QMessageBox::information(this, "Succès", "Laboratoire modifié avec succès.");
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la modification.");
        }
    } else {
        if (lab.ajouter()) {
            QMessageBox::information(this, "Succès", "Laboratoire ajouté avec succès.");
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de l'ajout.");
        }
    }

    // Update the table view
    ui->tableView->setModel(labTmp.afficher());

    // Clear input fields
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

        ui->tableView->setStyleSheet(
            "QHeaderView::section {"
            "   background-color: #B00000; "
            "   color: white; "
            "   font-weight: bold; "
            "   padding: 5px; "
            "   border: 1px solid black;"
            "}"
            );

    } else {
        QMessageBox::warning(this, "Erreur", "Échec du chargement des laboratoires.");
    }
}


// Supprimer un labo
void MainWindow::on_pushButton_37_clicked()
{
    QString nom = ui->lineEdit_44->text(); // Get the Nom_Lab from input field

    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer le nom du laboratoire à supprimer.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM SMARTVACC.LABORATOIRES WHERE NOM_LAB = :nom");
    query.bindValue(":nom", nom);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la vérification du laboratoire.");
        return;
    }

    // Ensure we have a valid query result before calling next()
    if (!query.next()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération des données.");
        return;
    }

    int count = query.value(0).toInt();

    if (count == 0) {
        QMessageBox::warning(this, "Erreur", "Laboratoire non trouvable pour le supprimer.");
        return;
    }

    // If found, proceed with deletion
    bool success = labTmp.supprimer(nom);

    if (success) {
        QMessageBox::information(this, "Succès", "Laboratoire supprimé avec succès.");
        ui->tableView->setModel(labTmp.afficher());
        ui->lineEdit_44->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression.");
    }
}






//modifier un labo
void MainWindow::on_pushButton_34_clicked()
{
    QString nom = ui->lineEdit_44->text();
    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer le nom du laboratoire à modifier.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM SMARTVACC.LABORATOIRES WHERE NOM_LAB = :nom");
    query.bindValue(":nom", nom);

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
        ui->doubleSpinBox_7->setValue(query.value("DEPENSE").toFloat());
        ui->dateEdit_7->setDate(query.value("DATE_CREATION").toDate());



        ui->stackedWidget->setCurrentWidget(ui->pageLabo);
        ui->Affichage_3->setCurrentWidget(ui->ajoutct_7);

        QMessageBox::information(this, "Modification", "Tu peux modifier maintenant !");

    } else {
        QMessageBox::warning(this, "Erreur", "Laboratoire introuvable.");
    }
}

/*
void MainWindow::validateFields()
{
    // Initially clear all error messages and set the borders to default (green or none)
    ui->label_NomError->clear();
    ui->label_AdresseError->clear();
    ui->label_ResponsableError->clear();
    ui->label_TypeError->clear();

    ui->lineEdit_NomLab->setStyleSheet("border: 2px solid green;");
    ui->lineEdit_Adresse->setStyleSheet("border: 2px solid green;");
    ui->lineEdit_Responsable->setStyleSheet("border: 2px solid green;");
    ui->lineEdit_Type->setStyleSheet("border: 2px solid green;");

    // Validate Nom
    QString nomText = ui->lineEdit_NomLab->text();
    QRegularExpression nomRegex("^[A-Za-zÀ-ÿ]+$");  // Only letters (no numbers or special characters)
    if (!nomRegex.match(nomText).hasMatch()) {
        ui->label_NomError->setText("Erreur: Le nom ne doit contenir que des lettres.");
        ui->lineEdit_NomLab->setStyleSheet("border: 2px solid red; border-radius: 5px; background-color: transparent; box-shadow: 0px 0px 10px red;");
        return;  // Exit after first error to show only one
    }

    // Validate Adresse (should not be empty)
    QString adresseText = ui->lineEdit_Adresse->text();
    if (adresseText.isEmpty()) {
        ui->label_AdresseError->setText("Erreur: L'adresse ne peut pas être vide.");
        ui->lineEdit_Adresse->setStyleSheet("border: 2px solid red; border-radius: 5px; background-color: transparent; box-shadow: 0px 0px 10px red;");
        return;  // Exit after first error to show only one
    }

    // Validate Responsable (should only contain letters)
    QString responsableText = ui->lineEdit_Responsable->text();
    QRegularExpression responsableRegex("^[A-Za-zÀ-ÿ]+$");  // Only letters
    if (!responsableRegex.match(responsableText).hasMatch()) {
        ui->label_ResponsableError->setText("Erreur: Le responsable ne doit contenir que des lettres.");
        ui->lineEdit_Responsable->setStyleSheet("border: 2px solid red; border-radius: 5px; background-color: transparent; box-shadow: 0px 0px 10px red;");
        return;  // Exit after first error to show only one
    }

    // Validate Type (should not be empty)
    QString typeText = ui->lineEdit_Type->text();
    if (typeText.isEmpty()) {
        ui->label_TypeError->setText("Erreur: Le type ne peut pas être vide.");
        ui->lineEdit_Type->setStyleSheet("border: 2px solid red; border-radius: 5px; background-color: transparent; box-shadow: 0px 0px 10px red;");
        return;  // Exit after first error to show only one
    }
}
*/

/*
void MainWindow::on_rechercherLabo_textChanged(QString &arg1)
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
