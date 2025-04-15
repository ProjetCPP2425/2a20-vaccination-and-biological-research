#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

//metiers
#include <QPdfWriter>
#include <QPainter>
#include <QTextDocument>
#include <QFileDialog>


#include <QAbstractItemModel>


#include <QTextDocument> // À mettre tout en haut si pas encore présent

#include <QCompleter>

//statistique
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

#include <QRandomGenerator>







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








    // 🔹 Empêcher la saisie de caractères non numériques dans les doses
    ui->objectif_doses->setValidator(new QIntValidator(1, 999999, this));
    ui->doses_administrees->setValidator(new QIntValidator(0, 999999, this));

    //metiers1
    connect(ui->search_date, &QDateEdit::dateChanged, this, &MainWindow::rechercherParDate);




    connect(ui->btn_export_appro, &QPushButton::clicked, this, [=]() {
        QString nom = ui->nom_saisie->text().trimmed();
        if (nom.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez entrer le nom d'une campagne à exporter !");
            return;
        }

        if (!compagneTmp.chargerCompagne(nom)) {
            QMessageBox::warning(this, "Erreur", "Aucune campagne trouvée avec ce nom !");
            return;
        }

        genererPDFApprovisionnement(compagneTmp);

    });

    //metier 2
    connect(ui->btn_affecter, &QPushButton::clicked, this, &MainWindow::affecterEmployeACompagne);



    if (QSqlDatabase::database().isOpen()) {
        afficherAffectations();
        chargerEmployes();
        chargerCompagnes();

    }



    connect(ui->btn_stat_vaccination_zone, &QPushButton::clicked, this, &MainWindow::afficherStatistiquesVaccinationParZone);

    connect(ui->btn_valider_affectations, &QPushButton::clicked, this, &MainWindow::validerAffectations);


//trie
    connect(ui->comboBox_tri, &QComboBox::currentTextChanged, this, &MainWindow::trierCompagnes);

//recherche
    ui->lineEdit_recherche->setPlaceholderText("🔎 Rechercher par nom, vaccin ou zone...");

    connect(ui->lineEdit_recherche, &QLineEdit::textChanged, this, &MainWindow::rechercherCompagnes);


//Annuler
   // connect(ui->pushButton_annuler, &QPushButton::clicked, this, &MainWindow::on_pushButton_annuler_clicked);







}

// Destructeur
MainWindow::~MainWindow()
{
    delete ui;
}

// ---- Navigation vers la page des compagnes ----
void MainWindow::navigateToCampaigns()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->frame->setVisible(true);
    displayCompagne();
}


// ---- Affichage des compagnes ----
void MainWindow::displayCompagne()
{
    QSqlQueryModel *model = compagneTmp.afficher();
    if (model) {
        ui->tableView->setModel(model);
        ui->tableView->resizeColumnsToContents();
    } else {
        QMessageBox::warning(this, "Erreur", "Échec du chargement des compagnes.");
    }
}




bool MainWindow::estValide()
{
    QString nom_compagne = ui->nom_compagne->text().trimmed();
    QString zone_geographique = ui->zone_geographique->text().trimmed();
    int objectif_doses = ui->objectif_doses->text().toInt();
    int doses_administrees = ui->doses_administrees->text().toInt();
    QDate date_debut = ui->date_debut->date();
    QDate date_fin = ui->date_fin->date();

    bool valide = true;
    QString styleErreur = "color: red; font-weight: bold; background: transparent;";
    QString styleOk = "color: green; font-weight: bold; background: transparent;";


    // 🔹 Champ : nom de la compagne
    if (nom_compagne.isEmpty()) {
        ui->labelErreurNomCompagne->setText("❌ Ce champ est requis.");
        ui->labelErreurNomCompagne->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->labelErreurNomCompagne->setText("✔️ Valide");
        ui->labelErreurNomCompagne->setStyleSheet(styleOk);
    }

    // 🔹 Champ : zone géographique
    if (zone_geographique.isEmpty()) {
        ui->labelErreurZone->setText("❌ Ce champ est requis.");
        ui->labelErreurZone->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->labelErreurZone->setText("✔️ Valide");
        ui->labelErreurZone->setStyleSheet(styleOk);
    }

    // 🔹 Champ : objectif_doses
    if (objectif_doses <= 0) {
        ui->labelErreurObjectif->setText("❌ Doit être un nombre strictement positif.");
        ui->labelErreurObjectif->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->labelErreurObjectif->setText("✔️ Valide");
        ui->labelErreurObjectif->setStyleSheet(styleOk);
    }

    // 🔹 Champ : doses_administrees
    if (doses_administrees < 0) {
        ui->labelErreurAdministrees->setText("❌ Doit être un nombre positif.");
        ui->labelErreurAdministrees->setStyleSheet(styleErreur);
        valide = false;
    } else if (doses_administrees > objectif_doses) {
        ui->labelErreurAdministrees->setText("❌ Ne peut pas dépasser l'objectif.");
        ui->labelErreurAdministrees->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->labelErreurAdministrees->setText("✔️ Valide");
        ui->labelErreurAdministrees->setStyleSheet(styleOk);
    }

    // 🔹 Champ : dates
    if (date_fin < date_debut) {
        ui->labelErreurDates->setText("❌ La date de fin doit être après la date de début.");
        ui->labelErreurDates->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->labelErreurDates->setText("✔️ Valide");
        ui->labelErreurDates->setStyleSheet(styleOk);
    }

    // 🔹 Champ : fournitures
    if (ui->fournitures->text().trimmed().isEmpty()) {
        ui->labelErreurFournitures->setText("❌ Ce champ est requis.");
        ui->labelErreurFournitures->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->labelErreurFournitures->setText("✔️ Valide");
        ui->labelErreurFournitures->setStyleSheet(styleOk);
    }

    // 🔹 Champ : coût des fournitures
    bool conversionOk = false;
    float cout = ui->cout_fournitures->text().toFloat(&conversionOk);
    if (!conversionOk || cout < 0) {
        ui->labelErreurCout->setText("❌ Entrez un coût valide.");
        ui->labelErreurCout->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->labelErreurCout->setText("✔️ Valide");
        ui->labelErreurCout->setStyleSheet(styleOk);
    }

    return valide;
}

















