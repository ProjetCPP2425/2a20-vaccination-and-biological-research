#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include "laboratoire.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>
#include <QMap>
#include <QString>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>
#include <QVBoxLayout>
#include <QFileDialog>

#include <QTextStream>
#include <QFile>
#include <QDate>
#include <QDesktopServices>
#include <QUrl>
#include <QHostAddress>

#include <QDebug>
#include <QDataStream>

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <QPixmap>
#include <QLabel>



#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDataStream>
#include <QSqlDatabase>


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

    connect(ui->lineEdit_NomLab, &QLineEdit::textChanged, this, &MainWindow::validateFields);
    connect(ui->lineEdit_Adresse, &QLineEdit::textChanged, this, &MainWindow::validateFields);
    connect(ui->lineEdit_Type, &QLineEdit::textChanged, this, &MainWindow::validateFields);

    //recherche
    ui->rechercherLabo->setPlaceholderText("Rechercher par nom du laboratoire, responsable ou statut...");
    connect(ui->rechercherLabo, &QLineEdit::textChanged, this, &MainWindow::rechercherLabo);

    //tri
        connect(ui->comboBox_Tri, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_comboBox_Tri_currentIndexChanged);
    //stat
    connect(ui->Affichage_3, &QTabWidget::currentChanged, this, &MainWindow::onTabWidgetPageChanged);

    connect(ui->pushButtonStat_16, &QPushButton::clicked, this, &MainWindow::displayStatisticsChart);

    connect(ui->pushButtonStat_16, &QPushButton::clicked, this, &MainWindow::goToStatistiques);
  //pdf
    connect(ui->btnExportExcel, &QPushButton::clicked, this, &MainWindow::onExcelClicked);

  //maps
    connect(ui->pushButton_search, &QPushButton::clicked, this, &MainWindow::showMap);


    //chatroom
    // Connect actions for selecting a lab and sending messages
    connect(ui->pushButton_selectLab, &QPushButton::clicked, this, &MainWindow::on_selectLab_clicked);
    connect(ui->pushButton_sendMessage, &QPushButton::clicked, this, &MainWindow::on_sendMessage_clicked);
     ui->lineEdit_message->setPlaceholderText("Veuillez écrire ici...");

    // Populate lab name combo box on initialization
   // populateLabNameComboBox();
    TCPSocket = new QTcpSocket(this);
     TCPSocket->connectToHost(QHostAddress::LocalHost, 1234);




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
        populateResponsableComboBox();  // <- make sure to call it here
        populateLabNameComboBox();
        displayLaboratoires();
        populateNomlabComboBox();
        Read_Data_From_Socket();



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
    if (TCPSocket && TCPSocket->isOpen()) {
        TCPSocket->close();
    }
    delete ui;
}
// Ajouter Laboratoire
void MainWindow::on_pushButton_32_clicked()
{
    QString nom = ui->lineEdit_NomLab->text();
    QString adresse = ui->lineEdit_Adresse->text();
    QString responsable = ui->comboBox_Responsable->currentText();

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
    ui->comboBox_Statut->setCurrentIndex(0);

    ui->lineEdit_Type->clear();
    ui->comboBox_Statut->setCurrentIndex(0);
    ui->spinBox_NbProjets->setValue(0);
    ui->spinBox_NbProjets_2->setValue(0);
    ui->spinBox_NbProjets_3->setValue(0);
    ui->doubleSpinBox_7->setValue(0.00);
    ui->dateEdit_7->setDate(QDate::currentDate());




    populateResponsableComboBox();
    ui->comboBox_Responsable->setCurrentIndex(0);

}


