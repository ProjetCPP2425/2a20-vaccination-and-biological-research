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
    connect(ui->btnPredire, &predictionResult::clicked, this, &MainWindow::on_btnPredire_clicked);

    ui->stackedWidget->setCurrentIndex(6);
    ui->frame->setVisible(false);
    connect(ui->carnet, &predictionResult::clicked, this, [=](){
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
    connect(ui->pushButton_3, &predictionResult::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(6);
        updateSidebarVisibility(6);

    });
    connect(ui->pushButton_6, &predictionResult::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(6);
        ui->frame->setVisible(false);
    });
    connect(ui->pushButton_3, &predictionResult::clicked, this, [=](){

        ui->stackedWidget->setCurrentIndex(0);
        ui->frame->setVisible(true);

    });
    // Bouton "Employé" -> Page 0 (pageEmp)
    connect(ui->employe, &predictionResult::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(0);
        ui->frame->setVisible(true);
    });

    // Bouton "Produits" -> Page 1 (pageProd)
    connect(ui->produits, &predictionResult::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(1);
        ui->frame->setVisible(true);
    });

    // Bouton "Laboratoire" -> Page 2 (pageLabo)
    connect(ui->labo, &predictionResult::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(2);
        ui->frame->setVisible(true);
    });

    // Bouton "Vaccin" -> Page 3 (pageVac)
    connect(ui->vaccin, &predictionResult::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(3);
        ui->frame->setVisible(true);
    });

    // Bouton "Carnet" -> Page 4 (pageCar)
    connect(ui->carnet, &predictionResult::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(5);
    });

    // Bouton "Campagne" -> Page 5 (pageComp)
    connect(ui->compagne, &predictionResult::clicked, this, [=](){
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
    QString cin = ui->cin->text();
    QString nom = ui->nom_carnet->text();
    QString prenom = ui->prenom_carnet->text();
    int age = ui->age->text().toInt();
    QString sexe = ui->G->isChecked() ? "Garçon" : "Femme";
    QString num = ui->num->text();
    float poids = ui->poids->text().toFloat();
    QDate date_rdv = ui->date_rdv->date();
    QString remarques = ui->remarques->toPlainText();
    QString statut_vaccinal = ui->statut_vaccinal->currentText();

    // Vérifier si les champs obligatoires sont remplis
    if (cin.isEmpty() || nom.isEmpty() || prenom.isEmpty() || num.isEmpty() || age <= 0) {
        QMessageBox::warning(this, "Champs vides", "Veuillez remplir tous les champs obligatoires.");
        return;
    }

    Carnets carnet(0, cin, nom, prenom, age, sexe, num, poids, date_rdv, remarques, statut_vaccinal);

    if (modeModification) {
        // 🔹 Mode Modification : Mettre à jour l'enregistrement existant
        carnetTmp.supprimer(idAModifier); // Supprime l'ancien et réinsère le nouveau
        if (carnet.ajouter()) {
            QMessageBox::information(this, "Succès", "Carnet modifié avec succès !");
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la modification !");
            return;
        }
        modeModification = false;
        idAModifier = -1;
    } else {
        // 🔹 Mode Ajout : Ajouter un nouveau carnet
        if (carnet.ajouter()) {
            QMessageBox::information(this, "Succès", "Carnet ajouté avec succès !");
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de l'ajout !");
            return;
        }
    }

    // 🔄 Rafraîchir la liste des carnets
    displayCarnet();

    // 🧹 Réinitialiser les champs
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

// 🔹 Supprimer un carnet
void MainWindow::on_supprimerC_clicked()
{
    int id = ui->suppid->text().toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "ID invalide", "Veuillez entrer un ID valide !");
        return;
    }

    if (QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment supprimer ce carnet ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        if (carnetTmp.supprimer(id)) {
            QMessageBox::information(this, "Succès", "Carnet supprimé avec succès !");
            displayCarnet();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression du carnet !");
        }
    }
}

// 🔹 Charger un carnet pour modification
void MainWindow::on_modifierC_clicked()
{
    int id = ui->suppid->text().toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID valide !");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT CIN, NOM, PRENOM, AGE, SEXE, NUM, POIDS, DATE_RDV, REMARQUES, STATUT_VACCINAL FROM CARNETS WHERE Id_Carnet = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        // 🔄 Charger les valeurs dans les champs
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

        // 🔄 Activer le mode modification
        idAModifier = id;
        modeModification = true;

        QMessageBox::information(this, "Modification", "Données chargées, vous pouvez modifier !");
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de charger les données du carnet !");
    }
}

void MainWindow::on_btnPredire_clicked()
{
    int id = ui->idPredictionInput->text().toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID valide.");
        return;
    }

    QString prediction = carnetTmp.analyserRisque(id);
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