void MainWindow::on_button_modifier_clicked()
{
    QString nom = ui->nom_saisie->text().trimmed();

    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un nom de compagne !");
        return;
    }

    if (!compagneTmp.chargerCompagne(nom)) {
        QMessageBox::critical(this, "Erreur", "Aucune compagne trouvée avec ce nom !");
        return;
    }

    modeModification = true;
    nomOriginal = nom; // ✅ Stocke le nom original de la compagne pour la mise à jour

    // 🔹 Pré-remplissage des champs avec les données existantes
    ui->nom_compagne->setText(compagneTmp.getNomCompagne());
    ui->date_debut->setDate(compagneTmp.getDateDebut());
    ui->date_fin->setDate(compagneTmp.getDateFin());
    ui->zone_geographique->setText(compagneTmp.getZoneGeographique());
    ui->objectif_doses->setText(QString::number(compagneTmp.getObjectifDoses()));
    ui->doses_administrees->setText(QString::number(compagneTmp.getDosesAdministrees()));
    ui->vaccins_utilises->setCurrentText(compagneTmp.getVaccinsUtilises());
    ui->statut->setCurrentText(compagneTmp.getStatut());

    ui->fournitures->setText(compagneTmp.getFournitures());
    ui->cout_fournitures->setText(QString::number(compagneTmp.getCoutFournitures()));


    ui->Affichage_6->setCurrentWidget(ui->ajoutct_14); // ✅ Redirige vers la page de modification
}







void MainWindow::on_pushButton_ajouter_clicked()
{
    if (!estValide()) {
        return; // ✅ Vérifie que les données saisies sont correctes
    }

    QString nom_compagne = ui->nom_compagne->text().trimmed();
    QDate date_debut = ui->date_debut->date();
    QDate date_fin = ui->date_fin->date();
    QString zone_geographique = ui->zone_geographique->text().trimmed();
    int objectif_doses = ui->objectif_doses->text().toInt();
    int doses_administrees = ui->doses_administrees->text().toInt();
    QString vaccins_utilises = ui->vaccins_utilises->currentText();
    QString statut = ui->statut->currentText();

    QString fournitures = ui->fournitures->text().trimmed();
    float cout_fournitures = ui->cout_fournitures->text().toFloat();

    // ✅ Création de l'objet compagne
    Compagne compagne(0, nom_compagne, date_debut, date_fin, zone_geographique, objectif_doses, doses_administrees, vaccins_utilises, statut, fournitures, cout_fournitures);

    if (modeModification) {
        // 🔹 Mode Modification : Mettre à jour l'enregistrement
        if (compagne.modifier(nomOriginal)) {
            QMessageBox::information(this, "Succès", "Compagne modifiée avec succès !");
            modeModification = false; // ✅ Désactiver le mode modification après mise à jour
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la modification !");
            return;
        }
    } else {
        // 🔹 Mode Ajout : Vérifier si une compagne du même nom existe avant d’ajouter
        if (compagne.ajouter()) {
            QMessageBox::information(this, "Succès", "Compagne ajoutée avec succès.");
        } else {
            QMessageBox::critical(this, "Erreur", "Cette compagne existe déjà !");
            return;
        }
    }

    // 🔹 Rafraîchir la liste des compagnes et revenir à l'affichage
    displayCompagne();
    ui->stackedWidget->setCurrentIndex(4);

    // 🔹 Réinitialisation des champs après Ajout/Modification
    ui->nom_compagne->clear();
    ui->date_debut->setDate(QDate::currentDate());
    ui->date_fin->setDate(QDate::currentDate());
    ui->zone_geographique->clear();
    ui->objectif_doses->clear();
    ui->doses_administrees->clear();
    ui->vaccins_utilises->setCurrentIndex(0);
    ui->statut->setCurrentIndex(0);

    ui->fournitures->clear();
    ui->cout_fournitures->clear();
}








