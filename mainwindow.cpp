#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "employes.h"
#include <QMessageBox>
#include <QHBoxLayout>  // Pour QHBoxLayout
#include <QPushButton>  // Pour QPushButton
#include <QWidget>      // Pour QWidget
#include <QDebug>       // (Facultatif) Pour le debug
#include <QSqlError>  // Ajoute cette ligne pour afficher les erreurs SQL
#include <QRegularExpression>



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
        // Mettre à jour immédiatement l'affichage des vaccins
        Employe e;
        ui->tableView->setModel(e.afficher());





        // Ajustements d'affichage
        ui->tableView->verticalHeader()->setDefaultSectionSize(35);
        ui->tableView->horizontalHeader()->setDefaultSectionSize(150);
        ui->tableView->resizeColumnsToContents();
        ui->tableView->setStyleSheet("QTableView::item { padding: 10px; }");
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
        //connect(ui->pushButton_modifier_employe, &QPushButton::clicked, this, &MainWindow::on_pushButton_modifier_employe_clicked);
        connect(ui->pushButton_supprimer_employe, &QPushButton::clicked, this, &MainWindow::on_pushButton_supprimer_employe_clicked);

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

    });


    connect(ui->carnet, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(5);
    });

    connect(ui->compagne, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(4);
        ui->frame->setVisible(true);
    });





}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ajouter_employe_clicked() {
    // 🔹 Récupération des valeurs des champs
    QString cin = ui->lineedit_cin_employe->text().trimmed();
    int id_employe = ui->lineedit_id_employe->text().toInt();
    QString nom = ui->lineedit_nom_employe->text().trimmed();
    QString prenom = ui->lineedit_prenom_employe->text().trimmed();
    QString poste = ui->lineeditposte->text().trimmed();
    float salaire = ui->lineditsalaire->text().toFloat();
    QString contact = ui->lineeditcontact->text().trimmed();
    QDate date_embauche = ui->dateEdit->date();
    QString type_absences = ui->combobox_abscences->currentText();

    // 🔹 Vérification du sexe via les radio buttons
    QString sexe;
    if (ui->radiobutton_homme->isChecked()) {
        sexe = "Homme";
    } else if (ui->radiobutton_femme->isChecked()) {
        sexe = "Femme";
    } else {
        QMessageBox::warning(this, "Champ manquant", "Veuillez sélectionner un sexe !");
        return;
    }

    // 🔹 Vérification de la disponibilité (doit être un entier)
    int disponibilite = (ui->lineeditdisponibilite->text().toLower() == "oui") ? 1 : 0;

    // 🔹 Vérifications des champs obligatoires
    if (cin.isEmpty() || nom.isEmpty() || prenom.isEmpty() || poste.isEmpty() || contact.isEmpty()) {
        QMessageBox::warning(this, "Champs vides", "Veuillez remplir tous les champs obligatoires !");
        return;
    }

    // 🔹 Vérification du format du CIN (doit contenir exactement 8 chiffres)
    QRegularExpression regexCIN("^\\d{8}$");
    if (!regexCIN.match(cin).hasMatch()) {
        QMessageBox::warning(this, "Format incorrect", "Le CIN doit contenir exactement 8 chiffres !");
        return;
    }

    // 🔹 Vérification du salaire (doit être positif)
    if (salaire <= 0) {
        QMessageBox::warning(this, "Salaire invalide", "Le salaire doit être un nombre positif !");
        return;
    }

    // 🔹 Vérification que ID_EMPLOYE est unique en base
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM SMARTVACC.EMPLOYES WHERE ID_EMPLOYE = :id");
    checkQuery.bindValue(":id", id_employe);

    if (!checkQuery.exec()) {
        QMessageBox::critical(this, "Erreur SQL", "Échec de la vérification de l'ID Employé : " + checkQuery.lastError().text());
        return;
    }

    checkQuery.next();
    if (checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Doublon détecté", "Cet ID Employé existe déjà !");
        return;
    }

    // 🔹 Création de l'objet Employe et ajout en base de données
    Employe emp(cin, id_employe, nom, prenom, poste, sexe, salaire, contact, date_embauche, disponibilite, type_absences);

    if (emp.ajouter()) {
        QMessageBox::information(this, "Succès", "L'employé a été ajouté avec succès !");

        // 🔄 Mise à jour de la tableView
        ui->tableView->setModel(emp.afficher());
        ui->tableView->verticalHeader()->setDefaultSectionSize(35);
        ui->tableView->horizontalHeader()->setDefaultSectionSize(150);
        ui->tableView->resizeColumnsToContents();
        ui->tableView->setStyleSheet("QTableView::item { padding: 10px; }");
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

        // 🧹 Effacer les champs après ajout
        ui->lineedit_cin_employe->clear();
        ui->lineedit_id_employe->clear();
        ui->lineedit_nom_employe->clear();
        ui->lineedit_prenom_employe->clear();
        ui->lineeditposte->clear();
        ui->lineditsalaire->clear();
        ui->lineeditcontact->clear();
        ui->dateEdit->setDate(QDate::currentDate());
        ui->lineeditdisponibilite->clear();
        ui->combobox_abscences->setCurrentIndex(0);
    } else {
        QMessageBox::critical(this, "Erreur SQL", "L'ajout de l'employé a échoué !");
    }
}