void MainWindow::displayLaboratoires()
{
    QSqlTableModel *model = labTmp.afficher();

    if (model) {
        ui->tableView->setModel(model);
        ui->tableView->hideColumn(0); // Hides the NUM (ID) column
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
        QString responsable = query.value("RESPONSABLE").toString();
       ui->comboBox_Responsable->setCurrentText(responsable);

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


  //n3abi combox fil page ajouter


void MainWindow::populateResponsableComboBox()
{
    ui->comboBox_Responsable->clear();
    ui->comboBox_Responsable->addItem("Choisissez un responsable dans la liste:");

    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM FROM SMARTVACC.EMPLOYES WHERE POSTE = 'Responsable'");

    if (query.exec()) {
        while (query.next()) {
            QString nomPrenom = query.value(0).toString() + " " + query.value(1).toString();
            ui->comboBox_Responsable->addItem(nomPrenom);
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération des responsables.");
    }

    ui->comboBox_Responsable->setCurrentIndex(0);
}




  //controle de siasie
void MainWindow::validateFields()
{
    // Regex: Only letters and spaces
    QRegularExpression regexText("^[A-Za-z\\s]*$");
    QString nom = ui->lineEdit_NomLab->text();
    QString adresse = ui->lineEdit_Adresse->text();
    QString type = ui->lineEdit_Type->text();

    QString redStyle = "border: 2px solid red; border-radius: 5px; background-color: transparent; box-shadow: 0px 0px 10px red; border-style: dashed;";
    QString greenStyle = "border: 2px solid green; border-radius: 5px; background-color: transparent; box-shadow: 0px 0px 10px green; border-style: dashed;";


    // Validation Nom
    // Validation Nom
    if (!regexText.match(nom).hasMatch() || nom.trimmed().isEmpty()) {
        ui->label_NomError->setText("❗ Le nom ne doit contenir que des lettres (pas de chiffres ou symboles)!!!");
        ui->label_NomError->setStyleSheet("color: red; background: none;");
        ui->lineEdit_NomLab->setStyleSheet(redStyle);
    } else {
        ui->label_NomError->setText("✅ Nom valide.");
        ui->label_NomError->setStyleSheet("color: green; background: none;");
        ui->lineEdit_NomLab->setStyleSheet(greenStyle);
    }

    // Validation Adresse
    if (adresse.trimmed().isEmpty()) {
        ui->label_AdresseError->setText("❗ L'adresse ne peut pas être vide!!!");
        ui->label_AdresseError->setStyleSheet("color: red; background: none;");
        ui->lineEdit_Adresse->setStyleSheet(redStyle);

    } else {
        ui->label_AdresseError->setText("✅ Adresse valide.");
        ui->label_AdresseError->setStyleSheet("color: green; background: none;");
        ui->lineEdit_Adresse->setStyleSheet(greenStyle);
    }

    // Validation Type
    if (!regexText.match(type).hasMatch() || type.trimmed().isEmpty()) {
        ui->label_TypeError->setText("❗ Le type ne doit contenir que des lettres (pas de chiffres ou symboles)!!!");
        ui->label_TypeError->setStyleSheet("color: red; background: none;");
        ui->lineEdit_Type->setStyleSheet(redStyle);
    } else {
        ui->label_TypeError->setText("✅ Type valide.");
        ui->label_TypeError->setStyleSheet("color: green; background: none;");
        ui->lineEdit_Type->setStyleSheet(greenStyle);
    }

}

  //recherche

void MainWindow::rechercherLabo(const QString &searchText) {
    QString searchTerm = searchText.trimmed();

    QSqlTableModel *model = new QSqlTableModel(this);
    model->setTable("SMARTVACC.LABORATOIRES");

    if (!searchTerm.isEmpty()) {
        QString filter = QString(
                             "NOM_LAB LIKE '%%1%' OR RESPONSABLE LIKE '%%1%' OR STATUT LIKE '%%1%'"
                             ).arg(searchTerm);
        model->setFilter(filter);

        // Alphabetically sort by STATUT
        model->setSort(model->fieldIndex("STATUT"), Qt::AscendingOrder);
    } else {
        model->setFilter("");
    }

    model->select();
    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);  // Optional: hide ID
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setVisible(true);  // 💥 Ensure table stays visible
}


 //tri

void MainWindow::on_comboBox_Tri_currentIndexChanged()
{
    QString sortBy = ui->comboBox_Tri->currentText();

    QSqlQueryModel *model = new QSqlQueryModel();

    QString query = "SELECT * FROM SMARTVACC.LABORATOIRES";  // Select all columns (includes ID_LAB and NOM_LAB)

    if (sortBy == "Statut") {
        query += " ORDER BY CASE STATUT "
                 "WHEN 'Fonctionnel' THEN 1 "
                 "WHEN 'Maintenance' THEN 2 "
                 "WHEN 'Non-Fonctionnel' THEN 3 "
                 "ELSE 4 END";
    }
    else if (sortBy == "Nb_Projets") {
        query += " ORDER BY NB_PROJETS DESC";
    }
    else if (sortBy == "Type") {
        query += " ORDER BY TYPE ASC";
    }

    model->setQuery(query);

    ui->tableView->setModel(model);

    // ✅ Hide only ID_LAB (column 0)
    ui->tableView->setColumnHidden(0, true);

    // ✅ Make sure NOM_LAB (column 1) and others are shown
    for (int col = 1; col < model->columnCount(); ++col) {
        ui->tableView->setColumnHidden(col, false);
    }

    // ✅ Ensure table stays visible and nicely formatted
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setVisible(true); // Force show the table if needed
}





  //stat
QMap<QString, LaboStats> MainWindow::fetchLaboStatistics()
{
    QMap<QString, LaboStats> statsMap;
    QSqlQuery query;
    // Adjust table/column names if needed.
    query.prepare("SELECT NOM_LAB, NB_PROJETS, PERSONNEL, MATRIELS, DEPONSE "
                  "FROM SMARTVACC.LABORATOIRES");
    if (query.exec()) {
        while (query.next()) {
            QString nom = query.value("NOM_LAB").toString();
            LaboStats stats;
            stats.nbProjets = query.value("NB_PROJETS").toInt();
            stats.personnel = query.value("PERSONNEL").toInt();
            stats.matriels  = query.value("MATRIELS").toInt();
            stats.depense   = query.value("DEPONSE").toFloat();
            statsMap[nom]   = stats;
        }
    } else {
        qDebug() << "Error fetching statistics:" << query.lastError().text();
    }
    return statsMap;
}
void MainWindow::displayStatisticsChart()
{
    QMap<QString, LaboStats> laboStats = fetchLaboStatistics();
    if (laboStats.isEmpty()) {
        qDebug() << "No laboratory data found!";
        return;
    }

    // Total sums
    float totalProjets = 0, totalPersonnel = 0, totalMatriels = 0, totalDepense = 0;

    // Bar sets
    QBarSet *setProjets   = new QBarSet("Nb Projets");
    QBarSet *setPersonnel = new QBarSet("Personnel");
    QBarSet *setMatriels  = new QBarSet("Matériels");
    QBarSet *setDepense   = new QBarSet("Dépense");

    QStringList labNames;
    QString bestLabName;
    float bestPerformanceIndex = -1;

    // First pass: calculate totals
    for (auto it = laboStats.begin(); it != laboStats.end(); ++it) {
        LaboStats stats = it.value();
        totalProjets   += stats.nbProjets;
        totalPersonnel += stats.personnel;
        totalMatriels  += stats.matriels;
        totalDepense   += stats.depense;
    }

    // Second pass: populate sets with percentage values
    for (auto it = laboStats.begin(); it != laboStats.end(); ++it) {
        QString labName = it.key();
        LaboStats stats = it.value();

        labNames << labName;

        *setProjets   << (totalProjets   > 0 ? (100.0 * stats.nbProjets / totalProjets)   : 0);
        *setPersonnel << (totalPersonnel > 0 ? (100.0 * stats.personnel / totalPersonnel) : 0);
        *setMatriels  << (totalMatriels  > 0 ? (100.0 * stats.matriels / totalMatriels)   : 0);
        *setDepense   << (totalDepense   > 0 ? (100.0 * stats.depense / totalDepense)     : 0);

        float denominator = stats.personnel + stats.matriels + stats.depense;
        float index = (denominator > 0) ? (stats.nbProjets / denominator) : 0;

        if (index > bestPerformanceIndex) {
            bestPerformanceIndex = index;
            bestLabName = labName;
        }
    }

    // Set custom colors
    setProjets->setColor(QColor("#3498db"));    // Blue
    setPersonnel->setColor(QColor("#2ecc71"));  // Green
    setMatriels->setColor(QColor("#f1c40f"));   // Yellow
    setDepense->setColor(QColor("#e74c3c"));    // Red

    // Create series and add sets
    QBarSeries *series = new QBarSeries();
    series->append(setProjets);
    series->append(setPersonnel);
    series->append(setMatriels);
    series->append(setDepense);
    series->setBarWidth(0.6);  // Reduce bar width for a more compact appearance

    // Create chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques des Laboratoires (% par catégorie)\n🏆 Meilleur labo : " + bestLabName);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // X-axis: lab names
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(labNames);
    axisX->setTitleText("Nom du Laboratoire");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Y-axis: percentage
    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelFormat("%.0f%%");     // Show as percentages
    axisY->setRange(0, 100);             // 0% to 100%
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Chart view
    QChartView *chartView = new QChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Resize the chart view to make it smaller
    chartView->setFixedWidth(1200);  // Set desired width (smaller than the original)
    chartView->setFixedHeight(650); // Optional: Set a fixed height if needed

    // Inject chartView into Statistique_7 layout
    QLayout *layout = ui->Statistique_7->layout();
    if (!layout) {
        layout = new QVBoxLayout(ui->Statistique_7);
        ui->Statistique_7->setLayout(layout);
    }

    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }
    layout->addWidget(chartView);

    // Display color legend next to the performance index
    QLabel *legendLabel = new QLabel("🟦 Projets | 🟩 Personnel | 🟨 Matériels | 🟥 Dépense");
    ui->Statistique_7->layout()->addWidget(legendLabel);

    // Print best performing lab
    qDebug() << "🏅 Meilleur labo basé sur l'indice de performance:" << bestLabName
             << "avec un indice de" << bestPerformanceIndex;
}



void MainWindow::onTabWidgetPageChanged(int index)
{
    // Replace '2' with the actual index of Statistique_7 tab.
    if (index == 2) {
        displayStatisticsChart();
    }
}
void MainWindow::goToStatistiques()
{
    // Assuming 'Statistiques_7' is the widget for your statistics page.
    ui->Affichage_3->setCurrentWidget(ui->Statistique_7);

    // Optionally, refresh or display the chart if needed:
    displayStatisticsChart();
}




  //excel
void MainWindow::onExcelClicked() {
    QString cheminFichier = QFileDialog::getSaveFileName(this, "Exporter vers Excel", "", "Fichiers Excel (*.xls)");

    if (!cheminFichier.isEmpty()) {
        if (exporterLabsVersExcel(cheminFichier)) {
            QMessageBox::information(this, "Exportation réussie", "Les données des laboratoires ont été exportées avec succès.");
        } else {
            QMessageBox::critical(this, "Erreur", "L'exportation a échoué.");
        }
    }
}
bool MainWindow::exporterLabsVersExcel(const QString& cheminFichier) {
    QSqlQuery query("SELECT NOM_LAB, RESPONSABLE, DEPONSE, PERSONNEL, MATRIELS, NB_PROJETS, DATE_CREATION FROM SMARTVACC.LABORATOIRES");
    QFile fichier(cheminFichier);

    if (!fichier.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream flux(&fichier);

    // En-têtes
    flux << "Nom du Laboratoire:\tResponsable\tDepenses \tPersonnel \tEquipements \tProjets Actifs\tDate de Creation\n";

    // Données
    while (query.next()) {
        flux << query.value("NOM_LAB").toString() << "\t"
             << query.value("RESPONSABLE").toString() << "\t"
             << query.value("DEPONSE").toDouble() << "\t"
             << query.value("PERSONNEL").toInt() << "\t"
             << query.value("MATRIELS").toInt() << "\t"
             << query.value("NB_PROJETS").toInt() << "\t"
             << query.value("DATE_CREATION").toDate().toString("yyyy-MM-dd") << "\n";
    }

    fichier.close();
    return true;

}

 //n3abi el el box mta3 nom lab lil localisation (map)
void MainWindow::populateNomlabComboBox()
{
    ui->comboBox_nomlab->clear();
    ui->comboBox_nomlab->addItem("Choisissez un laboratoire dans la liste:");

    QSqlQuery query;
    query.prepare("SELECT NOM_LAB FROM SMARTVACC.LABORATOIRES");

    if (query.exec()) {
        while (query.next()) {
            QString nom = query.value(0).toString();  // Use NOM_LAB only
            ui->comboBox_nomlab->addItem(nom);
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération des responsables.");
    }

    ui->comboBox_nomlab->setCurrentIndex(0);
}





 //map
void MainWindow::showMap() {
    QString nom = ui->comboBox_nomlab->currentText();

    QSqlQuery query;
    query.prepare("SELECT ADRESSE FROM SMARTVACC.LABORATOIRES WHERE NOM_LAB = :nom");
    query.bindValue(":nom", nom);

    if (!query.exec()) {
        QMessageBox::warning(this, "DB Error", query.lastError().text());
        return;
    }

    QString address;
    if (query.next()) {
        address = query.value(0).toString();
    } else {
        QMessageBox::warning(this, "Address Error", "Lab address not found.");
        return;
    }

    QString encodedAddress = QUrl::toPercentEncoding(address);
    QString geoUrl = QString("https://nominatim.openstreetmap.org/search?q=%1&format=json").arg(encodedAddress);

    QUrl url(geoUrl);  // ✅ Correct QUrl
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkRequest request(url);  // ✅ Correct declaration
    request.setHeader(QNetworkRequest::UserAgentHeader, "QtApp");

    QNetworkReply* reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() != QNetworkReply::NoError) {
            QMessageBox::warning(this, "Geocoding Error", reply->errorString());
            reply->deleteLater();
            manager->deleteLater();
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        if (!doc.isArray() || doc.array().isEmpty()) {
            QMessageBox::warning(this, "Geocoding", "Location not found.");
            reply->deleteLater();
            manager->deleteLater();
            return;
        }

        QJsonObject location = doc.array()[0].toObject();
        double lat = location["lat"].toString().toDouble();
        double lon = location["lon"].toString().toDouble();

        // 🗺️ Geoapify static map with marker
        QString mapUrl = QString("https://maps.geoapify.com/v1/staticmap?style=osm-carto&center=lonlat:%2,%1&zoom=16&size=1200x800&marker=lonlat:%2,%1;type:material;color:%233c00ff&apiKey=294bd34925934104854ce7e72dda7b81")
                             .arg(lat)
                             .arg(lon);

        QUrl mapQUrl(mapUrl);  // ✅ Proper QUrl
        QNetworkRequest mapRequest(mapQUrl);  // ✅ Correct declaration
        QNetworkReply* mapReply = manager->get(mapRequest);

        connect(mapReply, &QNetworkReply::finished, this, [=]() {
            if (mapReply->error() == QNetworkReply::NoError) {
                QPixmap pix;
                pix.loadFromData(mapReply->readAll());
                ui->label_map->setPixmap(pix.scaled(ui->label_map->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

            } else {
                QMessageBox::warning(this, "Map Error", mapReply->errorString());
            }
            mapReply->deleteLater();
            manager->deleteLater();
        });

        reply->deleteLater();
    });
}




 //chatrom
// Function to populate the combo box with lab names (NOM_LAB) from the database
void MainWindow::populateLabNameComboBox()
{
    ui->comboBox_labName->clear();  // Clear any existing items
    ui->comboBox_labName->addItem("👉 Choisissez un laboratoire :");  // Default prompt

    QSqlQuery query;
    query.prepare("SELECT NOM_LAB FROM SMARTVACC.LABORATOIRES");

    if (query.exec()) {
        if (query.size() == 0) {
            ui->comboBox_labName->addItem("Aucun laboratoire trouvé.");
        } else {
            while (query.next()) {
                QString labName = query.value(0).toString();  // Get NOM_LAB
                ui->comboBox_labName->addItem(labName);  // Add lab name to combo box
            }
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la récupération des laboratoires.");
    }

    ui->comboBox_labName->setCurrentIndex(0);  // Set the default selection to the first item
}

// Function to handle the lab selection button click
void MainWindow::on_selectLab_clicked()
{
    // Check if a valid lab is selected
    if (ui->comboBox_labName->currentIndex() > 0) {
        QString selectedLab = ui->comboBox_labName->currentText();
        QMessageBox::information(this, "Laboratoire sélectionné", "Vous avez sélectionné: " + selectedLab);
    } else {
        QMessageBox::warning(this, "Erreur de sélection", " Veuillez choisir un laboratoire dans la liste.");
    }
}

// Function to handle the send message button click
// Function to handle the send message button click
// Function to handle the send message button click
void MainWindow::on_sendMessage_clicked()
{
    // Get the selected lab and message to send
    QString labName = ui->comboBox_labName->currentText();
    QString message = ui->lineEdit_message->text();

    if (labName.isEmpty() || labName == "Veuillez sélectionner un laboratoire.") {
        QMessageBox::warning(this, "Erreur de sélection", "Veuillez choisir un laboratoire dans la liste.");
        return;
    }

    if (message.isEmpty()) {
        QMessageBox::warning(this, "Erreur de message", "Veuillez entrer un message.");
        return;
    }

    if (TCPSocket && TCPSocket->state() == QAbstractSocket::ConnectedState) {
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_7);
        out << QString("Message from %1: %2").arg(labName).arg(message);
        TCPSocket->write(data);

        // Make the lab name bold (gras) and in blue color, message in bigger font
        QString formattedMessage = QString("<font color='red' style='font-size: 14px;'><b>%1</b></font>: <font style='font-size: 16px;'>%2</font>")
                                       .arg(labName)  // Lab name in bold and blue
                                       .arg(message);  // Message in larger font

        ui->textEdit_messages->append(formattedMessage);  // Show formatted message in the chat

        ui->lineEdit_message->clear();  // Clear message input
    } else {
        QMessageBox::critical(this, "Erreur de connexion", "Non connecté au serveur.");
    }
}

// Function to read data from the socket
void MainWindow::Read_Data_From_Socket()
{
    if (TCPSocket && TCPSocket->state() == QAbstractSocket::ConnectedState) {
        QByteArray data = TCPSocket->readAll();
        QString message = QString::fromUtf8(data);
        if (!message.isEmpty()) {
            // Set server message text style
            QString formattedMessage = QString("<font color='green' style='font-size: 16px;'>Server: %1</font>").arg(message);
            ui->textEdit_messages->append(formattedMessage);  // Display received message with custom format
        }
    }
}