void MainWindow::on_Button_supprimer_clicked()
{
    QString nom = ui->nom_saisie->text().trimmed();  // 🔹 Récupérer le nom de la compagne à supprimer

    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un nom de compagne valide !");
        return;
    }

    // 🔹 Vérifier si la compagne existe avant de supprimer
    if (!compagneTmp.chargerCompagne(nom)) {
        QMessageBox::critical(this, "Erreur", "Aucune compagne trouvée avec ce nom !");
        return;
    }

    // 🔹 Boîte de confirmation avant suppression
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Voulez-vous vraiment supprimer la compagne \"" + nom + "\" ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (compagneTmp.supprimer(nom)) {
            QMessageBox::information(this, "✅ Succès", "Compagne supprimée avec succès !");

            // 🔹 Effacer les champs après suppression
            ui->nom_saisie->clear();

            // 🔹 Rafraîchir l'affichage après suppression
            displayCompagne();
        } else {
            QMessageBox::critical(this, "❌ Erreur", "Échec de la suppression !");
        }
    }
}



//metier 1

void MainWindow::on_calendar_campaigns_clicked(const QDate &date)
{
    chargerCompagnesParDate(date);
    alerterCompagnesImminentes();  // pour mettre à jour le QLabel
}



void MainWindow::chargerCompagnesParDate(const QDate &date)
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    QSqlQuery query;
    query.prepare("SELECT NOM_COMPAGNE, DATE_DEBUT, DATE_FIN, ZONE_GEOGRAPHIQUE, STATUT FROM COMPAGNE WHERE DATE_DEBUT <= :date AND DATE_FIN >= :date");
    query.bindValue(":date", date);
    query.exec();

    model->setQuery(std::move(query));  // ✅ éviter la copie (message warning)
    model->setHeaderData(0, Qt::Horizontal, "Nom");
    model->setHeaderData(1, Qt::Horizontal, "Début");
    model->setHeaderData(2, Qt::Horizontal, "Fin");
    model->setHeaderData(3, Qt::Horizontal, "Zone");
    model->setHeaderData(4, Qt::Horizontal, "Statut");

    ui->table_campaigns->setModel(model);
    ui->table_campaigns->resizeColumnsToContents();
}




void MainWindow::alerterCompagnesImminentes()
{
    QSqlQuery query("SELECT NOM_COMPAGNE, DATE_DEBUT FROM COMPAGNE");
    QDate aujourdHui = QDate::currentDate();

    QString message = "";

    while (query.next()) {
        QString nom = query.value(0).toString();
        QDate dateDebut = query.value(1).toDate();

        int joursRestants = aujourdHui.daysTo(dateDebut);

        if (joursRestants <= 3 && joursRestants >= 0) {
            message += "⚠️ La compagne \"" + nom + "\" commence dans " + QString::number(joursRestants) + " jour(s).\n";
        }
    }

    if (!message.isEmpty()) {
        ui->label_notification->setText(message);
        ui->label_notification->setStyleSheet("color: red; font-weight: bold;");
    } else {
        ui->label_notification->clear();
    }
}

void MainWindow::rechercherParDate(const QDate &date)
{
    QSqlQueryModel *model = new QSqlQueryModel(this);

    QSqlQuery query;
    query.prepare("SELECT NOM_COMPAGNE, DATE_DEBUT, DATE_FIN, ZONE_GEOGRAPHIQUE, STATUT "
                  "FROM COMPAGNE WHERE :date BETWEEN DATE_DEBUT AND DATE_FIN");
    query.bindValue(":date", date);

    if (!query.exec()) {
        qDebug() << "Erreur de recherche :" << query.lastError().text();
        return;
    }

    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, "Nom");
    model->setHeaderData(1, Qt::Horizontal, "Début");
    model->setHeaderData(2, Qt::Horizontal, "Fin");
    model->setHeaderData(3, Qt::Horizontal, "Zone");
    model->setHeaderData(4, Qt::Horizontal, "Statut");

    ui->table_campaigns->setModel(model);
    ui->table_campaigns->resizeColumnsToContents();
}