void MainWindow::on_pushButton_supprimer_employe_clicked() {
    // 🔹 Récupérer le CIN depuis le champ de saisie
    QString cin = ui->lineedit_cin_employe_test->text().trimmed();

    // 🔹 Vérifier si le CIN est valide
    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un CIN valide !");
        return;
    }

    // 🔹 Vérifier si l'employé existe
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT * FROM SMARTVACC.EMPLOYES WHERE CIN = :cin");
    checkQuery.bindValue(":cin", cin);
    if (!checkQuery.exec() || !checkQuery.next()) {
        QMessageBox::warning(this, "Erreur", "Aucun employé trouvé avec ce CIN !");
        return;
    }

    // 🔹 Demander confirmation
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment supprimer cet employé ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // 🔹 Exécuter la suppression
        QSqlQuery query;
        query.prepare("DELETE FROM SMARTVACC.EMPLOYES WHERE CIN = :cin");
        query.bindValue(":cin", cin);

        if (!query.exec()) {
            QMessageBox::critical(this, "Erreur SQL", "Échec de la suppression : " + query.lastError().text());
            return;
        }

        // ✅ Succès
        QMessageBox::information(this, "Succès", "L'employé a été supprimé avec succès !");
        ui->tableView->setModel(employe.afficher()); // Mise à jour de l'affichage

        // 🔹 Nettoyer les champs
        ui->lineedit_cin_employe->clear();
        ui->lineedit_nom_employe->clear();
        ui->lineedit_prenom_employe->clear();
        ui->lineeditposte->clear();
        ui->radiobutton_homme->setChecked(false);
        ui->radiobutton_femme->setChecked(false);
        ui->lineditsalaire->clear();
        ui->lineeditcontact->clear();
        ui->dateEdit->setDate(QDate::currentDate());
        ui->lineeditdisponibilite->clear();
        ui->combobox_abscences->setCurrentIndex(0);
    }
}

/*void MainWindow::on_pushButton_modifier_employe_clicked() {
    QString cin = ui->lineedit_cin_employe->text().trimmed();

    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un CIN valide !");
        return;
    }

    // 🔹 Vérifier si l'employé existe
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT * FROM SMARTVACC.EMPLOYES WHERE CIN = :cin");
    checkQuery.bindValue(":cin", cin);

    if (!checkQuery.exec()) {
        qDebug() << "❌ Erreur SQL lors de la vérification du CIN :" << checkQuery.lastError().text();
        return;
    }

    if (!checkQuery.next()) {
        qDebug() << "❌ Aucun employé trouvé avec CIN :" << cin;
        QMessageBox::warning(this, "Erreur", "Aucun employé trouvé avec ce CIN !");
        return;
    }

    // 🔹 Récupérer les nouvelles valeurs des champs
    QString newNom = ui->lineedit_nom_employe->text().trimmed();
    QString newPrenom = ui->lineedit_prenom_employe->text().trimmed();
    QString newPoste = ui->lineeditposte->text().trimmed();
    QString newSexe = (ui->radiobutton_homme->isChecked()) ? "Homme" : "Femme";
    float newSalaire = ui->lineditsalaire->text().toFloat();
    QString newContact = ui->lineeditcontact->text().trimmed();
    QDate newDateEmbauche = ui->dateEdit->date();
    int newDisponibilite = (ui->lineeditdisponibilite->text().toLower() == "oui") ? 1 : 0;
    QString newTypeAbsences = ui->combobox_abscences->currentText();

    // 🔹 Vérifier si `QDate` est bien converti
    QString dateEmbaucheString = newDateEmbauche.toString("yyyy-MM-dd");

    // 🔹 Afficher les valeurs avant `UPDATE`
    qDebug() << "🔹 Valeurs envoyées :"
             << "CIN:" << cin
             << "NOM:" << newNom
             << "PRENOM:" << newPrenom
             << "POSTE:" << newPoste
             << "SEXE:" << newSexe
             << "SALAIRE:" << newSalaire
             << "CONTACT:" << newContact
             << "DATE_EMBAUCHE:" << dateEmbaucheString
             << "DISPONIBILITE:" << newDisponibilite
             << "TYPE_ABSENCES:" << newTypeAbsences;

    // 🔹 Exécuter l'UPDATE
    QSqlQuery query;
    query.prepare("UPDATE SMARTVACC.EMPLOYES SET NOM = :nom, PRENOM = :prenom, POSTE = :poste, "
                  "SEXE = :sexe, SALAIRE = :salaire, CONTACT = :contact, "
                  "DATE_EMBAUCHE = TO_DATE(:date_embauche, 'YYYY-MM-DD'), "
                  "DISPONIBILITE = :disponibilite, TYPE_ABSENCES = :type_absences "
                  "WHERE CIN = :cin");

    query.bindValue(":nom", newNom);
    query.bindValue(":prenom", newPrenom);
    query.bindValue(":poste", newPoste);
    query.bindValue(":sexe", newSexe);
    query.bindValue(":salaire", newSalaire);
    query.bindValue(":contact", newContact);
    query.bindValue(":date_embauche", dateEmbaucheString);
    query.bindValue(":disponibilite", newDisponibilite);
    query.bindValue(":type_absences", newTypeAbsences);
    query.bindValue(":cin", cin);

    if (!query.exec()) {
        qDebug() << "❌ ERREUR SQL :" << query.lastError().text();
        QMessageBox::critical(this, "Erreur SQL", "Échec de la mise à jour : " + query.lastError().text());
        return;
    }

    if (query.numRowsAffected() == 0) {
        QMessageBox::warning(this, "Aucune modification", "Aucune ligne mise à jour !");
        qDebug() << "⚠️ Aucune ligne mise à jour !";
        return;
    }

    QMessageBox::information(this, "Succès", "L'employé a été modifié avec succès !");
    ui->tableView->setModel(employe.afficher());
}*/