void MainWindow::on_btn_export_campaigns_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty())
        return;

    QPdfWriter pdf(fileName);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setResolution(300);

    QPainter painter(&pdf);
    if (!painter.isActive()) {
        QMessageBox::critical(this, "Erreur", "Impossible de créer le fichier PDF.");
        return;
    }

    QAbstractItemModel* model = ui->table_campaigns->model();
    if (!model) {
        qDebug() << "⚠️ Modèle vide !";
        return;
    }

    int margin = 50;
    int x = margin;
    int y = margin;

    QFont titleFont("Arial", 14, QFont::Bold);
    QFont dateFont("Arial", 10);
    QFont cellFont("Arial", 10);
    painter.setPen(Qt::black);

    // 🔹 Titre
    painter.setFont(titleFont);
    QRect titleRect(x, y, pdf.width() - 2 * margin, 80);
    painter.drawText(titleRect, Qt::AlignHCenter, "📋 Tableau des Compagnes de Vaccination");

    // 🔹 Date alignée à droite
    painter.setFont(dateFont);

    QString selectedDate = ui->calendar_campaigns->selectedDate().toString("dd/MM/yyyy");
    QString dateStr = "Date : " + selectedDate;

    QRect dateRect(x, y, pdf.width() - 2 * margin, 80);
    painter.drawText(dateRect, Qt::AlignRight, dateStr);

    // 🔹 Espacement entre titre/date et tableau
    y += 200;

    QFont headerFont = cellFont;
    headerFont.setBold(true);

    int rowHeight = 55;
    int colCount = model->columnCount();
    int colWidth = (pdf.width() - 2 * margin) / colCount;

    // 🔹 En-têtes du tableau

    painter.setFont(headerFont);  // 🟩 Texte en gras

    for (int col = 0; col < colCount; ++col) {
        QString header = model->headerData(col, Qt::Horizontal).toString();
        painter.drawRect(x + col * colWidth, y, colWidth, rowHeight);
        painter.drawText(QRect(x + col * colWidth, y, colWidth, rowHeight), Qt::AlignCenter, header);
    }

    painter.setFont(cellFont);  // 🔄 Revenir à texte normal
    y += rowHeight;

    // 🔹 Lignes de données
    for (int row = 0; row < model->rowCount(); ++row) {
        // ✅ Nouvelle page si dépasse
        if (y + rowHeight > pdf.height() - margin) {
            pdf.newPage();
            y = margin;

            // 🔁 Réafficher les en-têtes sur la nouvelle page
            for (int col = 0; col < colCount; ++col) {
                QString header = model->headerData(col, Qt::Horizontal).toString();
                painter.drawRect(x + col * colWidth, y, colWidth, rowHeight);
                painter.drawText(QRect(x + col * colWidth, y, colWidth, rowHeight), Qt::AlignCenter, header);
            }
            y += rowHeight;
        }

        for (int col = 0; col < colCount; ++col) {
            QString value = model->data(model->index(row, col)).toString();

            // 🔹 Supprimer "T00:00..." dans les dates
            if (value.contains("T"))
                value = value.section('T', 0, 0);

            painter.drawRect(x + col * colWidth, y, colWidth, rowHeight);
            painter.drawText(QRect(x + col * colWidth, y, colWidth, rowHeight), Qt::AlignCenter, value);
        }
        y += rowHeight;
    }

    painter.end();
    QMessageBox::information(this, "✅ PDF Exporté", "Le tableau a été exporté avec succès !");
}




//PDF


void MainWindow::genererPDFApprovisionnement(const Compagne& compagne)
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter Rapport Approvisionnement", "", "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    QPdfWriter pdf(fileName);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setResolution(300);

    QPainter painter(&pdf);
    if (!painter.isActive()) {
        QMessageBox::critical(this, "Erreur", "Impossible de créer le fichier PDF.");
        return;
    }

    QFont titleFont("Helvetica", 16, QFont::Bold);
    QFont sectionFont("Helvetica", 12, QFont::Bold);
    QFont contentFont("Helvetica", 11);
    painter.setPen(Qt::black);

    int xLeft = 60;
    int xRight = 750;
    int y = 180;
    int lineSpacing = 90;

    // ✅ 🔹 Logo SmartVacc
    QPixmap logo(":/images/logo.png");  // ton logo dans resources.qrc
    if (!logo.isNull()) {
        QRect targetRect(60, 20, 180, 180);  // position (x,y) et taille (w,h)
        painter.drawPixmap(targetRect, logo);
    }

    // 🔹 Titre centré
    painter.setFont(titleFont);
    painter.drawText(QRect(0, 150, pdf.width(), 90), Qt::AlignHCenter, "📦 Rapport d'Approvisionnement - Compagne Vaccination");
    y += 270;

    // 🔹 Nom de la compagne
    painter.setFont(sectionFont);
    painter.drawText(xLeft, y, "🆔 Nom de la compagne :");
    painter.setFont(contentFont);
    painter.drawText(xRight, y, compagne.getNomCompagne());
    y += lineSpacing;

    // 🔹 Vaccins utilisés
    painter.setFont(sectionFont);
    painter.drawText(xLeft, y, "💉 Vaccins utilisés :");
    painter.setFont(contentFont);
    painter.drawText(xRight - 170, y, compagne.getVaccinsUtilises());
    y += lineSpacing;

    // 🔹 Doses
    painter.setFont(sectionFont);
    painter.drawText(xLeft, y, "📊 Doses :");
    painter.setFont(contentFont);
    painter.drawText(xRight - 400, y, QString("%1 / %2").arg(compagne.getDosesAdministrees()).arg(compagne.getObjectifDoses()));
    y += lineSpacing;

    // 🔹 Fournitures nécessaires
    painter.setFont(sectionFont);
    painter.drawText(xLeft, y, "📋 Fournitures nécessaires :");
    painter.setFont(contentFont);
    painter.drawText(xRight + 40, y, compagne.getFournitures());
    y += lineSpacing;

    // 🔹 Coût estimé
    painter.setFont(sectionFont);
    painter.drawText(xLeft, y, "💵 Coût estimé :");
    painter.setFont(contentFont);
    painter.drawText(xRight - 260, y, QString::number(compagne.getCoutFournitures(), 'f', 2) + " TND");
    y += lineSpacing;

    // 🔹 Footer
    y += 100;
    painter.setFont(contentFont);
    painter.drawText(xLeft, y, "✒️ Signature & Cachet du Centre de Vaccination");

    painter.end();

    QMessageBox::information(this, "✅ PDF Exporté", "Le rapport d'approvisionnement a été exporté avec succès !");
}





//metier 2


void MainWindow::chargerEmployes() {
    mapEmployes.clear();
    QSqlQuery query("SELECT id_employe, nom FROM EMPLOYES");

    QStringList listeNoms;
    while (query.next()) {
        int id = query.value(0).toInt();
        QString nom = query.value(1).toString();
        listeNoms << nom;
        mapEmployes[nom] = id;
    }

    QCompleter *completer = new QCompleter(listeNoms, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->line_employe->setCompleter(completer);
}


void MainWindow::chargerCompagnes() {
    mapCompagnes.clear();
    QSqlQuery query("SELECT id_compagne, nom_compagne, date_debut, date_fin FROM COMPAGNE");

    QStringList listeCompagnes;
    while (query.next()) {
        int id = query.value(0).toInt();
        QString nom = query.value(1).toString();
        QString debut = query.value(2).toDate().toString("dd/MM/yyyy");
        QString fin = query.value(3).toDate().toString("dd/MM/yyyy");

        QString label = QString("%1 (%2 - %3)").arg(nom).arg(debut).arg(fin);
        listeCompagnes << label;
        mapCompagnes[label] = id;
    }

    QCompleter *completer = new QCompleter(listeCompagnes, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->line_compagne->setCompleter(completer);
}





bool MainWindow::aConflitAvecCompagne(int id_employe, QDate newDebut, QDate newFin) {
    QSqlQuery query;
    query.prepare(R"(
        SELECT date_debut, date_fin
        FROM COMPAGNE
        JOIN CONTRIBUER ON COMPAGNE.id_compagne = CONTRIBUER.id_compagne
        WHERE id_employe = :id
    )");
    query.bindValue(":id", id_employe);

    if (!query.exec()) return false;

    while (query.next()) {
        QDate debut = query.value(0).toDate();
        QDate fin = query.value(1).toDate();
        if (newDebut <= fin && debut <= newFin)
            return true;
    }

    return false;
}





void MainWindow::affecterEmployeACompagne() {
    QString nomEmploye = ui->line_employe->text().trimmed();
    QString nomCompagne = ui->line_compagne->text().trimmed();

    if (!mapEmployes.contains(nomEmploye) || !mapCompagnes.contains(nomCompagne)) {
        QMessageBox::warning(this, "Erreur", "Employé ou compagne invalide !");
        return;
    }

    int id_employe = mapEmployes[nomEmploye];
    int id_compagne = mapCompagnes[nomCompagne];

    // Charger les dates de la compagne
    QSqlQuery query;
    query.prepare("SELECT date_debut, date_fin FROM COMPAGNE WHERE id_compagne = :id");
    query.bindValue(":id", id_compagne);
    if (!query.exec() || !query.next()) return;

    QDate dateDebut = query.value(0).toDate();
    QDate dateFin = query.value(1).toDate();

    if (aConflitAvecCompagne(id_employe, dateDebut, dateFin)) {
        QMessageBox::warning(this, "⛔ Conflit", "Cet employé a déjà une compagne sur cette période !");
        return;
    }

    QSqlQuery insert;
    insert.prepare("INSERT INTO CONTRIBUER (id_employe, id_compagne) VALUES (:e, :c)");
    insert.bindValue(":e", id_employe);
    insert.bindValue(":c", id_compagne);

    if (insert.exec()) {
        QMessageBox::information(this, "✅ Succès", "Employé affecté à la compagne !");
    } else {
        QMessageBox::critical(this, "❌ Erreur", "Insertion échouée : " + insert.lastError().text());
    }
}





void MainWindow::afficherAffectations() {
    QWidget *contentWidget = new QWidget;
    QHBoxLayout *mainLayout = new QHBoxLayout(contentWidget);

    // 🟩 Colonne de gauche : Liste fixe des employés
    QWidget *listeWidget = new QWidget;
    QVBoxLayout *listeLayout = new QVBoxLayout(listeWidget);
    listeLayout->addWidget(new QLabel("👥 Liste des employés :"));

    QListWidget *sourceListeEmployes = new QListWidget(this);
    sourceListeEmployes->setDragEnabled(true);
    sourceListeEmployes->setSelectionMode(QAbstractItemView::SingleSelection);

    QSqlQuery employeQuery("SELECT ID_EMPLOYE, NOM, POSTE FROM EMPLOYES");
    while (employeQuery.next()) {
        int id = employeQuery.value(0).toInt();
        QString nom = employeQuery.value(1).toString();
        QString poste = employeQuery.value(2).toString();

        QListWidgetItem *item = new QListWidgetItem("👤 " + nom + " (" + poste + ")");
        item->setData(Qt::UserRole, id);
        sourceListeEmployes->addItem(item);
    }

    listeLayout->addWidget(sourceListeEmployes);
    mainLayout->addWidget(listeWidget);

    // 🟦 Colonnes des campagnes (avec DragDropListWidget)
    QSqlQuery compagnesQuery("SELECT ID_COMPAGNE, NOM_COMPAGNE, DATE_DEBUT, DATE_FIN FROM COMPAGNE ORDER BY DATE_DEBUT");

    while (compagnesQuery.next()) {
        int id_compagne = compagnesQuery.value(0).toInt();
        QString nom = compagnesQuery.value(1).toString();
        QString debut = compagnesQuery.value(2).toDate().toString("dd/MM/yyyy");
        QString fin = compagnesQuery.value(3).toDate().toString("dd/MM/yyyy");

        QString cle = nom + "\n(" + debut + " - " + fin + ")";

        QWidget *colonne = new QWidget;
        QVBoxLayout *colLayout = new QVBoxLayout(colonne);
        colLayout->addWidget(new QLabel("🗓️ " + cle));

        DragDropListWidget *listWidget = new DragDropListWidget(this, id_compagne);
        listWidget->setAcceptDrops(true);
        listWidget->setDragEnabled(true);  // ✅ Active le drag depuis la campagne
        listWidget->setDefaultDropAction(Qt::MoveAction);  // ✅ Permet le déplacement entre campagnes


        // Charger les employés affectés
        QSqlQuery affectationsQuery;
        affectationsQuery.prepare(R"(
            SELECT E.ID_EMPLOYE, E.NOM, E.POSTE
            FROM CONTRIBUER C
            JOIN EMPLOYES E ON C.ID_EMPLOYE = E.ID_EMPLOYE
            WHERE C.ID_COMPAGNE = :id
        )");
        affectationsQuery.bindValue(":id", id_compagne);

        if (!affectationsQuery.exec()) {
            qDebug() << "❌ Erreur chargement employés pour compagne" << id_compagne << ":" << affectationsQuery.lastError().text();
            continue;
        }

        while (affectationsQuery.next()) {
            int id_employe = affectationsQuery.value(0).toInt();
            QString nomEmp = affectationsQuery.value(1).toString();
            QString poste = affectationsQuery.value(2).toString();

            QListWidgetItem *item = new QListWidgetItem("👤 " + nomEmp + " (" + poste + ")");
            item->setData(Qt::UserRole, id_employe);
            listWidget->addItem(item);
        }

        colLayout->addWidget(listWidget);
        colonne->setLayout(colLayout);
        mainLayout->addWidget(colonne);
    }

    contentWidget->setLayout(mainLayout);
    ui->scroll_affectations->setWidget(contentWidget);
}



void MainWindow::validerAffectations() {
    QList<DragDropListWidget*> listes = this->findChildren<DragDropListWidget*>();

    bool conflitsTrouves = false;
    QString listeConflits;

    for (DragDropListWidget* liste : listes) {
        int id_compagne = liste->id_compagne;

        // Obtenir les dates de cette compagne
        QSqlQuery dateQuery;
        dateQuery.prepare("SELECT date_debut, date_fin FROM COMPAGNE WHERE id_compagne = :id");
        dateQuery.bindValue(":id", id_compagne);
        if (!dateQuery.exec() || !dateQuery.next()) continue;

        QDate dateDebut = dateQuery.value(0).toDate();
        QDate dateFin = dateQuery.value(1).toDate();

        for (int i = 0; i < liste->count(); ++i) {
            QListWidgetItem* item = liste->item(i);
            int id_employe = item->data(Qt::UserRole).toInt();

            // 🔁 Vérifie si l'affectation existe déjà
            QSqlQuery checkQuery;
            checkQuery.prepare("SELECT COUNT(*) FROM CONTRIBUER WHERE id_employe = :e AND id_compagne = :c");
            checkQuery.bindValue(":e", id_employe);
            checkQuery.bindValue(":c", id_compagne);
            checkQuery.exec();
            checkQuery.next();

            if (checkQuery.value(0).toInt() == 0) {
                // ✅ Nouvel employé à ajouter → Vérifier conflit
                if (aConflitAvecCompagne(id_employe, dateDebut, dateFin)) {
                    conflitsTrouves = true;
                    listeConflits += "❌ Employé ID " + QString::number(id_employe)
                                     + " déjà affecté à une compagne durant cette période.\n";
                    continue;
                }

                // ✅ Pas de conflit, on l’insère
                QSqlQuery insert;
                insert.prepare("INSERT INTO CONTRIBUER (id_employe, id_compagne) VALUES (:e, :c)");
                insert.bindValue(":e", id_employe);
                insert.bindValue(":c", id_compagne);
                if (!insert.exec()) {
                    qDebug() << "Erreur INSERT :" << insert.lastError().text();
                }
            }
        }
    }

    if (conflitsTrouves) {
        QMessageBox::warning(this, "⛔ Conflits détectés", listeConflits);
    } else {
        QMessageBox::information(this, "✅ Succès", "Toutes les affectations ont été enregistrées sans conflit !");
    }
}






//statistique

void MainWindow::afficherStatistiquesVaccinationParZone() {
    qDebug() << "👉 Statistiques vaccination par zone : lancement";

    if (!ui->chartVaccZone || !ui->resume_vaccination_zone) {
        qDebug() << "❌ Widgets manquants.";
        return;
    }

    QPieSeries *series = new QPieSeries();
    qDebug() << "Étape 1 : QPieSeries OK";

    QString resume = "📊 Résumé par zone :<br><br>";

    QSqlQuery query;
    query.prepare(R"(
        SELECT ZONE_GEOGRAPHIQUE, SUM(DOSES_ADMINISTREES), SUM(OBJECTIF_DOSES)
        FROM COMPAGNE
        GROUP BY ZONE_GEOGRAPHIQUE
    )");

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL : " << query.lastError().text();
        return;
    }

    qDebug() << "Étape 2 : Requête exécutée avec succès";

    while (query.next()) {
        QString zone = query.value(0).toString();
        int administrees = query.value(1).toInt();
        int objectif = query.value(2).toInt();

        qDebug() << "-> Zone : " << zone << "| Doses : " << administrees << "/" << objectif;

        if (objectif == 0) {
            qDebug() << "‼️ Objectif = 0 → Ignorer la zone : " << zone;
            continue;
        }

        float taux = (administrees * 100.0 / objectif);
        QString statut = (taux >= 100) ? "✅ Objectif atteint" : "⚠️ Non atteint";

        QPieSlice *slice = series->append(zone + " (" + QString::number(taux, 'f', 1) + "%)", taux);
        slice->setLabelVisible(true);
        QColor color = QColor::fromHsv(QRandomGenerator::global()->bounded(360), 200, 255);
        slice->setBrush(color);

        if (taux < 50.0) {
            slice->setExploded(true);
            slice->setLabelColor(Qt::red);
        }

        resume += QString("🌍 <b>%1</b> : <span style='color:%2;'>%3%</span> (%4)<br>")
                      .arg(zone)
                      .arg((taux >= 100) ? "green" : (taux >= 60) ? "orange" : "red")
                      .arg(QString::number(taux, 'f', 1))
                      .arg(statut);
    }


    qDebug() << "Étape 4 : Top 3 calculé";

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("📊 Taux de Couverture Vaccinale par Zone (%)");
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setAnimationOptions(QChart::AllAnimations);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    if (ui->chartVaccZone->layout()) {
        QLayoutItem *item;
        while ((item = ui->chartVaccZone->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        ui->chartVaccZone->layout()->addWidget(chartView);
    } else {
        QVBoxLayout *layout = new QVBoxLayout();
        layout->addWidget(chartView);
        ui->chartVaccZone->setLayout(layout);
    }

    ui->resume_vaccination_zone->setHtml(resume);

    qDebug() << "✅ Fin du calcul des statistiques";
}




void MainWindow::trierCompagnes(const QString &critere)
{
    QString orderBy;

    if (critere == "Date_debut") {
        orderBy = "DATE_DEBUT";
    } else if (critere == "statut") {
        orderBy = "STATUT";
    } else if (critere == "doses_administrees") {
        orderBy = "DOSES_ADMINISTREES";
    } else {
        orderBy = "ID_COMPAGNE";
    }

    QString queryString = "SELECT * FROM COMPAGNE ORDER BY " + orderBy;
    qDebug() << "🔍 Requête SQL triée :" << queryString;

    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(queryString);

    if (model->lastError().isValid()) {
        qDebug() << "❌ Erreur SQL :" << model->lastError().text();
        QMessageBox::critical(this, "Erreur", "Erreur de tri !");
        return;
    }

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
}



//recherche

void MainWindow::rechercherCompagnes(const QString &texte)
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    QSqlQuery query;

    QString filtre = "%" + texte + "%";

    query.prepare(R"(
        SELECT * FROM COMPAGNE
        WHERE LOWER(NOM_COMPAGNE) LIKE LOWER(:filtre)
           OR LOWER(VACCINS_UTILISES) LIKE LOWER(:filtre)
           OR LOWER(ZONE_GEOGRAPHIQUE) LIKE LOWER(:filtre)
    )");

    query.bindValue(":filtre", filtre);

    if (!query.exec()) {
        qDebug() << "❌ Erreur recherche :" << query.lastError().text();
        return;
    }

    model->setQuery(std::move(query));
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
}



//Annuler

void MainWindow::on_pushButton_annuler_clicked()
{
    if (modeModification) {
        // 🔁 Recharger les données d'origine
        if (compagneTmp.chargerCompagne(nomOriginal)) {
            ui->nom_compagne->setText(compagneTmp.getNomCompagne());
            ui->date_debut->setDate(compagneTmp.getDateDebut());
            ui->date_fin->setDate(compagneTmp.getDateFin());
            ui->zone_geographique->setText(compagneTmp.getZoneGeographique());
            ui->objectif_doses->setText(QString::number(compagneTmp.getObjectifDoses()));
            ui->doses_administrees->setText(QString::number(compagneTmp.getDosesAdministrees()));
            ui->vaccins_utilises->setCurrentText(compagneTmp.getVaccinsUtilises());
            ui->statut->setCurrentText(compagneTmp.getStatut());
            ui->fournitures->setText(compagneTmp.getFournitures());
            ui->cout_fournitures->setText(QString::number(compagneTmp.getCoutFournitures(), 'f', 2));

            QMessageBox::information(this, "Restauré", "Les anciennes données de la compagne ont été restaurées.");
        } else {
            QMessageBox::warning(this, "Erreur", "Impossible de recharger les données de la compagne.");
        }
    } else {
        // 🧹 Vider tous les champs
        ui->nom_compagne->clear();
        ui->date_debut->setDate(QDate::currentDate());
        ui->date_fin->setDate(QDate::currentDate());
        ui->zone_geographique->clear();
        ui->objectif_doses->clear();
        ui->doses_administrees->clear();
        ui->vaccins_utilises->setCurrentIndex(0);
        ui->statut->setCurrentIndex(0);
        ui->fournitures->clear();
        ui->cout_fournitures->clear();

        QMessageBox::information(this, "Réinitialisé", "Le formulaire a été vidé.");
    }
}






