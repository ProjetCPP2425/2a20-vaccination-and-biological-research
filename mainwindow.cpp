#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "employes.h"
#include <QMessageBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QDebug>
#include <QSqlError>
#include <QRegularExpression>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QRandomGenerator>
#include <QTimer>
#include <QTextDocument>
#include <QPdfWriter>
#include <QPainter>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QPropertyAnimation>
//#include <QAbstractAnimation>
//#include <QPropertyAnimation>
#include <QEasingCurve>

#include "smsnotif.h"

#include <QDateTime>
#include <QMessageBox>
#include <QShortcut>
#include <QToolBar>
#include <QAction>     // aussi nécessaire si tu ajoutes des actions
#include <QIcon>       // pour utiliser des icônes
#include <QKeySequence>  // pour les raccourcis clavier
#include <QProcess>
#include <QCompleter>

#include <QAbstractItemModel>


#include <QTextDocument> // À mettre tout en haut si pas encore présent

#include <QCompleter>

#include <QRandomGenerator>

#include <QThread>

//daoussar


#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

// === 📦 Fichiers pour PDF et affichage HTML ===
#include <QPdfWriter>
#include <QPainter>
#include <QTextDocument>
#include <QFileDialog>
#include <QAbstractItemModel>
#include <QCompleter>

// === 📊 Statistiques avec QtCharts ===
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QRandomGenerator>



//mayssem
#include "Qrcode.hpp"
#include "email.h"

#include "laboratoire.h"


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


#include <QFile>
#include <QDate>
#include <QDesktopServices>
#include <QUrl>
#include <QHostAddress>


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


#include <QAxObject>
#include <QVariant>

//eya
#include "vaccin.h"
#include <QMessageBox>
#include <QHBoxLayout>  // Pour QHBoxLayout
#include <QPushButton>  // Pour QPushButton
#include <QWidget>      // Pour QWidget
#include <QDebug>       // (Facultatif) Pour le debug
#include <QSqlError>  // Ajoute cette ligne pour afficher les erreurs SQL
#include <QSystemTrayIcon>
#include <QApplication>
#include <QDate>
#include <QSqlQuery>
#include <QTimer>
#include <QTextDocument>
#include <QFileDialog>
#include <QtPrintSupport/QPrinter>

#include <QTextDocument>
#include <QDate>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QChartView>
#include <QBarSet>
#include <QBarSeries>
#include <QVBoxLayout>
#include <QBarCategoryAxis>
#include <QValueAxis>


// Constructeur de MainWindow
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
     ui->frame_chatbox->hide();


    connect(ui->btn_toggle_password, &QPushButton::clicked, this, [=]() {
        if (ui->lineedit_password->echoMode() == QLineEdit::Password) {
            ui->lineedit_password->setEchoMode(QLineEdit::Normal);
            ui->btn_toggle_password->setText("🙈"); // ou une autre icône
        } else {
            ui->lineedit_password->setEchoMode(QLineEdit::Password);
            ui->btn_toggle_password->setText("👁️");
        }
    });


    ui->stackedWidget->setCurrentIndex(6);
    ui->frame->setVisible(false);

    // ✅ Validation en temps réel des champs employés
    connect(ui->lineedit_nom_employe, &QLineEdit::textChanged, this, [=]() {
        validateInput(ui->lineedit_nom_employe, ui->nomErrorLabel,
                      QRegularExpression("^[A-Za-zÀ-ÿ\\s-]+$"),
                      "Le nom doit contenir uniquement des lettres.");
    });

    connect(ui->lineedit_prenom_employe, &QLineEdit::textChanged, this, [=]() {
        validateInput(ui->lineedit_prenom_employe, ui->prenomErrorLabel,
                      QRegularExpression("^[A-Za-zÀ-ÿ\\s-]+$"),
                      "Le prénom doit contenir uniquement des lettres.");
    });

    connect(ui->lineedit_cin_employe, &QLineEdit::textChanged, this, [=]() {
        validateInput(ui->lineedit_cin_employe, ui->cinErrorLabel,
                      QRegularExpression("^[0-9]{8}$"),
                      "Le CIN doit contenir exactement 8 chiffres.");
    });

    connect(ui->lineeditcontact, &QLineEdit::textChanged, this, [=]() {
        validateInput(ui->lineeditcontact, ui->contactErrorLabel,
                      QRegularExpression("^[0-9]{8}$"),
                      "Le numéro de contact doit contenir exactement 8 chiffres.");
    });

    connect(ui->lineditsalaire, &QLineEdit::textChanged, this, [=]() {
        validateInput(ui->lineditsalaire, ui->salaireErrorLabel,
                      QRegularExpression("^[0-9]+(\\.[0-9]{1,2})?$"),
                      "Le salaire doit être un nombre valide.");
    });


    // Validation pour combobox_abscences
    connect(ui->combobox_abscences, &QComboBox::currentTextChanged, this, [=]() {
        if (ui->combobox_abscences->currentIndex() == 0) {
            ui->abscenceErrorLabel->setText("❌ Veuillez sélectionner un type d'absence.");
            ui->abscenceErrorLabel->setStyleSheet("color: red; font-weight: bold;");
        } else {
            ui->abscenceErrorLabel->setText("✔️ Valide");
            ui->abscenceErrorLabel->setStyleSheet("color: green; font-weight: bold;");
        }
    });

    // Validation pour combobox_disponibilite
    connect(ui->combobox_disponibilite, &QComboBox::currentTextChanged, this, [=]() {
        if (ui->combobox_disponibilite->currentIndex() == 0) {
            ui->disponibiliteErrorLabel->setText("❌ Veuillez sélectionner une disponibilité.");
            ui->disponibiliteErrorLabel->setStyleSheet("color: red; font-weight: bold;");
        } else {
            ui->disponibiliteErrorLabel->setText("✔️ Valide");
            ui->disponibiliteErrorLabel->setStyleSheet("color: green; font-weight: bold;");
        }
    });

    // Validation pour combobox_poste
    connect(ui->combobox_poste, &QComboBox::currentTextChanged, this, [=]() {
        if (ui->combobox_poste->currentIndex() == 0) {
            ui->posteErrorLabel->setText("❌ Veuillez sélectionner un poste.");
            ui->posteErrorLabel->setStyleSheet("color: red; font-weight: bold;");
        } else {
            ui->posteErrorLabel->setText("✔️ Valide");
            ui->posteErrorLabel->setStyleSheet("color: green; font-weight: bold;");
        }
    });


    // ✅ Réinitialisation des champs avec le bouton Annuler
    connect(ui->annuler, &QPushButton::clicked, this, &MainWindow::on_annuler_clicked);

    // ✅ Fonction pour gérer l'affichage du sidebar selon la page active
    auto updateSidebarVisibility = [=](int index) {
        ui->frame->setVisible(index != 6);
    };

    // ✅ Bouton retour à la connexion
    connect(ui->btn_login, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(6);
        updateSidebarVisibility(6);
    });

    connect(ui->pushButton_6, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(6);
        ui->frame->setVisible(false);
    });


    // ✅ Navigation entre les pages
    connect(ui->employe, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);
        ui->frame->setVisible(true);

        // Mettre à jour immédiatement l'affichage des employés
        Employe e;
        ui->tableView_2->setModel(e.afficher());
        chargerNomsDansComboBoxPDF();

        // Ajustements d'affichage
        ui->tableView_2->verticalHeader()->setDefaultSectionSize(35);
        ui->tableView_2->horizontalHeader()->setDefaultSectionSize(150);
        ui->tableView_2->resizeColumnsToContents();
        ui->tableView_2->setStyleSheet("QTableView_2::item { padding: 10px; }");
        ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

        connect(ui->pushButton_supprimer_employe, &QPushButton::clicked, this, &MainWindow::on_pushButton_supprimer_employe_clicked);
    });



    connect(ui->btnPredire, &QPushButton::clicked, this, &MainWindow::on_btnPredire_clicked);

    connect(ui->nom_carnet, &QLineEdit::textChanged, this, [=]() {
        validateInput(ui->nom_carnet, ui->nomErrorLabell, QRegularExpression("^[A-Za-zÀ-ÿ\\s-]+$"),
                      "Le nom ne doit contenir que des lettres et espaces.", false, false, -1, -1);
    });

    connect(ui->prenom_carnet, &QLineEdit::textChanged, this, [=]() {
        validateInput(ui->prenom_carnet, ui->prenomErrorLabell, QRegularExpression("^[A-Za-zÀ-ÿ\\s-]+$"),
                      "Le prénom ne doit contenir que des lettres et espaces.", false, false, -1, -1);
    });

    connect(ui->cin, &QLineEdit::textChanged, this, [=]() {
        validateInput(ui->cin, ui->cinErrorLabel, QRegularExpression("^[0-9]{8}$"),
                      "Le CIN doit contenir exactement 8 chiffres.", true, true, -1, -1);
    });

    connect(ui->num, &QLineEdit::textChanged, this, [=]() {
        validateInput(ui->num, ui->numErrorLabel, QRegularExpression("^[0-9]{8}$"),
                      "Le numéro doit contenir exactement 8 chiffres.", true, true, -1, -1);
    });

    connect(ui->poids, &QLineEdit::textChanged, this, [=]() {
        validateInput(ui->poids, ui->poidsErrorLabel, QRegularExpression("^[0-9]{1,3}(\\.[0-9]{1,2})?$"),
                      "Le poids doit être un nombre valide.", false, true, -1, -1);
    });

    connect(ui->age, &QLineEdit::textChanged, this, [=]() {
        validateInput(ui->age, ui->ageErrorLabel, QRegularExpression("^[0-9]{1,2}$"),
                      "L'âge doit être entre 1 et 100.", false, true, 1, 100);
    });


    connect(ui->remarques, &QTextEdit::textChanged, this, [=]() {
        validateTextEdit(ui->remarques, ui->remarquesErrorLabel, QRegularExpression("^.{0,200}$"), "Les remarques doivent contenir au moins 2 mots sans chiffres ni caractères spéciaux .");
    });

    connect(ui->annuler, &QPushButton::clicked, this, &MainWindow::on_annuler_clicked);



    //testSMS();
    // envoyerRappelSMS();

    /* smsTimer = new QTimer(this);
    connect(smsTimer, &QTimer::timeout, this, &MainWindow::envoyerRappelSMS);
    smsTimer->start(60000); // 60 000 ms = toutes les 60 secondes*///--->correcte
    connect(ui->comboTrierCarnets, &QComboBox::currentTextChanged, this, &MainWindow::trierCarnets);
    ui->comboTrierCarnets->addItem("Âge");
    ui->comboTrierCarnets->addItem("Date de RDV");
    ui->comboTrierCarnets->addItem("Poids");
    ui->rechercheC->setPlaceholderText("🔍 Recherche par CIN, nom, prénom,Statut_vaccinal");
    ui->rechercheC->setStyleSheet(R"(
    QLineEdit {
        color: black;
        font-size: 14px;
    }
    QLineEdit:empty {
        color: #B0B0B0; /* gris très clair pour placeholder uniquement */
    }
)");
    // Function to handle page changes

       displayCarnet();



    //mayssem

    connect(ui->pushButton_annulermay, &QPushButton::clicked, this, &MainWindow::clearAjoutFields);


    connect(ui->lineEdit_NomLab, &QLineEdit::textChanged, this, &MainWindow::validateFields);
    connect(ui->lineEdit_Adresse, &QLineEdit::textChanged, this, &MainWindow::validateFields);
    connect(ui->lineEdit_Type, &QLineEdit::textChanged, this, &MainWindow::validateFields);

    //recherche
    ui->rechercherLabo->setPlaceholderText("Rechercher par nom du laboratoire, responsable ou statut...");
    connect(ui->rechercherLabo, &QLineEdit::textChanged, this, &MainWindow::rechercherLabo);

    //tri
    connect(ui->comboBox_Tri, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_comboBox_Tri_currentIndexChanged_M);
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
    populateResponsableComboBox();
    // Populate lab name combo box on initialization
     populateLabNameComboBox();
    populateNomlabComboBox();
    TCPSocket = new QTcpSocket(this);
    TCPSocket->connectToHost(QHostAddress::LocalHost, 1234);


   displayLaboratoires();

   Read_Data_From_Socket();

    connect(ui->produits, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(1);
        ui->frame->setVisible(true);
    });

    connect(ui->labo, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(2);
        ui->frame->setVisible(true);
  });

    connect(ui->vaccin, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(3);
        ui->frame->setVisible(true);
    });

    connect(ui->carnet, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(5);
    });

    connect(ui->compagne, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(4);
        ui->frame->setVisible(true);
        displayCompagne();                      // Affiche la liste des compagnes
    });

    connect(ui->pushButton_modifier_employe, &QPushButton::clicked, this, &MainWindow::on_button_modifier_clicked);
    connect(ui->btn_login, &QPushButton::clicked, this, &MainWindow::seConnecter);
    connect(ui->comboBox_tri_3, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::appliquerTriEmployes);

    connect(ui->btn_rechercher, &QPushButton::clicked, this, &MainWindow::lancerRechercheEmploye);
    connect(ui->btn_reset_table, &QPushButton::clicked, this, &MainWindow::reinitialiserTableEmployes);
    connect(ui->btn_planning_pdf, &QPushButton::clicked, this, &MainWindow::exporterPlanningHoraireHTML);


    QTimer::singleShot(0, this, SLOT(chargerNomsDansComboBoxPDF()));  // pour bien le charger après l'UI
    connect(ui->btn_stats_employes, &QPushButton::clicked, this, &MainWindow::afficherStatistiquesEmployes);


    manager = new QNetworkAccessManager(this);

    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::receptionReponseGPT);

    connect(ui->button_fermer_chatbot, &QPushButton::clicked, this, &MainWindow::on_button_fermer_chatbot_clicked);
    connect(ui->button_envoyer, &QPushButton::clicked, this, &MainWindow::on_button_envoyer_clicked);
    ui->textEdit_chat->setStyleSheet("color: black; background-color: white; border-radius: 10px;");


    ui->textEdit_chat->setWordWrapMode(QTextOption::WordWrap);



    // (optionnel : connecter ici si non connecté par le designer)
    faqMap.insert("comment ajouter un employe", "Pour ajouter un employé, allez dans la section 'Employés', cliquez sur 'Ajouter', remplissez le formulaire puis validez.");
    faqMap.insert("ajouter employe", "Pour ajouter un employé, allez dans la section 'Employés', cliquez sur 'Ajouter', remplissez le formulaire puis validez.");
    faqMap.insert("supprimer un employe", "Pour supprimer un employé, sélectionnez-le dans la liste puis cliquez sur 'Supprimer'.");
    faqMap.insert("modifier employe", "Cliquez sur l'employé à modifier, changez les champs désirés et appuyez sur 'Enregistrer'.");
    faqMap.insert("liste employes", "Vous pouvez consulter la liste complète des employés depuis l’onglet 'Gestion des employés'.");
    faqMap.insert("statistiques employes", "Les statistiques RH sont disponibles dans l’onglet 'Statistiques' du module Employés.");
    faqMap.insert("employe le plus ancien", "L’employé le plus ancien est affiché dans la section des statistiques RH.");
    faqMap.insert("absences employes", "Les absences sont gérées automatiquement depuis le module RH.");


    faq = QJsonArray{
        QJsonObject{{"keywords", QJsonArray{"ajouter employé", "ajouter un employé", "nouvel employé", "ajout employé"}},
                    {"response", "Pour ajouter un employé, allez dans l'onglet RH et remplissez le formulaire avec le nom, prénom, poste, etc., puis cliquez sur Ajouter."}},
        QJsonObject{{"keywords", QJsonArray{"modifier employé", "changer infos employé", "mettre à jour employé"}},
                    {"response", "Pour modifier un employé, recherchez-le par CIN puis cliquez sur Modifier. Modifiez les champs et validez."}},
        QJsonObject{{"keywords", QJsonArray{"supprimer employé", "effacer employé", "retirer employé"}},
                    {"response", "Pour supprimer un employé, entrez son CIN dans le champ dédié puis cliquez sur Supprimer."}},
        QJsonObject{{"keywords", QJsonArray{"statistiques employé", "camembert employés", "taux d'absence"}},
                    {"response", "Cliquez sur 'Statistiques' pour voir les données de disponibilité, absences et le top 3 des anciens employés."}},
        QJsonObject{{"keywords", QJsonArray{"export planning", "pdf employés", "horaire employés"}},
                    {"response", "Utilisez le bouton 'Exporter Planning Hebdo' pour générer un planning en HTML ou PDF sur une période donnée."}},
        QJsonObject{{"keywords", QJsonArray{"rechercher employé", "chercher un employé", "trouver employé"}},
                    {"response", "Utilisez la section de recherche par CIN, contact ou disponibilité pour filtrer les employés."}},
        QJsonObject{{"keywords", QJsonArray{"connexion", "se connecter", "login employé"}},
                    {"response", "Les employés se connectent avec leur prénom + 4 derniers chiffres du CIN. Le poste détermine l'accès aux modules."}}
    };
   connect(ui->btn_historique_connexions, &QPushButton::clicked, this, &MainWindow::afficherHistoriqueConnexions);

    chatbotButton = new QPushButton(this);
    chatbotButton->setText("💬 Chatbot");

    chatbotButton->setText("💬");
    chatbotButton->setToolTip("💬 Ouvrir le Chat RH (Ctrl+M)");
    chatbotButton->setCursor(Qt::PointingHandCursor);
    chatbotButton->setFixedSize(50, 50);
    chatbotButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #3498db;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 25px;"  // bouton rond
        "  font-size: 20px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #2980b9;"
        "}"
        );
    qApp->setStyleSheet(
        "QToolTip {"
        "  background-color: #3498db;"
        "  color: white;"
        "  font-size: 13px;"
        "  border: none;"
        "  padding: 8px;"
        "  border-radius: 6px;"
        "}"
        );


    chatbotButton->setToolTip("Ouvrir le Chat RH (Ctrl+M)");
    chatbotButton->hide();
    chatbotButton->move(this->width() - chatbotButton->width() - 20, this->height() - chatbotButton->height() - 120);



    chatbotButton->raise(); // Par-dessus tout
    connect(chatbotButton, &QPushButton::clicked, this, &MainWindow::on_button_chatbot_icon_clicked);

    ui->tab_2->setTabText(3, "Statistiques");
    ui->Affichage_6->setTabText(3, "Affectation");
    ui->Affichage_6->setTabText(2, "Calendrier");
    ui->Affichage_6->setTabText(4, "Statistiques");









    //daoussar
    displayCompagne();

    // 📁 Aller à la page des compagnes

    connect(ui->compagne, &QPushButton::clicked, this, &MainWindow::navigateToCampaigns);


    // 🗑️ Bouton suppression
    connect(ui->button_supprimer, &QPushButton::clicked, this, &MainWindow::on_Button_supprimer_clicked);




    // === 🔢 VALIDATION DES CHAMPS NUMÉRIQUES ===

    // 📌 Objectif doses : obligatoirement positif
    ui->objectif_doses->setValidator(new QIntValidator(1, 999999, this));

    // 📌 Doses administrées : ≥ 0 uniquement
    ui->doses_administrees->setValidator(new QIntValidator(0, 999999, this));



    // === 💡 MÉTIER 1 - Recherche par date de campagne ===

    connect(ui->search_date, &QDateEdit::dateChanged, this, &MainWindow::rechercherParDate);


    // 📤 Générer un PDF d’approvisionnement
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

    // === 👨‍⚕️ MÉTIER 2 - Affectation d’employés ===


    // 🛠️ Charger les données si la BDD est bien ouverte
    if (QSqlDatabase::database().isOpen()) {

        afficherAffectations();  // Tableau dynamique style Trello
    }

    // 📊 Statistiques (camembert)
    connect(ui->btn_stat_vaccination_zone, &QPushButton::clicked, this, &MainWindow::afficherStatistiquesVaccinationParZone);

    // ✅ Valider les affectations glissées
    connect(ui->btn_valider_affectations, &QPushButton::clicked, this, &MainWindow::validerAffectations);

    // === 🔍 RECHERCHE & TRI ===

    // 🌐 Placeholder dans le champ de recherche
    ui->lineEdit_recherche->setPlaceholderText("🔎 Rechercher par nom, vaccin ou zone...");

    // 🔄 Recherche dynamique
    connect(ui->lineEdit_recherche, &QLineEdit::textChanged, this, &MainWindow::rechercherCompagnes);

    // 🔃 Tri dynamique
    connect(ui->comboBox_tri, &QComboBox::currentTextChanged, this, &MainWindow::trierCompagnes);

    // ❌ Annulation (déconnectée pour l’instant)
    // connect(ui->pushButton_annuler, &QPushButton::clicked, this, &MainWindow::on_pushButton_annuler_clicked);


   /* //arduino CLAVIER

    // 🎯 Connexion clavier Arduino

   QObject::connect(A.getserial(), &QSerialPort::readyRead, this, [=]() {
        static QString buffer;

        QByteArray data = A.read_from_arduino();
        QString received = QString(data).trimmed();

        if (!received.isEmpty()) {
            buffer += received;
            qDebug() << "🔁 Accumulation clavier : " << buffer;
        }

        if (buffer.endsWith("#")) {
            QString code = buffer.left(buffer.length() - 1);
            qDebug() << "✅ Code complet reçu : " << code;
            buffer.clear();

            QSqlQuery query;
            query.prepare(R"(
                SELECT E.NOM, E.POSTE, CP.NOM_COMPAGNE
                FROM EMPLOYES E
                JOIN CONTRIBUER C ON E.ID_EMPLOYE = C.ID_EMPLOYE
                JOIN COMPAGNE CP ON CP.ID_COMPAGNE = C.ID_COMPAGNE
                WHERE E.RFID_ID = :code
            )");
            query.bindValue(":code", code);

            if (query.exec() && query.next()) {
                QString nom = query.value("NOM").toString();
                QString poste = query.value("POSTE").toString();
                QString nomCompagne = query.value("NOM_COMPAGNE").toString();

                QString nomAffiche = nom.toUpper();
                QString message = QString(
                    "<div style='font-size:15px;'>"
                    "<p><b style='font-size:18px;'>🎉 BIENVENUE %1</b></p>"
                    "<p><b>Poste :</b> %2</p>"
                    "<p><b>Compagne :</b> %3</p>"
                    "<hr>"
                    "<p style='color:green;'>✅ Bonne mission !<br>✊ Merci d’être là pour sauver des vies.</p>"
                    "</div>"
                )
                .arg(nomAffiche)
                .arg(poste)
                .arg(nomCompagne);

                QMessageBox msgBox;
                msgBox.setWindowTitle("✔️ Let's save lives");
                msgBox.setTextFormat(Qt::RichText);
                msgBox.setText(message);
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setButtonText(QMessageBox::Ok, "Let's save lives");
                msgBox.setStyleSheet(
                    "QLabel { min-width: 300px; font-family: Arial; font-size: 14px; } "
                    "QPushButton { background-color: darkgreen; color: white; font-weight: bold; padding: 8px 18px; border-radius: 8px; } "
                    "QPushButton:hover { background-color: green; }"
                );
                msgBox.exec();
            } else {
                QMessageBox::warning(this, "❌ Accès refusé",
                    "Code invalide ou employé non affecté à une compagne !");
            }
        }
    });


*/

    //ele

    ui->labelErreurNom_6->clear();
    ui->labelErreurNom_6->setVisible(false);
    ui->labelErreurFournisseur_6->clear();
    ui->labelErreurFournisseur_6->setVisible(false);
    ui->labelErreurDateExpiration_6->clear();
    ui->labelErreurDateExpiration_6->setVisible(false);

    QRegularExpression regexNom("^[A-Za-zÀ-ÿ ]+$");
    ui->nomProduit_6->setValidator(new QRegularExpressionValidator(regexNom, this));
    ui->nomFournisseur_6->setValidator(new QRegularExpressionValidator(regexNom, this));
    ui->dateFabrication_6->setDate(QDate::currentDate());
    ui->dateExpiration_6->setDate(QDate::currentDate());
    ui->champRecherche_11->setPlaceholderText("🔍 Rechercher par nom, catégorie ou fournisseur");



    connect(ui->nomProduit_6, &QLineEdit::textChanged, this, &MainWindow::verifierNomProduit);
    connect(ui->nomFournisseur_6, &QLineEdit::textChanged, this, &MainWindow::validerChampsP);
    connect(ui->dateExpiration_6, &QDateEdit::dateChanged, this, &MainWindow::validerChampsP);
    connect(ui->quantite_6, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::validerChampsP);
    connect(ui->rechercher_6, &QPushButton::clicked, this, &MainWindow::on_rechercherP_clicked);
    connect(ui->Stat_6, &QPushButton::clicked, this, &MainWindow::on_StatP_clicked);
    QString message=produitTmp.MessageDeExpiration();
    qDebug()<<message;
    if(message!="")
    {
        email.sendEmail("najoua.dahmen18@gmail.com", "Notification d'Expiration",message );
    }

  //eya
    ui->lineEdit_recherche_2->setToolTip("Tapez un mot-clé présent dans : nom, type ou certification du vaccin.");
    ui->lineEdit_recherche_2->setPlaceholderText("Rechercher par nom, type ou certification...");
    ui->stackedWidget->setCurrentIndex(6);
    ui->frame->setVisible(false);
    ui->labelErrorNom->clear(); // Supprimer le texte
    ui->labelErrorNom->setStyleSheet("color: transparent; background-color: transparent; border: none; font-weight: normal;");
    ui->labelErrorType->setStyleSheet("color: transparent; background-color: transparent; border: none; font-weight: normal;");
    ui->labelErrorCertification->setStyleSheet("color: transparent; background-color: transparent; border: none; font-weight: normal;");

    qDebug() << "🚀 Initialisation de MainWindow. nomAModifier =" << nomAModifier;


    // Connexion des champs à la vérification en temps réel
    connect(ui->lineEdit_nom_2, &QLineEdit::textChanged, this, &MainWindow::verifierNom);
    // connect(ui->lineEdit_typev_2, &QLineEdit::textChanged, this, &MainWindow::verifierTypeVaccin);
    //connect(ui->lineEdit_certification_2, &QLineEdit::textChanged, this, &MainWindow::verifierCertificationVaccin);

    // 🔹 Initialiser medApi ici pour éviter un accès à un nullptr
    medApi = new MedAnalysis(this);
    connect(medApi, &MedAnalysis::dataFetched, this, &MainWindow::updateCovidStats);
    connect(medApi, &MedAnalysis::errorOccurred, this, &MainWindow::showError);

    // 🔹 Initialisation de la liste des pays
    ui->comboBoxPays->addItems({
        "France", "Tunisie", "Allemagne", "Italie", "Espagne", "États-Unis", "Canada", "Maroc", "Algérie", "Chine"
    });
    // ✅ Appliquer un style moderne avec des bordures arrondies et un fond agréable
    this->setStyleSheet(R"(
    QWidget {
        background-color: #f4f4f4;
        font-family: 'Arial';
    }



    QPushButton {
        background-color: #B22222;
        color: white;
        border-radius: 10px;
        padding: 8px;
        font-size: 14px;
        font-weight: bold;
    }

    QPushButton:hover {
        background-color: #8B0000;
    }

    QComboBox {
        background-color: white;
        border: 1px solid #aaa;
        border-radius: 5px;
        padding: 5px;
    }

    QTableView
 {
        background-color: white;
        border-radius: 5px;
        border: 1px solid #ddd;
    }

    QHeaderView::section {
        background-color: #B22222;
        color: white;
        padding: 5px;
        font-weight: bold;
    }

    QMessageBox {
        font-size: 14px;
    }
)");

    connect(ui->btnPredire_2, &QPushButton::clicked, this, &MainWindow::lancerPrediction);
    // Fonction pour gérer l'affichage du sidebar selon la page active
    /*auto updateSidebarVisibility = [=](int index) {
        if (index == 6) { // Page de connexion
            ui->frame->setVisible(false);
        } else {
            ui->frame->setVisible(true);
        }
    };*/
    connect(ui->vaccin, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(3);
        ui->frame->setVisible(true);
        verifierVaccinsExpires();

        // Mettre à jour immédiatement l'affichage des vaccins
        Vaccin v;
        ui->tableView_2->setModel(v.afficher());

        // Ajustements d'affichage
        ui->tableView_2->verticalHeader()->setDefaultSectionSize(35);
        ui->tableView_2->horizontalHeader()->setDefaultSectionSize(150);
        ui->tableView_2->resizeColumnsToContents();
        ui->tableView_2->setStyleSheet("QTableView::item { padding: 10px; }");

        // Faire en sorte que les colonnes prennent toute la largeur
        ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    });

    connect(ui->pushButton_ajouter_2, &QPushButton::clicked, this, &MainWindow::on_pushButton_ajouter_v_clicked);
    //arduino
    int ret = A.connect_arduino();
        switch (ret) {
        case 0:
            qDebug() << "Arduino connecté sur :" << A.getarduino_port_name();
            break;
        case 1:
            qDebug() << "Port trouvé mais connexion impossible : " << A.getarduino_port_name();
            break;
        case -1:
            qDebug() << "Arduino non détecté";
            break;
        }

        connect(A.getserial(), &QSerialPort::readyRead, this, &MainWindow::lireDonneesSerie);
}







MainWindow::~MainWindow()
{
    if (TCPSocket && TCPSocket->isOpen()) {
        TCPSocket->close();
    }
    delete ui;
}

void MainWindow::on_button_chatbot_icon_clicked()
{
    if (ui->frame_chatbox->isHidden()) {
        ui->frame_chatbox->show();

        // 🎯 Animation rebond vers le haut
        QPropertyAnimation *anim = new QPropertyAnimation(ui->frame_chatbox, "geometry");
        QRect startRect = ui->frame_chatbox->geometry();
        anim->setDuration(500);
        anim->setStartValue(QRect(startRect.x(), startRect.y() + 100, startRect.width(), startRect.height()));
        anim->setEndValue(startRect);
        anim->setEasingCurve(QEasingCurve::OutBounce);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    } else {
        ui->frame_chatbox->hide();
    }
}

void MainWindow::afficherChatbot()
{
    if (ui->frame_chatbox->isVisible())
        ui->frame_chatbox->hide();
    else
        ui->frame_chatbox->show();
}
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    int margin = 20;
    int buttonWidth = chatbotButton->width();
    int buttonHeight = chatbotButton->height();
    chatbotButton->move(width() - buttonWidth - margin, height() - buttonHeight - margin);
}
void MainWindow::on_Affichage_currentChanged(int index)
{
    if (index ==0){
        Employe employe;
        ui->tableView->setModel(employe.afficher());

    }
}
void MainWindow::afficherDashboard(const QString& prenom, const QString& poste)
{
    ui->label_bienvenue->setText("👋 Bienvenue, " + prenom + " !");
    ui->label_poste->setText("Poste : " + poste);
    ui->stackedWidget->setCurrentIndex(7); // vers le dashboard
}

void MainWindow::on_ajouter_employe_clicked()
{
    // ✅ 1. Récupérer le prochain ID_EMPLOYE automatiquement
    QSqlQuery idQuery;
    idQuery.prepare("SELECT COALESCE(MAX(ID_EMPLOYE), 0) + 1 FROM SMARTVACC.EMPLOYES");

    int id_employe = 1; // Valeur par défaut si la table est vide
    if (idQuery.exec() && idQuery.next()) {
        id_employe = idQuery.value(0).toInt(); // Récupère l'ID suivant
    }

    // ✅ 2. Récupérer les valeurs des champs
    QString cin = ui->lineedit_cin_employe->text().trimmed();
    QString nom = ui->lineedit_nom_employe->text().trimmed();
    QString prenom = ui->lineedit_prenom_employe->text().trimmed();
    QString poste = ui->combobox_poste->currentText();
    float salaire = ui->lineditsalaire->text().toFloat();
    QString contact = ui->lineeditcontact->text().trimmed();
    QDate date_embauche = ui->dateEdit->date();
    int disponibilite = (ui->combobox_disponibilite->currentText().toLower() == "oui") ? 1 : 0;
    QString type_absences = ui->combobox_abscences->currentText();

    // ✅ 3. Vérification du sexe
    QString sexe;
    if (ui->radiobutton_homme->isChecked()) {
        sexe = "Homme";
    } else if (ui->radiobutton_femme->isChecked()) {
        sexe = "Femme";
    } else {
        QMessageBox::warning(this, "Champ manquant", "Veuillez sélectionner un sexe !");
        return;
    }

    // ✅ 4. Vérifier si les champs obligatoires sont remplis
    if (cin.isEmpty() || nom.isEmpty() || prenom.isEmpty() || poste.isEmpty() || salaire <= 0 || contact.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires !");
        return;
    }

    // ✅ 5. Mode Modification 🛠
    if (modeModification) {
        Employe employe (cin, id_employe, nom, prenom, poste, sexe, salaire, contact, date_embauche, disponibilite, type_absences);

        if (employe.modifier(cin)) {
            QMessageBox::information(this, "Succès", "L'employé a été modifié avec succès !");
            modeModification = false;  // Désactiver le mode modification
            cinOriginal.clear();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la modification de l'employé !");
            return;
        }
    }
    // ✅ 6. Mode Ajout 🆕
    else {
        Employe employe(cin, id_employe, nom, prenom, poste, sexe, salaire, contact, date_embauche, disponibilite, type_absences);

        if (employe.ajouter()) {
            QMessageBox::information(this, "Succès", "L'employé a été ajouté avec succès !");
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de l'ajout de l'employé !");
            return;
        }
    }

    // ✅ 7. Mise à jour de la `tableView`
    ui->tableView->setModel(employe.afficher());

    // ✅ 8. Réinitialiser les champs après l'ajout ou modification
    ui->lineedit_cin_employe->clear();
    ui->lineedit_nom_employe->clear();
    ui->lineedit_prenom_employe->clear();
    ui->combobox_poste->setCurrentIndex(0); // Remettre à l'option par défaut
    ui->lineditsalaire->clear();
    ui->lineeditcontact->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->combobox_disponibilite->setCurrentIndex(0);
    ui->combobox_abscences->setCurrentIndex(0);
    ui->radiobutton_homme->setChecked(false);
    ui->radiobutton_femme->setChecked(false);


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
        ui->combobox_poste->clear();
        ui->radiobutton_homme->setChecked(false);
        ui->radiobutton_femme->setChecked(false);
        ui->lineditsalaire->clear();
        ui->lineeditcontact->clear();
        ui->dateEdit->setDate(QDate::currentDate());
        ui->combobox_disponibilite->clear();
        ui->combobox_abscences->setCurrentIndex(0);
    }
}




















void MainWindow::on_button_modifier_clicked()
{
    QString cin = ui->lineedit_cin_employe_test->text().trimmed();

    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un CIN !");
        return;
    }

    // 🔹 Vérifier si l'employé existe avant de modifier
    Employe employe;
    if (!employe.chargerEmploye(cin)) {
        QMessageBox::critical(this, "Erreur", "Aucun employé trouvé avec ce CIN !");
        return;
    }

    modeModifications = true;  // ✅ Active le mode modification
    cinOriginal = cin;  // ✅ Stocke l'ancien CIN pour la mise à jour

    // 🔹 Pré-remplissage des champs avec les données existantes
    ui->lineedit_cin_employe->setText(employe.getCIN());
    ui->lineedit_nom_employe->setText(employe.getNom());
    ui->lineedit_prenom_employe->setText(employe.getPrenom());

    // ✅ Sélection du poste dans la ComboBox
    int posteIndex = ui->combobox_poste->findText(employe.getPoste());
    if (posteIndex != -1) {
        ui->combobox_poste->setCurrentIndex(posteIndex);
    }

    // ✅ Sélection du sexe
    ui->radiobutton_homme->setChecked(employe.getSexe() == "Homme");
    ui->radiobutton_femme->setChecked(employe.getSexe() == "Femme");

    // ✅ Conversion et mise à jour des champs numériques
    ui->lineditsalaire->setText(QString::number(employe.getSalaire(), 'f', 2));  // Format avec 2 décimales
    ui->lineeditcontact->setText(employe.getContact());
    ui->dateEdit->setDate(employe.getDateEmbauche());

    // ✅ Sélection de la disponibilité dans la ComboBox
    int dispoIndex = ui->combobox_disponibilite->findText(employe.getDisponibilite() ? "Oui" : "Non");
    if (dispoIndex != -1) {
        ui->combobox_disponibilite->setCurrentIndex(dispoIndex);
    }

    // ✅ Sélection du type d'absence dans la ComboBox
    int absenceIndex = ui->combobox_abscences->findText(employe.getTypeAbsences());
    if (absenceIndex != -1) {
        ui->combobox_abscences->setCurrentIndex(absenceIndex);
    }

    // ✅ Redirige vers la page d'édition
   // ui->pdf->setCurrentWidget(ui->ajoutct);
}



void MainWindow::validateInput(QLineEdit *field, QLabel *errorLabel, QRegularExpression regex, const QString &errorMsg)
{
    QString text = field->text().trimmed();

    if (regex.match(text).hasMatch()) {
        field->setStyleSheet("border: 2px solid green; padding: 5px; background: white;");
        errorLabel->setText("✔️ Valide");
        errorLabel->setStyleSheet("color: green; font-weight: bold; background: transparent;");
    } else {
        field->setStyleSheet("border: 2px solid red; padding: 5px; background: white;");
        errorLabel->setText("❌ " + errorMsg);
        errorLabel->setStyleSheet("color: red; font-weight: bold; background: transparent;");
    }
}

// ✅ Validation des champs TextEdit
void MainWindow::validateTextEdit(QTextEdit *field, QLabel *errorLabel, QRegularExpression regex)
{
    QString text = field->toPlainText().trimmed();
    static const QRegularExpression wordCheckRegex(R"(^[A-Za-zÀ-ÿ\s,]+$)");
    QStringList words = text.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

    if (regex.match(text).hasMatch() && wordCheckRegex.match(text).hasMatch() && words.size() >= 2) {
        errorLabel->setText("✔️ Valide");
        errorLabel->setStyleSheet("color: green; font-weight: bold; background: transparent;");
    } else {
        errorLabel->setText("❌ Le texte doit contenir au moins 2 mots, sans chiffres.");
        errorLabel->setStyleSheet("color: red; font-weight: bold; background: transparent;");
    }
}

// ✅ Fonction globale de validation avant soumission
bool MainWindow::estValide()
{
    bool valide = true;
    QString styleErreur = "color: red; font-weight: bold; background: transparent;";
    QString styleValide = "color: green; font-weight: bold; background: transparent;";

    // ✅ Vérification du CIN (8 chiffres uniquement)
    QString cin = ui->lineedit_cin_employe->text().trimmed();
    if (!QRegularExpression("^[0-9]{8}$").match(cin).hasMatch()) {
        ui->cinErrorLabel->setText("❌ Le CIN doit contenir exactement 8 chiffres.");
        ui->cinErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->cinErrorLabel->setText("✔️ Valide");
        ui->cinErrorLabel->setStyleSheet(styleValide);
    }

    // ✅ Vérification du Nom (lettres uniquement)
    QString nom = ui->lineedit_nom_employe->text().trimmed();
    if (!QRegularExpression("^[A-Za-zÀ-ÿ\\s-]+$").match(nom).hasMatch() || nom.isEmpty()) {
        ui->nomErrorLabel->setText("❌ Le nom doit contenir uniquement des lettres.");
        ui->nomErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->nomErrorLabel->setText("✔️ Valide");
        ui->nomErrorLabel->setStyleSheet(styleValide);
    }

    // ✅ Vérification du Prénom (lettres uniquement)
    QString prenom = ui->lineedit_prenom_employe->text().trimmed();
    if (!QRegularExpression("^[A-Za-zÀ-ÿ\\s-]+$").match(prenom).hasMatch() || prenom.isEmpty()) {
        ui->prenomErrorLabel->setText("❌ Le prénom doit contenir uniquement des lettres.");
        ui->prenomErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->prenomErrorLabel->setText("✔️ Valide");
        ui->prenomErrorLabel->setStyleSheet(styleValide);
    }

    // ✅ Vérification du Salaire (valeur positive uniquement)
    bool ok;
    float salaire = ui->lineditsalaire->text().toFloat(&ok);
    if (!ok || salaire <= 0) {
        ui->salaireErrorLabel->setText("❌ Le salaire doit être un nombre strictement positif.");
        ui->salaireErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->salaireErrorLabel->setText("✔️ Valide");
        ui->salaireErrorLabel->setStyleSheet(styleValide);
    }

    // ✅ Vérification du Contact (exactement 8 chiffres)
    QString contact = ui->lineeditcontact->text().trimmed();
    if (!QRegularExpression("^[0-9]{8}$").match(contact).hasMatch()) {
        ui->contactErrorLabel->setText("❌ Le contact doit contenir exactement 8 chiffres.");
        ui->contactErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->contactErrorLabel->setText("✔️ Valide");
        ui->contactErrorLabel->setStyleSheet(styleValide);
    }

    // ✅ Vérification du Poste (ComboBox)
    if (ui->combobox_poste->currentIndex() == 0) {
        ui->posteErrorLabel->setText("❌ Veuillez sélectionner un poste.");
        ui->posteErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->posteErrorLabel->setText("✔️ Valide");
        ui->posteErrorLabel->setStyleSheet(styleValide);
    }

    // ✅ Vérification de la Disponibilité (ComboBox)
    if (ui->combobox_disponibilite->currentIndex() == 0) {
        ui->disponibiliteErrorLabel->setText("❌ Veuillez sélectionner une disponibilité.");
        ui->disponibiliteErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->disponibiliteErrorLabel->setText("✔️ Valide");
        ui->disponibiliteErrorLabel->setStyleSheet(styleValide);
    }

    // ✅ Vérification du Type d'Absence (ComboBox)
    if (ui->combobox_abscences->currentIndex() == 0) {
        ui->abscenceErrorLabel->setText("❌ Veuillez sélectionner un type d'absence.");
        ui->abscenceErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->abscenceErrorLabel->setText("✔️ Valide");
        ui->abscenceErrorLabel->setStyleSheet(styleValide);
    }

    // ✅ Vérification du Sexe (RadioButton)
    if (!ui->radiobutton_homme->isChecked() && !ui->radiobutton_femme->isChecked()) {
        ui->sexeErrorLabel->setText("❌ Veuillez sélectionner un sexe.");
        ui->sexeErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->sexeErrorLabel->setText("✔️ Valide");
        ui->sexeErrorLabel->setStyleSheet(styleValide);
    }

    return valide;
}

void MainWindow::on_annuler_clicked()
{
    if (modeModification) {
        // 🔹 Si on est en mode modification, remettre les anciennes valeurs
        QSqlQuery query;
        query.prepare("SELECT CIN, ID_EMPLOYE, NOM, PRENOM, POSTE, SEXE, SALAIRE, CONTACT, DATE_EMBAUCHE, DISPONIBILITE, TYPE_ABSENCES FROM SMARTVACC.EMPLOYES WHERE CIN = :cin");
        query.bindValue(":cin", ui->lineedit_cin_employe->text().trimmed());

        if (query.exec() && query.next()) {
            // ✅ Récupérer et afficher les anciennes valeurs
            ui->lineedit_cin_employe->setText(query.value("CIN").toString());
            ui->lineedit_nom_employe->setText(query.value("NOM").toString());
            ui->lineedit_prenom_employe->setText(query.value("PRENOM").toString());

            // ✅ Sélection de la valeur correcte dans la ComboBox "Poste"
            int posteIndex = ui->combobox_poste->findText(query.value("POSTE").toString());
            if (posteIndex != -1) ui->combobox_poste->setCurrentIndex(posteIndex);

            // ✅ Sélection du sexe
            ui->radiobutton_homme->setChecked(query.value("SEXE").toString() == "Homme");
            ui->radiobutton_femme->setChecked(query.value("SEXE").toString() == "Femme");

            // ✅ Formatage des valeurs numériques
            ui->lineditsalaire->setText(QString::number(query.value("SALAIRE").toFloat(), 'f', 2));
            ui->lineeditcontact->setText(query.value("CONTACT").toString());
            ui->dateEdit->setDate(query.value("DATE_EMBAUCHE").toDate());

            // ✅ Sélection de la valeur correcte dans la ComboBox "Disponibilité"
            int dispoIndex = ui->combobox_disponibilite->findText(query.value("DISPONIBILITE").toInt() == 1 ? "Oui" : "Non");
            if (dispoIndex != -1) ui->combobox_disponibilite->setCurrentIndex(dispoIndex);

            // ✅ Sélection de la valeur correcte dans la ComboBox "Type d'absence"
            int absenceIndex = ui->combobox_abscences->findText(query.value("TYPE_ABSENCES").toString());
            if (absenceIndex != -1) ui->combobox_abscences->setCurrentIndex(absenceIndex);
        } else {
            QMessageBox::warning(this, "Erreur", "Impossible de récupérer les données de l'employé.");
        }
    } else {
        // 🔹 Si on est en mode ajout, vider les champs
        ui->lineedit_cin_employe->clear();
        ui->lineedit_nom_employe->clear();
        ui->lineedit_prenom_employe->clear();
        ui->lineditsalaire->clear();
        ui->lineeditcontact->clear();
        ui->dateEdit->setDate(QDate::currentDate());

        // ✅ Réinitialiser les ComboBox à leur valeur par défaut
        ui->combobox_poste->setCurrentIndex(0);
        ui->combobox_disponibilite->setCurrentIndex(0);
        ui->combobox_abscences->setCurrentIndex(0);

        // ✅ Décocher les boutons radio
        ui->radiobutton_homme->setChecked(false);
        ui->radiobutton_femme->setChecked(false);
    }

}

void MainWindow::seConnecter()
{
    static int tentativeConnexion = 0;
    static bool connexionBloquee = false;
        // dans le constructeur


    if (connexionBloquee) {
        QMessageBox::critical(this, "Connexion bloquée", "Vous avez atteint le nombre maximal de tentatives.\nVeuillez patienter 30 secondes.");
        return;
    }

    QString login = ui->lineedit_login->text().trimmed();
    QString cin = ui->lineedit_password->text().trimmed();

    if (login.isEmpty() || cin.isEmpty()) {
        QMessageBox::warning(this, "Champs manquants", "Veuillez remplir tous les champs !");
        return;
    }

    Employe e;
    if (e.authentifier(login, cin)) {
        ui->label_tentatives->clear();  // Réinitialise l'affichage des tentatives
        tentativeConnexion = 0;
        chatbotButton->show();

        QString poste = e.getPosteFromCIN(cin).toLower();
        //chatbotButton->show();  // dans le constructeur


        // ✅ Animation rebond
        QPropertyAnimation *animation = new QPropertyAnimation(ui->frame_7, "geometry");
        QRect startRect = ui->frame_7->geometry();
        animation->setDuration(500);
        animation->setStartValue(startRect.adjusted(0, -50, 0, -50));
        animation->setEndValue(startRect);
        animation->setEasingCurve(QEasingCurve::OutBounce);
        animation->start(QAbstractAnimation::DeleteWhenStopped);

        enregistrerConnexion(login); // ✅ Historique

        // ✅ Redirection après animation
        QTimer::singleShot(750, this, [=]() {
            afficherDashboard(login, poste); // ✅ Ajouter ici

            if (poste == "hr")            redirectTo("employe", 0);
            else if (poste == "produits") redirectTo("produits", 1);
            else if (poste == "labo")     redirectTo("labo", 2);
            else if (poste == "vaccin")   redirectTo("vaccin", 3);
            else if (poste == "compagne") redirectTo("compagne", 4);
            else if (poste == "carnet")   redirectTo("carnet", 5);
            else if (poste == "admin")    redirectTo("admin", 0, true);
            else {
                QMessageBox::critical(this, "Accès refusé", "Ce poste n'a pas accès à l'application.");
                ui->stackedWidget->setCurrentIndex(6); // retour login
            }
        });
    }
    else {
        tentativeConnexion++;
        ui->label_tentatives->setText("❌ Tentative " + QString::number(tentativeConnexion) + "/3");

        // ❌ Animation shake
        QPropertyAnimation *shake = new QPropertyAnimation(ui->frame_7, "pos");
        QPoint originalPos = ui->frame_7->pos();
        shake->setDuration(500);
        shake->setKeyValueAt(0, originalPos);
        shake->setKeyValueAt(0.25, originalPos + QPoint(-10, 0));
        shake->setKeyValueAt(0.5, originalPos + QPoint(10, 0));
        shake->setKeyValueAt(0.75, originalPos + QPoint(-10, 0));
        shake->setKeyValueAt(1, originalPos);
        shake->start(QAbstractAnimation::DeleteWhenStopped);

        if (tentativeConnexion >= 3) {
            connexionBloquee = true;
            QMessageBox::critical(this, "Trop de tentatives", "Vous avez fait 3 tentatives incorrectes.\nConnexion bloquée pendant 30 secondes.");

            QTimer::singleShot(30000, this, [=]() mutable {
                tentativeConnexion = 0;
                connexionBloquee = false;
                ui->label_tentatives->clear();
                QMessageBox::information(this, "Connexion réactivée", "Vous pouvez réessayer de vous connecter.");
            });
        } else {
            QMessageBox::critical(this, "Erreur", QString("Identifiant ou mot de passe incorrect.\nTentative %1/3").arg(tentativeConnexion));
        }
    }
}




void MainWindow::redirectTo(const QString &poste, int pageIndex, bool fullAccess)
{
    ui->stackedWidget->setCurrentIndex(pageIndex);
    ui->frame->setVisible(true); // afficher la sidebar

    if (fullAccess) {
        disableOtherModulesExcept({"employe", "produits", "labo", "vaccin", "compagne", "carnet"});
    } else {
        disableOtherModulesExcept({poste});
    }
}

void MainWindow::disableOtherModulesExcept(const QStringList &allowed)
{
    ui->employe->setEnabled(allowed.contains("employe"));
    ui->produits->setEnabled(allowed.contains("produits"));
    ui->labo->setEnabled(allowed.contains("labo"));
    ui->vaccin->setEnabled(allowed.contains("vaccin"));
    ui->compagne->setEnabled(allowed.contains("compagne"));
    ui->carnet->setEnabled(allowed.contains("carnet"));
}



void MainWindow::appliquerTriEmployes()
{
    Employe e;
    int index = ui->comboBox_tri_3->currentIndex();

    switch(index) {
    case 0: // Trier par poste
        ui->tableView->setModel(e.afficherParPoste());
        break;
    case 1: // Trier par ancienneté
        ui->tableView->setModel(e.afficherParAnciennete());
        break;
    case 2: // Trier par salaire
        ui->tableView->setModel(e.afficherParSalaire());
        break;
    default:
        break;
    }
}
void MainWindow::lancerRechercheEmploye()
{
    QString critere = ui->comboBox_recherche->currentText().toLower().trimmed();
    QString valeur = ui->lineEdit_recherche_3->text().trimmed();

    if (valeur.isEmpty()) {
        QMessageBox::warning(this, "Champ vide", "Veuillez entrer une valeur de recherche.");
        return;
    }

    Employe e;

    if (critere.contains("cin")) {
        ui->tableView->setModel(e.rechercherParCIN(valeur));
    }
    else if (critere.contains("contact")) {
        ui->tableView->setModel(e.rechercherParContact(valeur));
    }
    else if (critere.contains("dispo")) {
        if (valeur == "oui" || valeur == "1") {
            ui->tableView->setModel(e.rechercherParDisponibilite(1));
        } else if (valeur == "non" || valeur == "0") {
            ui->tableView->setModel(e.rechercherParDisponibilite(0));
        } else {
            QMessageBox::warning(this, "Erreur", "Veuillez taper 'oui' ou 'non' (ou 1 ou 0) pour la disponibilité.");
        }
    }
    else {
        QMessageBox::warning(this, "Critère inconnu", "Ce critère de recherche n'est pas reconnu.");
    }
}



void MainWindow::reinitialiserTableEmployes()
{
    Employe e;
    ui->tableView->setModel(e.afficher());
    ui->comboBox_tri_3->setCurrentIndex(-1); // désélectionne le tri
    ui->comboBox_recherche->setCurrentIndex(-1); // désélectionne recherche
    ui->lineEdit_recherche->clear(); // vide le champ
}
void MainWindow::chargerNomsDansComboBoxPDF()
{
    QSqlQuery query("SELECT CIN, NOM, PRENOM FROM SMARTVACC.EMPLOYES ORDER BY NOM ASC");

    ui->comboBox_employe_pdf->clear();
    ui->comboBox_employe_pdf->addItem("Tous");

    while (query.next()) {
        QString cin = query.value("CIN").toString();
        QString nom = query.value("NOM").toString();
        QString prenom = query.value("PRENOM").toString();
        ui->comboBox_employe_pdf->addItem(cin + " " + nom + " " + prenom);
    }
}



void MainWindow::exporterPlanningHoraireHTML()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter Planning Hebdo", "", "Fichier HTML (*.html)");
    if (fileName.isEmpty()) return;

    QDate dateDebut = ui->dateEdit_debut_pdf->date();
    QDate dateFin = ui->dateEdit_fin_pdf->date();
    QString nomSelectionne = ui->comboBox_employe_pdf->currentText();

    if (dateDebut > dateFin) {
        QMessageBox::warning(this, "Erreur de date", "La date de début doit être antérieure à la date de fin !");
        return;
    }

    QMap<QString, QString> horairesParPoste = {
        {"medecin", "10h-14h"}, {"infirmier", "8h-12h"}, {"hr", "14h-17h"},
        {"produits", "13h-16h"}, {"carnet", "9h-11h"}, {"compagne", "11h-15h"},
        {"vaccin", "9h-12h"}, {"admin", "8h-10h"}
    };

    QString html = R"(
<!DOCTYPE html>
<html>
<head>
<meta charset='utf-8'>
<title>Planning Hebdomadaire</title>
<style>
    body { font-family: 'Segoe UI', sans-serif; padding: 20px; }
    .header { display: flex; align-items: center; justify-content: space-between; margin-bottom: 10px; }
    .header img { width: 80px; height: 80px; }
    .title-block { flex-grow: 1; text-align: center; }
    .title-block h1 { font-size: 26px; margin-bottom: 5px; }
    .title-block p { font-size: 16px; margin-bottom: 0; }

    .commentaire { text-align: center; margin: 5px 0 15px; font-size: 16px; }

    table { width: 100%; border-collapse: collapse; margin-top: 10px; }
    th { background-color: #2980b9; color: white; padding: 10px; font-size: 14px; }
    td { padding: 8px; text-align: center; border: 1px solid #ccc; font-size: 13px; }
    tr:nth-child(even) { background-color: #f4f7fa; }

    .green { color: green; font-weight: bold; }
    .red { color: red; font-weight: bold; }

    .summary { margin-top: 10px; font-size: 15px; padding: 10px; background: #ecf0f1; border-radius: 8px; }
    .signature { margin-top: 40px; text-align: right; font-style: italic; font-size: 15px; padding-right: 40px; }
    .signature-line { border-top: 1px solid #000; width: 300px; margin-top: 20px; float: right; }

    .actions { text-align: center; margin-top: 40px; }
    button { padding: 10px 20px; font-size: 15px; border: none; background-color: #3498db; color: white; border-radius: 5px; cursor: pointer; }
    button:hover { background-color: #2980b9; }
</style>

<script src='https://cdnjs.cloudflare.com/ajax/libs/html2pdf.js/0.10.1/html2pdf.bundle.min.js'></script>
<script>
    function generatePDF() {
        const element = document.body;
        html2pdf().set({
            margin: 0.2,
            filename: 'planning_employes.pdf',
            image: { type: 'jpeg', quality: 0.98 },
            html2canvas: { scale: 2 },
            jsPDF: { unit: 'in', format: 'a4', orientation: 'landscape' }
        }).from(element).save();
    }
</script>
</head>
<body>
<div class='header'>
            <img src=':/images/logo.png'>
    <div class='title-block'>
        <h1>📅 Planning Hebdomadaire des Employés</h1>
        <p><strong>Période :</strong> )" + dateDebut.toString("dd/MM/yyyy") + " ➡ " + dateFin.toString("dd/MM/yyyy") + R"(</p>
    </div>
</div>

<div class='commentaire'><strong>Commentaire général RH :</strong> <span contenteditable='true'>-</span></div>
)";

    html += "<table><tr><th>CIN</th><th>Nom</th><th>Poste</th><th>Disponibilité</th><th>Absence</th>";

    QStringList jours;
    for (QDate d = dateDebut; d <= dateFin; d = d.addDays(1)) {
        jours << d.toString("dd/MM");
        html += "<th>" + d.toString("ddd<br>dd/MM") + "</th>";
    }
    html += "<th>Remarques RH</th></tr>";

    QSqlQuery query;
    if (nomSelectionne == "Tous") {
        query.prepare("SELECT CIN, NOM, PRENOM, POSTE, DISPONIBILITE, TYPE_ABSENCES, HORAIRE FROM SMARTVACC.EMPLOYES");
    } else {
        QStringList parts = nomSelectionne.split(" ");
        if (parts.size() >= 2) {
            QString nom = parts[0];
            QString prenom = parts.mid(1).join(" ");
            query.prepare("SELECT CIN, NOM, PRENOM, POSTE, DISPONIBILITE, TYPE_ABSENCES, HORAIRE FROM SMARTVACC.EMPLOYES WHERE NOM = :nom AND PRENOM = :prenom");
            query.bindValue(":nom", nom);
            query.bindValue(":prenom", prenom);
        }
    }

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL", query.lastError().text());
        return;
    }

    int total = 0, dispo = 0;
    while (query.next()) {
        total++;
        QString cin = query.value("CIN").toString();
        QString nom = query.value("NOM").toString();
        QString prenom = query.value("PRENOM").toString();
        QString poste = query.value("POSTE").toString().toLower();
        QString disponibilite = query.value("DISPONIBILITE").toInt() == 1 ? "Oui" : "Non";
        QString absence = query.value("TYPE_ABSENCES").toString();
        QString horaire = query.value("HORAIRE").toString().trimmed();
        if (horaire.isEmpty()) horaire = horairesParPoste.value(poste, "modifiable");
        if (disponibilite == "Oui") dispo++;

        html += "<tr>";
        html += "<td>" + cin + "</td>";
        html += "<td>" + nom + " " + prenom + "</td>";
        html += "<td>" + poste + "</td>";
        html += QString("<td class='%1'>%2</td>").arg(disponibilite == "Oui" ? "green" : "red", disponibilite);
        html += "<td>" + absence + "</td>";
        for (int i = 0; i < jours.size(); ++i)
            html += "<td contenteditable='true'>" + horaire + "</td>";
        html += "<td contenteditable='true'>-</td>";
        html += "</tr>";
    }

    double taux = total > 0 ? dispo * 100.0 / total : 0.0;

    html += "</table>";
    html += "<div class='summary'>Total : " + QString::number(total) +
            " | Disponibles : " + QString::number(dispo) +
            " | Taux : " + QString::number(taux, 'f', 1) + "%</div>";

    html += R"(
    <div class='signature'>
        <p>Signé par le responsable RH</p>
        <div class='signature-line'></div>
    </div>

    <p style="text-align:center; font-style:italic; margin-top: 30px; color: gray;">
        🖊️ Vous pouvez modifier les horaires ou les remarques ci-dessus.<br>
        Ensuite, cliquez sur le bouton ci-dessous pour générer un PDF.
    </p>

    <div class='actions'>
        <button onclick=" generatePDF() ">📥 Télécharger une copie PDF personnalisée</button>
    </div>

</body>
</html>
)";

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << html;
        file.close();

        QMessageBox::information(this, "Succès", "Planning HTML généré avec succès !");
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));

        convertirHTMLenPDF(fileName);
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible d'écrire le fichier.");
    }
}



void MainWindow::convertirHTMLenPDF(const QString& cheminHTML)
{
    QString cheminPDF = cheminHTML;
    cheminPDF.replace(".html", ".pdf");

    QString wkhtmlPath = "C:/Program Files/wkhtmltopdf/bin/wkhtmltopdf.exe";

    if (!QFile::exists(wkhtmlPath)) {
        QMessageBox::warning(this, "Erreur", "wkhtmltopdf n'est pas installé !");
        return;
    }

    QStringList arguments;
    arguments << "--enable-local-file-access" << cheminHTML << cheminPDF;

    QProcess *process = new QProcess(this);
    process->start(wkhtmlPath, arguments);
    process->waitForFinished();

    if (QFile::exists(cheminPDF)) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(cheminPDF));
    } else {
        QMessageBox::warning(this, "Erreur", "La génération du PDF a échoué.");
    }
}



void MainWindow::afficherStatistiquesEmployes()
{
    int total = 0, dispo = 0, indispo = 0;
    QMap<QString, int> absentsParPoste, totalParPoste;
    QVector<QPair<QString, QDate>> anciens;

    QSqlQuery query("SELECT POSTE, DISPONIBILITE, DATE_EMBAUCHE, NOM, PRENOM FROM SMARTVACC.EMPLOYES");
    while (query.next()) {
        QString poste = query.value("POSTE").toString();
        bool disponible = query.value("DISPONIBILITE").toInt();
        QDate embauche = query.value("DATE_EMBAUCHE").toDate();
        QString nom = query.value("NOM").toString() + " " + query.value("PRENOM").toString();

        total++;
        totalParPoste[poste]++;
        if (disponible)
            dispo++;
        else {
            indispo++;
            absentsParPoste[poste]++;
        }

        anciens.append(qMakePair(nom, embauche));
    }

    double taux = total > 0 ? ((double)indispo / total) * 100 : 0;
    std::sort(anciens.begin(), anciens.end(), [](auto &a, auto &b) {
        return a.second < b.second;
    });

    QString top3;
    for (int i = 0; i < anciens.size() && i < 3; ++i) {
        top3 += QString::number(i + 1) + ". " + anciens[i].first + " (" + anciens[i].second.toString("dd/MM/yyyy") + ")<br>";
    }

    QString statistiques = R"(
<div style='font-family:"Segoe UI", sans-serif;
            font-size:14px;
            color:#ecf0f1;
            line-height:1.5;
            text-align:left;
            padding:10px;
            background: transparent;'>

    <h2 style='color:#e74c3c; text-align:center; font-size:16px; margin-bottom:10px;'>📊 Statistiques RH des Employés</h2>

    <p><b>Total :</b> )" + QString::number(total) + R"(</p>
    <p><b>✅ Disponibles :</b> )" + QString::number(dispo) + R"(</p>
    <p><b>❌ Absents :</b> )" + QString::number(indispo) + R"(</p>
)";

    if (taux > 50.0)
        statistiques += "<p style='color:red; font-weight:bold;'>🚨 Plus de 50% des employés sont absents !</p>";

    for (auto it = absentsParPoste.begin(); it != absentsParPoste.end(); ++it) {
        double tauxPoste = (double)it.value() / totalParPoste[it.key()] * 100;
        if (tauxPoste > 50.0)
            statistiques += "<p style='color:orange;'>⚠️ " + it.key() + " : " + QString::number(tauxPoste, 'f', 1) + "% absents</p>";
    }

    statistiques += "<p><b>📈 Taux d’absentéisme :</b> <span style='color:" + QString(taux > 50 ? "red" : "limegreen") + "; font-weight:bold;'>" +
                    QString::number(taux, 'f', 1) + "%</span></p>";

    statistiques += "<h3 style='margin-top:15px;'>👴 <b>Top 3 Anciens Employés :</b></h3><p>" + top3 + "</p></div>";

    ui->label_statistiques->setText(statistiques);
    ui->label_statistiques->setAlignment(Qt::AlignCenter);
    ui->label_statistiques->setStyleSheet(R"(
        QLabel {
            background: transparent;
            font-size: 14px;
            color: #ecf0f1;
            font-family: 'Segoe UI', sans-serif;
        }
    )");

    // 🔵 Camembert
    QPieSeries *series = new QPieSeries();
    if (total > 0) {
        double dispoPct = (double)dispo / total * 100.0;
        double absPct = (double)indispo / total * 100.0;

        QPieSlice *sliceDispo = series->append("Disponibles (" + QString::number(dispoPct, 'f', 1) + "%)", dispo);
        QPieSlice *sliceIndispo = series->append("Absents (" + QString::number(absPct, 'f', 1) + "%)", indispo);

        sliceDispo->setBrush(QColor("#2ecc71"));         // Vert
        sliceIndispo->setBrush(QColor("#e74c3c"));       // Rouge
        sliceIndispo->setExploded(true);
        sliceIndispo->setExplodeDistanceFactor(0.12);

        sliceDispo->setLabelVisible(true);
        sliceIndispo->setLabelVisible(true);
        sliceDispo->setLabelColor(Qt::white);
        sliceIndispo->setLabelColor(Qt::white);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("📊 Répartition des employés par disponibilité");
    chart->setTitleFont(QFont("Segoe UI", 14, QFont::Bold));
    chart->setTitleBrush(Qt::white);
    chart->setBackgroundBrush(QColor("#2c3e50"));  // Fond foncé
    chart->legend()->setLabelColor(Qt::white);
    chart->legend()->setAlignment(Qt::AlignBottom);

    if (chartViewStatistique) {
        chartViewStatistique->setParent(nullptr);
        delete chartViewStatistique;
        chartViewStatistique = nullptr;
    }

    chartViewStatistique = new QChartView(chart);
    chartViewStatistique->setRenderHint(QPainter::Antialiasing);

    if (QLayout *layout = ui->frame_stats->layout()) {
        QLayoutItem *child;
        while ((child = layout->takeAt(0)) != nullptr) {
            if (child->widget()) child->widget()->deleteLater();
            delete child;
        }
        layout->addWidget(chartViewStatistique);
    }

    afficherTableauAbsencesDetaillees();
}










void MainWindow::afficherTableauAbsencesDetaillees()
{
    ui->table_absences->setRowCount(0);
    ui->table_absences->setColumnCount(6); // Ajout d'une colonne
    ui->table_absences->setHorizontalHeaderLabels({
        "Nom", "Prénom", "Poste", "Taux d'absence", "Remarques", "Action RH"
    });

    QSqlQuery query("SELECT NOM, PRENOM, POSTE, DISPONIBILITE FROM SMARTVACC.EMPLOYES");
    while (query.next()) {
        QString nom = query.value("NOM").toString();
        QString prenom = query.value("PRENOM").toString();
        QString poste = query.value("POSTE").toString();
        int dispo = query.value("DISPONIBILITE").toInt();

        // Hypothèse : 20 jours travaillés par mois
        int absents = dispo ? 0 : 1;
        double taux = (absents / 20.0) * 100.0;

        QString remarque, couleur, actionRH;
        if (taux > 60.0) {
            remarque = "🚨 À surveiller !";
            couleur = "#e74c3c";
        } else if (taux > 40.0) {
            remarque = "⚠️ En observation";
            couleur = "#f39c12";
        } else if (taux > 20.0) {
            remarque = "🙂 Moyenne";
            couleur = "#2980b9";
        } else {
            remarque = "🌟 Excellent";
            couleur = "#27ae60";
        }

        // Action RH suggérée
        if (taux > 50.0)
            actionRH = "❌ Entretien disciplinaire";
        else if (taux >= 20.0)
            actionRH = "📋 Suivi trimestriel";
        else
            actionRH = "✅ Bonus annuel";

        int row = ui->table_absences->rowCount();
        ui->table_absences->insertRow(row);
        ui->table_absences->setItem(row, 0, new QTableWidgetItem(nom));
        ui->table_absences->setItem(row, 1, new QTableWidgetItem(prenom));
        ui->table_absences->setItem(row, 2, new QTableWidgetItem(poste));

        QTableWidgetItem* tauxItem = new QTableWidgetItem(QString::number(taux, 'f', 1) + " %");
        tauxItem->setBackground(QColor(couleur));
        tauxItem->setForeground(Qt::white);
        tauxItem->setTextAlignment(Qt::AlignCenter);
        ui->table_absences->setItem(row, 3, tauxItem);

        QTableWidgetItem* remarqueItem = new QTableWidgetItem(remarque);
        remarqueItem->setTextAlignment(Qt::AlignCenter);
        ui->table_absences->setItem(row, 4, remarqueItem);

        QTableWidgetItem* actionItem = new QTableWidgetItem(actionRH);
        actionItem->setTextAlignment(Qt::AlignCenter);
        ui->table_absences->setItem(row, 5, actionItem);
    }

    ui->table_absences->resizeColumnsToContents();
    ui->table_absences->horizontalHeader()->setStretchLastSection(true);
}



void MainWindow::on_button_fermer_chatbot_clicked()
{
    ui->frame_chatbox->hide();
}

void MainWindow::on_button_envoyer_clicked()
{
    QString question = ui->lineEdit_question->text().trimmed();
    if (question.isEmpty()) return;

    ui->textEdit_chat->append("🧍‍♀️ <b>Vous</b> : " + question);
    ui->lineEdit_question->clear();

    QString reponseFAQ = chercherReponseFAQ(question);
    if (!reponseFAQ.isEmpty()) {
        ui->textEdit_chat->append("🤖 <b>RHBot</b> : " + reponseFAQ);
        return;
    }

    // ⚡ Animation et cache
    ui->textEdit_chat->append("⏳ <i>RHBot réfléchit...</i>");
    if (cacheGPT.contains(question)) {
        ui->textEdit_chat->append("🤖 RHBot (⚡ cache) : " + cacheGPT.value(question));
        return;
    }

    envoyerRequeteChatGPT(question);
}

void MainWindow::envoyerRequeteChatGPT(const QString &message)
{
    QJsonObject json;
    QJsonArray messages;

    messages.append(QJsonObject{
        {"role", "system"},
        {"content", "Tu es un assistant RH pour une application Qt. Tu dois toujours répondre en français."}
    });
    messages.append(QJsonObject{
        {"role", "user"},
        {"content", message}
    });

    json["model"] = "mistral-7b-instruct.Q4_K_M";
    json["messages"] = messages;
    json["max_tokens"] = 150;

    QNetworkRequest request(QUrl("http://localhost:4891/v1/chat/completions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonDocument doc(json);
    manager->post(request, doc.toJson());
}




void MainWindow::receptionReponseGPT(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        ui->textEdit_chat->append("❌ Erreur API : " + reply->errorString());
        reply->deleteLater();
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    if (!doc.isObject()) {
        ui->textEdit_chat->append("❌ Réponse inattendue.");
        reply->deleteLater();
        return;
    }

    QJsonArray choices = doc.object()["choices"].toArray();
    if (!choices.isEmpty()) {
        QString content = choices[0].toObject()["message"].toObject()["content"].toString().trimmed();
        ui->textEdit_chat->append("🤖 RHBot : " + content);
        cacheGPT.insert(ui->lineEdit_question->text().trimmed(), content);  // cache
    } else {
        ui->textEdit_chat->append("❌ Réponse vide.");
    }

    reply->deleteLater();
}

QString MainWindow::chercherReponseFAQ(const QString &question)
{
    QString questionMin = question.toLower();
    for (const QString &cle : faqMap.keys()) {
        if (questionMin.contains(cle)) {
            return faqMap.value(cle);
        }
    }
    return ""; // Rien trouvé
}
void MainWindow::enregistrerConnexion(const QString& login) {
    QFile file("historique_connexions.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        QString dateHeure = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
        out << login << " | " << dateHeure << "\n";
        file.close();
    }
}







void MainWindow::afficherHistoriqueConnexions() {
    QFile file("historique_connexions.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier d'historique !");
        return;
    }

    QTextStream in(&file);
    ui->tableWidget_historique->clear();  // Nettoyage
    ui->tableWidget_historique->setRowCount(0);
    ui->tableWidget_historique->setColumnCount(2);
    ui->tableWidget_historique->setHorizontalHeaderLabels({"Utilisateur", "Date & Heure"});

    int row = 0;
    while (!in.atEnd()) {
        QString ligne = in.readLine().trimmed();
        if (ligne.isEmpty()) continue;

        QStringList parts = ligne.split("|");
        if (parts.size() == 2) {
            ui->tableWidget_historique->insertRow(row);
            ui->tableWidget_historique->setItem(row, 0, new QTableWidgetItem(parts[0].trimmed()));
            ui->tableWidget_historique->setItem(row, 1, new QTableWidgetItem(parts[1].trimmed()));
            row++;
        }
    }

    file.close();

    ui->tableWidget_historique->resizeColumnsToContents();
    ui->tableWidget_historique->horizontalHeader()->setStretchLastSection(true);
}



void MainWindow::on_pushButtonStat_15_clicked()
{
    // 🔄 Affiche la page "Statistique" dans l'onglet employé
    ui->Affichage->setCurrentWidget(ui->Statistique);  // Page nommée "Statistique"
}

void MainWindow::on_pushButton_pdf_3_clicked()
{
    // 🔄 Affiche la page "PDF" dans l'onglet employé
    ui->Affichage->setCurrentWidget(ui->pdf);  // Page nommée "pdf"
}


void MainWindow::envoyerRappelSMS()
{
    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "❌ Base de données non ouverte!";
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM, NUM, DATE_RDV FROM CARNETS");

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL :" << query.lastError().text();
        return;
    }

    QDateTime now = QDateTime::currentDateTime();
    QDateTime limite = now.addSecs(48 * 3600); // 48 heures max

    while (query.next()) {
        QString nom = query.value(0).toString();
        QString prenom = query.value(1).toString();
        QString num = query.value(2).toString();
        QDate rdv = query.value(3).toDate();
        QDateTime rdvDateTime(rdv, QTime(8, 0)); // On suppose RDV à 08:00

        if (rdvDateTime > now && rdvDateTime <= limite) {
            if (!num.startsWith("+216")) {
                num = "+216" + num;
            }

            QString msg = QString("📅 Bonjour %1 %2, ceci est un rappel de votre RDV prévu le %3.")
                              .arg(nom)
                              .arg(prenom)
                              .arg(rdv.toString("dd/MM/yyyy"));

            SmsNotif sms;
            if (sms.sendSMS(num, msg)) {
                QMessageBox::information(this, "Rappel SMS", QString("📩 SMS envoyé à %1 %2").arg(nom).arg(num));
            } else {
                QMessageBox::critical(this, "Erreur SMS", QString("Échec de l'envoi à %1 %2").arg(nom).arg(num));
            }
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
    ui->tableVieww->setModel(model);
    ui->tableVieww->hideColumn(0); // 🔹 Masquer la première colonne (ID)

}




void MainWindow::on_ajout_carnet_clicked()
{
    if (!estValides()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Erreur");
        msgBox.setText("Veuillez corriger les erreurs avant de valider le carnet.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setButtonText(QMessageBox::Ok, "D'accord");
        msgBox.exec();
        return;
    }


    QString cin = ui->cin->text().trimmed();
    QString nom = ui->nom_carnet->text().trimmed();
    QString prenom = ui->prenom_carnet->text().trimmed();
    int age = ui->age->text().toInt();
    QString num = ui->num->text().trimmed();
    float poids = ui->poids->text().toFloat();
    QDate date_rdv = ui->date_rdv->date();
    QString remarques = ui->remarques->toPlainText().trimmed();
    QString statut_vaccinal = ui->statut_vaccinal->currentText();
    QString sexe;
    if (ui->G->isChecked()) {
        sexe = "Garçon";
    } else if (ui->F->isChecked()) {
        sexe = "Femme";
    } else {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un sexe !");


        return;
    }

    if (cin.isEmpty() || nom.isEmpty() || prenom.isEmpty() || num.isEmpty() || age <= 0) {
        QMessageBox::warning(this, "Champs vides", "Veuillez remplir tous les champs obligatoires.");
        return;
    }

    Carnets carnet(0, cin, nom, prenom, age, sexe, num, poids, date_rdv, remarques, statut_vaccinal);

    if (modeModifications) {
        if (idAModifier.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Aucun CIN sélectionné pour modification !");
            return;
        }
        if (!estValides()) {
            QMessageBox::warning(this, "Erreur", "Veuillez corriger les erreurs avant de modifier le carnet.");
            return; //  Bloquer la modification si les champs ne sont pas valides
        }
        qDebug() << "Modification du carnet avec CIN :" << idAModifier;

        carnetTmp.supprimer(idAModifier);

        if (carnet.ajouter()) {
            QMessageBox::information(this, "Succès", "Carnet modifié avec succès !");
            ui->tab_2->setCurrentIndex(1);  // Rediriger vers l'onglet Affichage

        } else {
            QMessageBox::critical(this, "Erreur", "Le CIN existe déjà ! Impossible de modifier");
            return;
        }

        modeModifications = false;
        idAModifier = "";
    } else {
        if (carnet.ajouter()) {
            QMessageBox::information(this, "Succès", "Carnet ajouté avec succès !");
            ui->tab_2->setCurrentIndex(1);  // Rediriger vers l'onglet Affichage

        } else {
            QMessageBox::critical(this, "Erreur", "Le CIN existe déjà ! Impossible d'ajouter");
            return;
        }
    }

    displayCarnet();

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

void MainWindow::displayCarnet()
{
    QSqlQueryModel *model = carnetTmp.afficher();
    if (!model) {
        QMessageBox::warning(this, "Erreur", "Échec du chargement des carnets.");
        return;
    }

    ui->tableVieww->setModel(model);
    ui->tableVieww->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tableVieww->setAlternatingRowColors(true);
    ui->tableVieww->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableVieww->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableVieww->horizontalHeader()->setStretchLastSection(true);
    ui->tableVieww->resizeRowsToContents();

    for (int i = 0; i < model->columnCount(); ++i) {
        ui->tableVieww->horizontalHeader()->setSectionResizeMode(i,
                                                                 i <= 2 ? QHeaderView::ResizeToContents : QHeaderView::Stretch);
    }
}



void MainWindow::on_supprimerC_clicked()
{
    QString cin = ui->suppid->text().trimmed(); // Récupérer le CIN

    qDebug() << "🔍 CIN saisi pour suppression :" << cin;  //  Vérification du CIN

    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un CIN valide !");
        return;
    }

    // Vérifier si le CIN existe avant suppression
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM CARNETS WHERE CIN = :cin");
    checkQuery.bindValue(":cin", cin);

    if (!checkQuery.exec() || !checkQuery.next()) {
        qDebug() << "❌ Erreur SQL lors de la vérification de l'existence du carnet :" << checkQuery.lastError().text();
        QMessageBox::critical(this, "Erreur", "Impossible de vérifier l'existence du carnet !");
        return;
    }

    int count = checkQuery.value(0).toInt();
    if (count == 0) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Erreur");
        msgBox.setText("Aucun carnet trouvé avec ce CIN !");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setButtonText(QMessageBox::Ok, "D'accord");
        msgBox.exec();
        return;
    }


    // confirmation avec boutons "Oui" et "Non"
    QMessageBox msgBox;
    msgBox.setWindowTitle("Confirmation");
    msgBox.setText("Voulez-vous vraiment supprimer ce carnet ?");
    QPushButton *btnOui = msgBox.addButton("Oui", QMessageBox::YesRole);
    QPushButton *btnNon = msgBox.addButton("Non", QMessageBox::NoRole);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.exec();

    // Vérifier quel bouton a été cliqué
    if (msgBox.clickedButton() == btnOui) {
        if (carnetTmp.supprimer(cin)) {
            QMessageBox::information(this, "Succès", "Carnet supprimé avec succès !");
            displayCarnet();
        } else {
            qDebug() << "❌ Erreur lors de la suppression :" << checkQuery.lastError().text();
            QMessageBox::critical(this, "Erreur", "Échec de la suppression !");
        }
    }
}


void MainWindow::on_modifierC_clicked()
{

    QString cin = ui->suppid->text().trimmed();

    qDebug() << "🔍 CIN sélectionné pour modification :" << cin;  // 🔍 Vérification de la valeur entrée

    //  Vérifier si le champ CIN est vide
    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un CIN valide !");
        return;
    }

    //  Vérifier si le CIN existe dans la base de données avant modification
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM CARNETS WHERE CIN = :cin");
    checkQuery.bindValue(":cin", cin);

    if (!checkQuery.exec() || !checkQuery.next()) {
        qDebug() << "❌ Erreur SQL lors de la vérification de l'existence du carnet :" << checkQuery.lastError().text();
        QMessageBox::critical(this, "Erreur", "Impossible de vérifier l'existence du carnet !");
        return;
    }

    int count = checkQuery.value(0).toInt();
    if (count == 0) {
        QMessageBox::warning(this, "Erreur", "Aucun carnet trouvé avec ce CIN !");
        return;
    }

    //  Charger les informations du carnet si le CIN existe
    QSqlQuery query;
    query.prepare("SELECT CIN,NOM, PRENOM, AGE, SEXE, NUM, POIDS, DATE_RDV, REMARQUES, STATUT_VACCINAL FROM CARNETS WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    if (query.exec() && query.next()) {
        //  Remplir les champs avec les valeurs existantes

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

        //  Activer le mode modification
        idAModifier = cin;
        modeModifications = true;

        QMessageBox::information(this, "Modification", "Données chargées, vous pouvez modifier !");
        ui->tab_2->setCurrentIndex(0);  // 🔄 Rediriger vers l'onglet Ajout

    } else {

        qDebug() << "❌ Erreur SQL lors de la récupération des données :" << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Impossible de charger les données du carnet !");
    }
}



void MainWindow::on_btnPredire_clicked()
{
    QString cin = ui->idPredictionInput->text().trimmed(); // ✅ Récupérer le CIN depuis l'interface

    qDebug() << "🔍 CIN sélectionné pour la prédiction :" << cin;  // 🔍 Vérification

    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un CIN valide !");
        return;
    }

    // 🔴 Vérifier si le CIN existe dans la base de données avant la prédiction
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM CARNETS WHERE CIN = :cin");
    checkQuery.bindValue(":cin", cin);

    if (!checkQuery.exec() || !checkQuery.next()) {
        qDebug() << "❌ Erreur SQL lors de la vérification de l'existence du carnet :" << checkQuery.lastError().text();
        QMessageBox::critical(this, "Erreur", "Impossible de vérifier l'existence du carnet !");
        return;
    }

    int count = checkQuery.value(0).toInt();
    if (count == 0) {
        QMessageBox::warning(this, "Erreur", "Aucun carnet trouvé avec ce CIN !");
        return;
    }

    // ✅ Récupérer les informations pour la prédiction
    QString prediction = carnetTmp.analyserRisqueParCIN(cin);  // ✅ Nouvelle méthode basée sur CIN
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
void MainWindow::validateInput(QLineEdit *field, QLabel *errorLabel, QRegularExpression regex,
                               const QString &errorMsg, bool checkZeros, bool allowOnlyNumbers, int minValue, int maxValue)
{
    QString text = field->text().trimmed();

    // 🔴 Blocage immédiat des lettres pour CIN, Numéro, Poids et Âge
    if (allowOnlyNumbers && text.contains(QRegularExpression("[A-Za-z]"))) {
        text.chop(1); // Supprime le dernier caractère saisi
        field->setText(text);
        return;
    }

    // 🔴 Blocage immédiat des chiffres pour Nom et Prénom
    if (!allowOnlyNumbers && text.contains(QRegularExpression("\\d"))) {
        text.chop(1); // Supprime le dernier caractère saisi
        field->setText(text);
        return;
    }

    // 🔴 Vérifie si le champ contient uniquement des "0" (CIN et Numéro)
    if (checkZeros && text == "00000000") {
        field->setStyleSheet("border: 2px solid red; padding: 5px; background: white;");
        errorLabel->setText("❌ Tous les chiffres ne peuvent pas être 0 !");
        errorLabel->setStyleSheet("color: red; font-weight: bold; background: transparent;");
        return;
    }

    // 🔹 Vérifie les valeurs minimales et maximales (ex: Âge)
    if (minValue >= 0 && maxValue > 0) {
        bool isNumber;
        int value = text.toInt(&isNumber);
        if (!isNumber || value < minValue || value > maxValue) {
            field->setStyleSheet("border: 2px solid red; padding: 5px; background: white;");
            errorLabel->setText("❌ " + errorMsg);
            errorLabel->setStyleSheet("color: red; font-weight: bold; background: transparent;");
            return;
        }
    }

    // ✅ Vérification du format via l'expression régulière
    if (regex.match(text).hasMatch()) {
        field->setStyleSheet("border: 2px solid green; padding: 5px; background: white;");
        errorLabel->setText("✔️ Valide");
        errorLabel->setStyleSheet("color: green; font-weight: bold; background: transparent;");
    } else {
        field->setStyleSheet("border: 2px solid red; padding: 5px; background: white;");
        errorLabel->setText("❌ " + errorMsg);
        errorLabel->setStyleSheet("color: red; font-weight: bold; background: transparent;");
    }
}



void MainWindow::validateTextEdit(QTextEdit *field, QLabel *errorLabel, QRegularExpression regex, const QString &errorMsg)
{
    QString text = field->toPlainText().trimmed();  // ✅ Supprime les espaces inutiles

    // 🔹 Vérifie que le texte contient au moins 2 mots (séparés par un espace)
    QRegularExpression wordCheckRegex(R"(^[A-Za-zÀ-ÿ\s,]+$)");
    QStringList words = text.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

    if (regex.match(text).hasMatch() && wordCheckRegex.match(text).hasMatch() && words.size() >= 2) {
        // ✅ Texte valide (fond transparent)
        errorLabel->setText("✔️ Valide");
        errorLabel->setStyleSheet("color: green; font-weight: bold; background: transparent;");
    } else {
        // ❌ Texte invalide → Affiche un message d'erreur
        errorLabel->setText("❌ Le texte doit contenir au moins 2 mots, sans chiffres .");
        errorLabel->setStyleSheet("color: red; font-weight: bold; background: transparent;");
    }
}


void MainWindow::on_annulerr_clicked()
{
    if (modeModifications) {
        // 🔹 Si on est en mode modification, remettre les anciennes valeurs
        QSqlQuery query;
        query.prepare("SELECT CIN, NOM, PRENOM, AGE, SEXE, NUM, POIDS, DATE_RDV, REMARQUES, STATUT_VACCINAL FROM CARNETS WHERE CIN = :cin");
        query.bindValue(":cin", idAModifier);

        if (query.exec() && query.next()) {
            // ✅ Remettre toutes les anciennes valeurs, y compris le CIN
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
        }
    } else {
        // 🔹 Si on est en mode ajout, vider les champs
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
}
bool MainWindow::estValides()
{
    bool valide = true;
    QString styleErreur = "color: red; font-weight: bold; background: transparent;";
    QString styleValide = "color: green; font-weight: bold; background: transparent;";

    // 🔹 Vérification du nom (lettres uniquement)
    if (!QRegularExpression("^[A-Za-zÀ-ÿ\\s-]+$").match(ui->nom_carnet->text().trimmed()).hasMatch()) {
        ui->nomErrorLabel->setText("❌ Le nom doit contenir uniquement des lettres.");
        ui->nomErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->nomErrorLabel->setText("✔️ Valide");
        ui->nomErrorLabel->setStyleSheet(styleValide);
    }

    // 🔹 Vérification du prénom
    if (!QRegularExpression("^[A-Za-zÀ-ÿ\\s-]+$").match(ui->prenom_carnet->text().trimmed()).hasMatch()) {
        ui->prenomErrorLabel->setText("❌ Le prénom doit contenir uniquement des lettres.");
        ui->prenomErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->prenomErrorLabel->setText("✔️ Valide");
        ui->prenomErrorLabel->setStyleSheet(styleValide);
    }

    // 🔹 Vérification de l'âge (entre 1 et 100)
    if (!QRegularExpression("^[1-9][0-9]?$|^100$").match(ui->age->text().trimmed()).hasMatch()) {
        ui->ageErrorLabel->setText("❌ L'âge doit être entre 1 et 100.");
        ui->ageErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->ageErrorLabel->setText("✔️ Valide");
        ui->ageErrorLabel->setStyleSheet(styleValide);
    }

    // 🔹 Vérification du CIN (exactement 8 chiffres)
    if (!QRegularExpression("^[0-9]{8}$").match(ui->cin->text().trimmed()).hasMatch()) {
        ui->cinErrorLabel->setText("❌ Le CIN doit contenir exactement 8 chiffres.");
        ui->cinErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->cinErrorLabel->setText("✔️ Valide");
        ui->cinErrorLabel->setStyleSheet(styleValide);
    }

    // 🔹 Vérification du numéro de téléphone (exactement 8 chiffres)
    if (!QRegularExpression("^[0-9]{8}$").match(ui->num->text().trimmed()).hasMatch()) {
        ui->numErrorLabel->setText("❌ Le numéro doit contenir exactement 8 chiffres.");
        ui->numErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->numErrorLabel->setText("✔️ Valide");
        ui->numErrorLabel->setStyleSheet(styleValide);
    }

    // 🔹 Vérification du poids (nombre valide avec max 2 décimales)
    if (!QRegularExpression("^[0-9]{1,3}(\\.[0-9]{1,2})?$").match(ui->poids->text().trimmed()).hasMatch()) {
        ui->poidsErrorLabel->setText("❌ Le poids doit être un nombre valide.");
        ui->poidsErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->poidsErrorLabel->setText("✔️ Valide");
        ui->poidsErrorLabel->setStyleSheet(styleValide);
    }

    // 🔹 Vérification des remarques (min 2 mots, max 200 caractères)
    QString remarques = ui->remarques->toPlainText().trimmed();
    QStringList words = remarques.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    if (remarques.isEmpty() || words.size() < 2 || remarques.length() > 200) {
        ui->remarquesErrorLabel->setText("❌ Les remarques doivent contenir au moins 2 mots et max 200 caractères.");
        ui->remarquesErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->remarquesErrorLabel->setText("✔️ Valide");
        ui->remarquesErrorLabel->setStyleSheet(styleValide);
    }

    // 🔹 Vérification du sexe (obligatoire)
    if (!ui->G->isChecked() && !ui->F->isChecked()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un sexe !");
        valide = false;
    }

    return valide;
}

void MainWindow::on_btnGeneratePDF_clicked()
{
    QString cin = ui->suppid->text().trimmed();
    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un CIN !");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM, AGE, SEXE, NUM, POIDS, STATUT_VACCINAL, REMARQUES, DATE_RDV FROM CARNETS WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "Erreur", "Aucun patient trouvé !");
        return;
    }

    QString nom = query.value("NOM").toString();
    QString prenom = query.value("PRENOM").toString();
    QString age = query.value("AGE").toString();
    QString sexe = query.value("SEXE").toString();
    QString num = query.value("NUM").toString();
    QString poids = query.value("POIDS").toString();
    QString statut = query.value("STATUT_VACCINAL").toString();
    QString remarques = query.value("REMARQUES").toString();
    QString dateRdv = query.value("DATE_RDV").toDate().toString("dd/MM/yyyy");

    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer le carnet PDF", "", "PDF Files (*.pdf)");
    if (filePath.isEmpty()) return;

    QPdfWriter pdf(filePath);
    QPageLayout layout(QPageSize(QPageSize::A4), QPageLayout::Landscape, QMarginsF(30,30,30,30));
    pdf.setPageLayout(layout);
    pdf.setResolution(300);

    QPainter painter(&pdf);
    QTextDocument doc;

    QString html = R"(
<html>
<head>
<style>
    body { font-family: Arial, sans-serif; font-size: 42pt; }
    .header { width: 100%; margin-bottom: 50px; }
    .header img { vertical-align: middle; width:180px; height:180px; }
    .title { color: #800000; font-weight:bold; display:inline-block; vertical-align: middle; margin-left:15px; font-size: 48pt; }

    .main-table {
        width: 100%;
        border-collapse: collapse;
        border: none;
    }

    .main-table td {
        vertical-align: top;
        border: none;
        padding: 0;
    }

.data-column { width: 60%; padding-right: 20px; }
.rdv-column { width: 40%; }

    .patient-table, .rdv-table {
        border-collapse: collapse;
        width: 100%;
    }

    .patient-table td, .patient-table th,
    .rdv-table td, .rdv-table th {
        border: 4px solid black;
        padding: 35px;
        font-size: 42pt;
    }

.signature {
    color: #800000;
    font-style: italic;
    margin-top: 120px;
    font-size: 38pt;
    text-align: right; /* ✅ Aligner à droite */
}

</style>
</head>
<body>
    <div class='header'>
        <img src=':/images/logo.png'>
        <span class='title'>🩺 Carnet de Vaccination - Patient</span>
    </div>

    <table class='main-table'>
        <tr>
            <td class='data-column'>
                <table class='patient-table'>
                    <tr><th>Nom</th><td>)" + nom + R"(</td></tr>
                    <tr><th>Prénom</th><td>)" + prenom + R"(</td></tr>
                    <tr><th>CIN</th><td>)" + cin + R"(</td></tr>
                    <tr><th>Âge</th><td>)" + age + R"( ans</td></tr>
                    <tr><th>Sexe</th><td>)" + sexe + R"(</td></tr>
                    <tr><th>Téléphone</th><td>)" + num + R"(</td></tr>
                    <tr><th>Poids</th><td>)" + poids + R"( kg</td></tr>
                    <tr><th>Statut Vaccinal</th><td>)" + statut + R"(</td></tr>
                    <tr><th>Remarques</th><td>)" + remarques + R"(</td></tr>
                </table>
                <div class='signature'>
                    ✒️ Signature & Cachet du Centre de vaccination
                </div>
            </td>
            <td class='rdv-column'>
<table class='rdv-table'>
    <tr>
        <th colspan='2' style="border: 4px solid black;">📅 <strong>Prochain Rendez-vous</strong></th>
    </tr>
    <tr>
        <td colspan='2' style='text-align:center; border-left: 4px solid black; border-right: 4px solid black; border-bottom: 4px solid black;'>)" + dateRdv + R"(</td>
    </tr>
    <tr>
        <td colspan='2' style='height:60px; border-left: 4px solid black; border-right: 4px solid black; border-bottom: 4px solid black;'></td>
    </tr>
    <tr>
        <td colspan='2' style='height:60px; border-left: 4px solid black; border-right: 4px solid black; border-bottom: 4px solid black;'></td>
    </tr>
    <tr>
        <td colspan='2' style='height:60px; border-left: 4px solid black; border-right: 4px solid black; border-bottom: 4px solid black;'></td>
    </tr>
    <tr>
        <td colspan='2' style='height:60px; border-left: 4px solid black; border-right: 4px solid black; border-bottom: 4px solid black;'></td>
    </tr>
    <tr>
        <td colspan='2' style='height:60px; border-left: 4px solid black; border-right: 4px solid black; border-bottom: 4px solid black;'></td>
    </tr>
    <tr>
        <td colspan='2' style='height:60px; border-left: 4px solid black; border-right: 4px solid black; border-bottom: 4px solid black;'></td>
    </tr>
    <tr>
        <td colspan='2' style='height:60px; border-left: 4px solid black; border-right: 4px solid black; border-bottom: 4px solid black;'></td>
    </tr>
</table>


            </td>
        </tr>
    </table>
</body>
</html>
)";


    doc.setHtml(html);
    doc.setPageSize(QSizeF(pdf.width(), pdf.height()));
    doc.drawContents(&painter);

    painter.end();

    QMessageBox::information(this, "PDF Généré", "📄 Le carnet a été exporté avec succès !");
}
void MainWindow::on_btnStat_clicked()
{
    ui->tab_2->setCurrentIndex(3);

    if (!ui->stat_carnet) {
        qDebug() << "Erreur : L'onglet stat_carnet n'existe pas";
        return;
    }

    QLayout *oldLayout = ui->stat_carnet->layout();
    if (oldLayout) {
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            if (item->widget()) item->widget()->deleteLater();
            delete item;
        }
        delete oldLayout;
        ui->stat_carnet->setLayout(nullptr);
    }

    int totalV = 0, totalNV = 0;
    QSqlQuery query("SELECT STATUT_VACCINAL FROM CARNETS");
    while (query.next()) {
        QString statut = query.value(0).toString().toLower().trimmed();
        if (statut == "vacciné" || statut == "vacciner" || statut == "vaccine")
            totalV++;
        else
            totalNV++;
    }

    int total = totalV + totalNV;
    double pourcentageV = total > 0 ? (double(totalV) / total) * 100 : 0;
    double pourcentageNV = total > 0 ? (double(totalNV) / total) * 100 : 0;

    QPieSeries *series = new QPieSeries();
    series->append("", totalV);
    series->append("", totalNV);
    series->setLabelsVisible(true);

    QPieSlice *sliceV = series->slices().at(0);
    QPieSlice *sliceNV = series->slices().at(1);
    sliceV->setBrush(Qt::green);
    sliceNV->setBrush(Qt::red);

    // 🎯 Appliquer les labels personnalisés avec taille + gras
    QFont sliceFont("Arial", 16, QFont::Bold);
    sliceV->setLabel(QString("Vaccinés (%1%)").arg(QString::number(pourcentageV, 'f', 1)));
    sliceNV->setLabel(QString("Non Vaccinés (%1%)").arg(QString::number(pourcentageNV, 'f', 1)));
    sliceV->setLabelFont(sliceFont);
    sliceNV->setLabelFont(sliceFont);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("📊 Couverture vaccinale totale (en %)");
    chart->setTitleFont(QFont("Arial", 18, QFont::Bold));
    chart->legend()->hide();  // ✅ Supprime la légende du bas

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(800, 500);

    // ✅ Résumé simple en bas (plus de redondance)
    QLabel *summary = new QLabel(
        QString("<span style='color:green; font-size: 22px; font-weight:bold;'>🟢 Vaccinés : %1%</span>"
                "&nbsp;&nbsp;&nbsp;"
                "<span style='color:red; font-size: 22px; font-weight:bold;'>🔴 Non Vaccinés : %2%</span>")
            .arg(QString::number(pourcentageV, 'f', 1))
            .arg(QString::number(pourcentageNV, 'f', 1)));
    summary->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(chartView);
    layout->addWidget(summary);
    ui->stat_carnet->setLayout(layout);
}



void MainWindow::trierCarnets(const QString &critere)
{
    QString requete = "SELECT STATUT_VACCINAL, AGE, SEXE, CIN, NUM, NOM, POIDS, DATE_RDV, PRENOM, REMARQUES FROM CARNETS";

    if (critere == "Âge") {
        requete += " ORDER BY AGE ASC";
    } else if (critere == "Date de RDV") {
        requete += " ORDER BY DATE_RDV ASC";
    } else if (critere == "Poids") {
        requete += " ORDER BY POIDS ASC";
    }

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(requete);
    ui->tableVieww->setModel(model);
    ui->tableVieww->resizeColumnsToContents();

    // ✅ Plus besoin de hideColumn(0), car l’ID n’est même pas sélectionné
}








// Daoussar




// =============================
// 🔁 NAVIGATION : Aller vers la page des compagnes
// =============================


void MainWindow::navigateToCampaigns()
{
    ui->stackedWidget->setCurrentIndex(4);  // Accès à la page des compagnes
    ui->frame->setVisible(true);            // Affichage de la sidebar
    displayCompagne();                      // Affiche la liste des compagnes
}

void MainWindow::displayCompagne()
{
    QSqlQueryModel *model = compagneTmp.afficher();  // Appelle la méthode de la classe Compagne

    //pour enlever l'heures de date
    if (model) {
        ui->tableView_d->setModel(model);              // Lier le modèle au tableau


        for (int row = 0; row < model->rowCount(); ++row) {
            for (int col = 0; col < model->columnCount(); ++col) {
                QString value = model->data(model->index(row, col)).toString();
                if (value.contains("00:00"))
                    model->setData(model->index(row, col), value.section(' ', 0, 0));
            }
        }



        ui->tableView_d->resizeColumnsToContents();    // Ajuster la taille des colonnes
    } else {
        QMessageBox::warning(this, "Erreur", "Échec du chargement des compagnes.");
    }

}


bool MainWindow::estValide_d()
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







void MainWindow::on_button_modifierD_clicked()
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

    modeModification_d = true;
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
    if (!estValide_d()) {
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

    if (modeModification_d) {
        // 🔹 Mode Modification : Mettre à jour l'enregistrement
        if (compagne.modifier(nomOriginal)) {
            QMessageBox::information(this, "Succès", "Compagne modifiée avec succès !");
            modeModification_d = false; // ✅ Désactiver le mode modification après mise à jour
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
    query.prepare(R"(
        SELECT
            NOM_COMPAGNE,
            TO_CHAR(DATE_DEBUT, 'DD/MM/YYYY') AS DATE_DEBUT,
            TO_CHAR(DATE_FIN, 'DD/MM/YYYY') AS DATE_FIN,
            ZONE_GEOGRAPHIQUE,
            STATUT
        FROM COMPAGNE
        WHERE DATE_DEBUT <= :date AND DATE_FIN >= :date
    )");
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
    query.prepare(R"(
        SELECT
            NOM_COMPAGNE,
            TO_CHAR(DATE_DEBUT, 'DD/MM/YYYY') AS DATE_DEBUT,
            TO_CHAR(DATE_FIN, 'DD/MM/YYYY') AS DATE_FIN,
            ZONE_GEOGRAPHIQUE,
            STATUT
        FROM COMPAGNE
        WHERE :date BETWEEN DATE_DEBUT AND DATE_FIN
    )");

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

    ui->tableView_d->setModel(model);
    ui->tableView_d->resizeColumnsToContents();
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
    ui->tableView_d->setModel(model);
    ui->tableView_d->resizeColumnsToContents();
}



//Annuler

void MainWindow::on_pushButton_annuler_clicked()
{
    if (modeModification_d) {
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


void MainWindow::on_pushButtonStat_14_clicked()
{
     ui->Affichage_6->setCurrentWidget(ui->tab_7);
}










//mayssem labo







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

    populateNomlabComboBox();
    populateLabNameComboBox();
    // Update the table view
    ui->tableViewmay->setModel(labTmp.afficher());

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

void MainWindow::clearAjoutFields()
{
    ui->lineEdit_NomLab->clear();
    ui->lineEdit_Adresse->clear();
    ui->comboBox_Responsable->setCurrentIndex(-1); // or 0
    ui->lineEdit_Type->clear();
    ui->comboBox_Statut->setCurrentIndex(-1); // or 0
    ui->spinBox_NbProjets->setValue(0);
    ui->spinBox_NbProjets_2->setValue(0);
    ui->spinBox_NbProjets_3->setValue(0);
    ui->doubleSpinBox_7->setValue(0.0);
    ui->dateEdit_7->setDate(QDate::currentDate());
}

void MainWindow::displayLaboratoires()
{
    QSqlTableModel *model = labTmp.afficher();

    if (model) {
        ui->tableViewmay->setModel(model);
        ui->tableViewmay->hideColumn(0); // Hides the NUM (ID) column
        ui->tableViewmay->resizeColumnsToContents();
        ui->tableViewmay->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        ui->tableViewmay->setStyleSheet(
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
        ui->tableViewmay->setModel(labTmp.afficher());
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
    ui->tableViewmay->setModel(model);
    ui->tableViewmay->hideColumn(0);  // Optional: hide ID
    ui->tableViewmay->resizeColumnsToContents();
    ui->tableViewmay->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewmay->setVisible(true);
}


//tri

void MainWindow::on_comboBox_Tri_currentIndexChanged_M()
{
    QString sortBy = ui->comboBox_Tri->currentText();

    QSqlQueryModel *model = new QSqlQueryModel();

    QString query = "SELECT * FROM SMARTVACC.LABORATOIRES";

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

    ui->tableViewmay->setModel(model);

    // ✅ Hide only ID_LAB (column 0)
    ui->tableViewmay->setColumnHidden(0, true);

    // ✅ Make sure NOM_LAB (column 1) and others are shown
    for (int col = 1; col < model->columnCount(); ++col) {
        ui->tableViewmay->setColumnHidden(col, false);
    }

    // ✅ Ensure table stays visible and nicely formatted
    ui->tableViewmay->resizeColumnsToContents();
    ui->tableViewmay->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewmay->setVisible(true); // Force show the table if needed
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

        // Only add labs with non-zero values for percentages
        if (stats.nbProjets == 0 && stats.personnel == 0 && stats.matriels == 0 && stats.depense == 0) {
            continue; // Skip lab if all values are zero
        }

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
    QString titleText = "Statistiques des dépenses par chaque Laboratoires \n🏆 Meilleur labo : " + bestLabName;
    chart->setTitle(titleText);

    // Set font to bold and bigger
    QFont titleFont;
    titleFont.setBold(true);
    titleFont.setPointSize(14); // You can increase or decrease as needed
    chart->setTitleFont(titleFont);

    // Set title color to red
    chart->setTitleBrush(QBrush(Qt::red));

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

    // Resize the chart view to make it bigger
    chartView->setFixedWidth(1270);
    chartView->setFixedHeight(690);

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

    // Display color legend next to the performance index (updated with larger font size)
    QLabel *legendLabel = new QLabel("🟦 Projets | 🟩 Personnel | 🟨 Matériels | 🟥 Dépense");
    legendLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    ui->Statistique_7->layout()->addWidget(legendLabel);

    // Print best performing lab
    qDebug() << "🏅 Meilleur labo basé sur l'indice de performance:" << bestLabName
             << "avec un indice de" << bestPerformanceIndex;
}



void MainWindow::onTabWidgetPageChanged(int index)
{

    if (index == 2) {
        displayStatisticsChart();
    }
}
void MainWindow::goToStatistiques()
{

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

    QAxObject *excel = new QAxObject("Excel.Application");
    if (!excel)
        return false;

    excel->setProperty("Visible", false);
    QAxObject *workbooks = excel->querySubObject("Workbooks");
    QAxObject *workbook = workbooks->querySubObject("Add");
    QAxObject *sheet = workbook->querySubObject("Sheets(int)", 1);

    QStringList headers = {
        "Nom du Laboratoire", "Responsable", "Depenses", "Personnel", "Equipements", "Projets Actifs", "Date de Creation"
    };

    // === Insert title ===
    QString title = "Liste des Laboratoires";
    QAxObject *titleRange = sheet->querySubObject("Range(const QString&)", "A1:G1");
    titleRange->setProperty("MergeCells", true);
    titleRange->setProperty("Value", title);

    QAxObject *titleFont = titleRange->querySubObject("Font");
    titleFont->setProperty("Bold", true);
    titleFont->setProperty("Size", 16);
    delete titleFont;

    QAxObject *titleAlign = titleRange->querySubObject("HorizontalAlignment");
    titleRange->setProperty("HorizontalAlignment", -4108); // Center alignment
    delete titleAlign;
    delete titleRange;

    // === Headers start at row 3 ===
    int headerRow = 3;
    for (int i = 0; i < headers.size(); ++i) {
        QAxObject *cell = sheet->querySubObject("Cells(int,int)", headerRow, i + 1);
        cell->setProperty("Value", headers[i]);

        QAxObject *interior = cell->querySubObject("Interior");
        interior->setProperty("Color", QColor(255, 200, 200).rgb()); // Light red background
        delete interior;

        QAxObject *font = cell->querySubObject("Font");
        font->setProperty("Bold", true);
        delete font;
    }

    // === Fill table starting from row 4 ===
    int row = headerRow + 1;
    while (query.next()) {
        sheet->querySubObject("Cells(int,int)", row, 1)->setProperty("Value", query.value("NOM_LAB").toString());
        sheet->querySubObject("Cells(int,int)", row, 2)->setProperty("Value", query.value("RESPONSABLE").toString());
        sheet->querySubObject("Cells(int,int)", row, 3)->setProperty("Value", query.value("DEPONSE").toDouble());
        sheet->querySubObject("Cells(int,int)", row, 4)->setProperty("Value", query.value("PERSONNEL").toInt());
        sheet->querySubObject("Cells(int,int)", row, 5)->setProperty("Value", query.value("MATRIELS").toInt());
        sheet->querySubObject("Cells(int,int)", row, 6)->setProperty("Value", query.value("NB_PROJETS").toInt());
        sheet->querySubObject("Cells(int,int)", row, 7)->setProperty("Value", query.value("DATE_CREATION").toDate().toString("yyyy-MM-dd"));
        row++;
    }

    int totalRows = row - 1;

    // === Create Excel table ===
    QString startCol = "A";
    QString endCol = QString(QChar('A' + static_cast<int>(headers.size()) - 1));
    QString excelRange = QString("%1%2:%3%4").arg(startCol).arg(headerRow).arg(endCol).arg(totalRows);

    QAxObject *rangeObj = sheet->querySubObject("Range(const QString&)", excelRange);
    QAxObject *listObjects = sheet->querySubObject("ListObjects");

    if (rangeObj && listObjects) {
        QAxObject *table = listObjects->querySubObject(
            "Add(int, QVariant, QVariant, int, QVariant)",
            1, QVariant::fromValue(rangeObj), QVariant(), 1, QVariant());
        delete table;
    }

    // === Autofit columns ==
    QAxObject *columns = sheet->querySubObject("Columns");
    columns->dynamicCall("AutoFit()");
    delete columns;

    // === Save and clean up ===
    workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(cheminFichier));
    workbook->dynamicCall("Close()");
    excel->dynamicCall("Quit()");

    delete rangeObj;
    delete listObjects;
    delete sheet;
    delete workbook;
    delete workbooks;
    delete excel;

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
            QString nom = query.value(0).toString();
            ui->comboBox_nomlab->addItem(nom);
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération des responsables.");
    }

    ui->comboBox_nomlab->setCurrentIndex(0);
}



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
void MainWindow::verifierNomProduit()
{
    QString nomProduit = ui->nomProduit_6->text().trimmed();

    ui->labelErreurNom_6->clear();
    ui->labelErreurNom_6->setVisible(false);
    ui->nomProduit_6->setStyleSheet("");

    QRegularExpression regexNom("^[A-Za-zÀ-ÿ ]+$");

    if (nomProduit.isEmpty() || nomProduit.length() < 3 || !regexNom.match(nomProduit).hasMatch()) {
        ui->labelErreurNom_6->setText("❌ Le nom doit contenir au moins 3 lettres et pas de caractères spéciaux.");
        ui->labelErreurNom_6->setStyleSheet("color: black; font-size: 12px; font-style: italic;");
        ui->labelErreurNom_6->setVisible(true);
        ui->nomProduit_6->setStyleSheet("border: 2px solid red;");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM PRODUITS WHERE NOM_PRODUIT = :nomProduit");
    query.bindValue(":nomProduit", nomProduit);

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL lors de la vérification d'unicité :" << query.lastError().text();
        return;
    }

    query.next();
    int count = query.value(0).toInt();

    if (count > 0) {
        ui->labelErreurNom_6->setText("❌ Ce nom de produit est déjà utilisé !");
        ui->labelErreurNom_6->setStyleSheet("color: red; font-size: 12px; font-style: italic;");
        ui->labelErreurNom_6->setVisible(true);
        ui->nomProduit_6->setStyleSheet("border: 2px solid red;");
    } else {
        ui->labelErreurNom_6->setText("✔ Ce nom de produit est valide !");
        ui->labelErreurNom_6->setStyleSheet("color: black; font-size: 12px; font-style: italic;");
        ui->labelErreurNom_6->setVisible(true);
        ui->nomProduit_6->setStyleSheet("border: 2px solid green;");
    }
}
void MainWindow::validerChampsP()
{
    QString nom = ui->nomProduit_6->text().trimmed();
    QString nomFournisseur = ui->nomFournisseur_6->text().trimmed();
    QDate dateFabrication = ui->dateFabrication_6->date();
    QDate dateExpiration = ui->dateExpiration_6->date();
    int quantite = ui->quantite_6->value();

    QRegularExpression regexNom("^[A-Za-zÀ-ÿ ]+$");

    if (nom.isEmpty() || nom.length() < 3 || !regexNom.match(nom).hasMatch()) {
        ui->nomProduit_6->setStyleSheet("border: 2px solid red;");
        ui->labelErreurNom_6->setText("❌ Le nom doit contenir au moins 3 lettres.");
        ui->labelErreurNom_6->setStyleSheet("color: black; font-size: 12px; font-style: italic;");
        ui->labelErreurNom_6->setVisible(true);
    } else {
        ui->nomProduit_6->setStyleSheet("border: 2px solid green;");
        ui->labelErreurNom_6->clear();
        ui->labelErreurNom_6->setVisible(false);
    }

    if (nomFournisseur.isEmpty() || nomFournisseur.length() < 3 || !regexNom.match(nomFournisseur).hasMatch()) {
        ui->nomFournisseur_6->setStyleSheet("border: 2px solid red;");
        ui->labelErreurFournisseur_6->setText("❌ Le nom du fournisseur doit contenir au moins 3 lettres.");
        ui->labelErreurFournisseur_6->setStyleSheet("color: black; font-size: 12px; font-style: italic;");
        ui->labelErreurFournisseur_6->setVisible(true);
    } else {
        ui->nomFournisseur_6->setStyleSheet("border: 2px solid green;");
        ui->labelErreurFournisseur_6->clear();
        ui->labelErreurFournisseur_6->setVisible(false);
    }

    if (dateExpiration <= dateFabrication) {
        ui->dateExpiration_6->setStyleSheet("border: 2px solid red;");
        ui->labelErreurDateExpiration_6->setText("❌ La date d'expiration doit être après la fabrication.");
        ui->labelErreurDateExpiration_6->setStyleSheet("color: black; font-size: 12px; font-style: italic;");
        ui->labelErreurDateExpiration_6->setVisible(true);
    } else {
        ui->dateExpiration_6->setStyleSheet("border: 2px solid green;");
        ui->labelErreurDateExpiration_6->clear();
        ui->labelErreurDateExpiration_6->setVisible(false);
    }
    if (quantite <= 0) {
        ui->quantite_6->setStyleSheet("border: 2px solid red;");
        ui->labelErreurQuantite_6->setText("❌ La quantité doit être supérieure à 0.");
        ui->labelErreurQuantite_6->setStyleSheet("color: black; font-size: 12px; font-style: italic;");
        ui->labelErreurQuantite_6->setVisible(true);
    } else {
        ui->quantite_6->setStyleSheet("border: 2px solid green;");
        ui->labelErreurQuantite_6->clear();
        ui->labelErreurQuantite_6->setVisible(false);
    }
}






void MainWindow::on_supprimer_6_clicked()
{
    QString nomProduit = ui->champRecherche_12->text().trimmed();

    if (nomProduit.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un nom de produit à supprimer.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM PRODUITS WHERE NOM_PRODUIT = :nomProduit");
    query.bindValue(":nomProduit", nomProduit);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la vérification du produit !");
        qDebug() << "❌ Erreur SQL (vérification de l'existence) :" << query.lastError().text();
        return;
    }

    query.next();
    int count = query.value(0).toInt();

    if (count == 0) {
        QMessageBox::warning(this, "Erreur", "Le produit '" + nomProduit + "' n'existe pas !");
        return;
    }

    Produit produit;
    if (produit.supprimer(nomProduit)) {
        QMessageBox::information(this, "Succès", "Produit supprimé avec succès !");

        // 🔄 Mettre à jour l'affichage après suppression
        ui->TableViewP_6->setModel(nullptr);
        ui->TableViewP_6->setModel(produit.afficher());
        ui->TableViewP_6->resizeColumnsToContents();

        // 🔄 Nettoyer le champ de saisie
        ui->champRecherche_11->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression du produit !");
    }
}


void MainWindow::on_Affichage_31_currentChanged(int index)
{
    if (index == 1) {
        Produit produit;
            // Exemple : largeur 1000px, hauteur 600px

        ui->TableViewP_6->setModel(produit.afficher());
        ui->TableViewP_6->setMinimumSize(1000, 600);
        ui->TableViewP_6->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // 📐 Ajustement des colonnes et lignes
        ui->TableViewP_6->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->TableViewP_6->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    }
}



void MainWindow::on_ajouter_6_clicked()
{ if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "Base de données non connectée !";

    }



    QString nomProduit = ui->nomProduit_6->text().trimmed();
    QString categorie = ui->categorie_6->currentText().trimmed();
    int quantite = ui->quantite_6->value();
    QDate dateFabrication = ui->dateFabrication_6->date();
    QDate dateExpiration = ui->dateExpiration_6->date();
    QString nomFournisseur = ui->nomFournisseur_6->text().trimmed();

    qDebug() << "Nom Produit:" << nomProduit << "| Catégorie:" << categorie << "| Quantité:" << quantite
             << "| Date Fabrication:" << dateFabrication << "| Date Expiration:" << dateExpiration
             << "| Nom Fournisseur:" << nomFournisseur;
    if (!ui->labelErreurNom_6->text().isEmpty() && ui->labelErreurNom_6->text().contains("déjà utilisé")) {
        QMessageBox::warning(this, "Erreur", "Ce nom de produit existe déjà. Veuillez en choisir un autre.");
        return;
    }
    if (nomProduit.isEmpty() || categorie.isEmpty() || nomFournisseur.isEmpty() || quantite <= 0) {
        QMessageBox::warning(this, "Champs vides", "Veuillez remplir tous les champs obligatoires.");
        return;
    }

    if (dateExpiration <= dateFabrication) {
        QMessageBox::warning(this, "Date invalide", "La date d'expiration doit être après la date de fabrication !");
        return;
    }

    Produit produit(0, nomProduit, categorie, quantite, dateFabrication, dateExpiration, nomFournisseur);

    if (modeModification && !produitAModifier.isEmpty()) {
        qDebug() << "Mode Modification - Produit :" << produitAModifier;
        qDebug() << "DEBUG - Valeurs saisies:"
                 << "\nNom:" << nomProduit
                 << "\nCatégorie:" << categorie
                 << "\nQuantité:" << quantite
                 << "\nDate Fab:" << dateFabrication
                 << "\nDate Exp:" << dateExpiration
                 << "\nFournisseur:" << nomFournisseur;
        if (produit.modifier(produitAModifier,nomProduit)) {
            QMessageBox::information(this, "Succès", "Produit modifié avec succès !");
            modeModification = false;
            produitAModifier.clear();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la modification !");
            return;
        }
    } else {
        qDebug() << "Mode Ajout - Nouveau produit";
        if (produit.ajouter()) {
            QMessageBox::information(this, "Succès", "Produit ajouté avec succès !");
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de l'ajout !");
            return;
        }
    }
    ui->stackedWidget->setCurrentIndex(1);
    ui->Affichage_31->setCurrentIndex(1);
    ui->TableViewP_6->setModel(nullptr);
    ui->TableViewP_6->setModel(produit.afficher());
    ui->TableViewP_6->resizeColumnsToContents();

    ui->nomProduit_6->clear();
    ui->categorie_6->setCurrentIndex(0);
    ui->quantite_6->setValue(0);
    ui->dateFabrication_6->setDate(QDate::currentDate());
    ui->dateExpiration_6->setDate(QDate::currentDate());
    ui->nomFournisseur_6->clear();
}




void MainWindow::on_modifier_6_clicked()
{
    QString nomProduit = ui->champRecherche_12->text().trimmed();

    if (nomProduit.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un nom de produit valide !");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM PRODUITS WHERE NOM_PRODUIT = :nomProduit");
    query.bindValue(":nomProduit", nomProduit);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur SQL : " + query.lastError().text());
        return;
    }

    query.next();
    int count = query.value(0).toInt();

    if (count == 0) {
        QMessageBox::warning(this, "Erreur", "Le produit '" + nomProduit + "' n'existe pas !");
        ui->stackedWidget->setCurrentIndex(1);
        ui->Affichage_31->setCurrentIndex(1);
        ui->champRecherche_11->clear();
        return;

    }

    remplirChampsModificationP(nomProduit);

    ui->stackedWidget->setCurrentIndex(1);
    ui->Affichage_31->setCurrentIndex(0);
    ui->champRecherche_11->clear();
}

void MainWindow::remplirChampsModificationP(QString nomProduit)
{
    QString categorie, nomFournisseur;
    int quantite;
    QDate dateFabrication, dateExpiration;

    Produit produit;
    if (produit.remplirChampsModification(nomProduit, categorie, quantite, dateFabrication, dateExpiration, nomFournisseur)) {
        ui->nomProduit_6->setText(nomProduit);
        ui->categorie_6->setCurrentText(categorie);
        ui->quantite_6->setValue(quantite);
        ui->dateFabrication_6->setDate(dateFabrication);
        ui->dateExpiration_6->setDate(dateExpiration);
        ui->nomFournisseur_6->setText(nomFournisseur);

        produitAModifier = nomProduit;
        modeModification=true;

        QMessageBox::information(this, "Modification", "Données chargées, vous pouvez modifier !");
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de charger les données du produit !");
    }
}




void MainWindow::on_pushButton_26_clicked()
{
    qDebug()<<"bouton annuler";


    ui->nomProduit_6->clear();
    ui->categorie_6->setCurrentIndex(0);
    ui->quantite_6->setValue(0);
    ui->dateFabrication_6->setDate(QDate::currentDate());
    ui->dateExpiration_6->setDate(QDate::currentDate());
    ui->nomFournisseur_6->clear();


    modeModification = false;
    produitAModifier.clear();


    ui->stackedWidget->setCurrentIndex(1);
    ui->Affichage_31->setCurrentIndex(1);
}



void MainWindow::on_rechercherP_clicked()
{ QString critere = ui->champRecherche_11->text().trimmed();

    Produit produit;
    QSqlQueryModel *model = produit.rechercherTout(critere);

    ui->TableViewP_6->setModel(model);
    ui->TableViewP_6->resizeColumnsToContents();
    if (model->rowCount()==0){
        QMessageBox::information(this, "Recherche","Aucun resultat trouvé ");
    }

}




/*

void MainWindow::on_pushButton_clicked()
{
    Produit produit;
    QSqlQueryModel* model = produit.trierPar("EXP_DESC");

    if (model) {
        ui->TableViewP_6->setModel(model);
        ui->TableViewP_6->resizeColumnsToContents();
    }312

}
*/

void MainWindow::on_comboBox_11_activated(int index)
{
    Produit produit;
    QSqlQueryModel* model = nullptr;

    if (index == 0) {
        model = produit.trierPar("EXP_DESC");
    } else if (index == 1) {
        model = produit.trierPar("FAB_ASC");
    } else if (index == 2) {
        model = produit.trierPar("QTY_ASC");
    } else {
        return;
    }

    if (model) {
        //ui->TableViewP_6->setSortingEnabled(false);
        ui->TableViewP_6->setModel(model);
        ui->TableViewP_6->resizeColumnsToContents();
    }
}


void MainWindow::on_pdf_6_clicked()
{
    Produit produit;
    QSqlQueryModel* model = produit.rapportStockSecurite();

    if (!model || model->rowCount() == 0) {
        QMessageBox::warning(this, "Aucun résultat", "Aucun produit en stock de sécurité.");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer le rapport PDF", "StockSecurite.pdf", "*.pdf");

    if (fileName.isEmpty())
        return;

    // 📄 PDF avec moins de marge en haut pour tout remonter
    QPdfWriter pdf(fileName);
    QPageLayout layout(QPageSize(QPageSize::A4), QPageLayout::Landscape, QMarginsF(40, 40, 40, 40));
    pdf.setPageLayout(layout);
    pdf.setResolution(300);

    QPainter painter(&pdf);
    QTextDocument doc;

    QString html;

    // 🔰 Logo à gauche en haut
    html += "<div style='text-align:left; margin-bottom:5px;'>";
    html += "<img src=':/images/logo.png' height='400' width='400'>";
    html += "</div>";

    // 🧾 Titre du rapport bien centré et en plus grand
    html += "<p style='font-size:65pt; font-weight:bold; color:#1B5E20; text-align:center; margin:10px;'>"
            "📋  produits à acheter </p>";

    // 🕓 Date alignée à droite
    html += "<p style='text-align:right; font-size:50pt; color:#000; margin-top:-10px;'>"
            "<b>Date :</b> " + QDateTime::currentDateTime().toString("dd/MM/yyyy") + "</p>";

    // 🗃️ Tableau allégé visuellement
    html += "<table border='5' cellspacing='50' cellpadding='50' "
            "style='font-size:40pt; width:100%; margin-top:30px;'>"
            "<thead><tr style='background-color:#f0f0f0;'>"
            "<th>Fournisseur</th><th>Produit</th><th>Catégorie</th><th>Quantité</th>"
            "<th>Date fabrication</th><th>Date expiration</th>"
            "</tr></thead><tbody>";

    for (int i = 0; i < model->rowCount(); ++i) {
        int quantite = model->data(model->index(i, 3)).toInt();

        if (quantite != 1)
            continue; // 🚫 Ignorer les produits dont la quantité n'est pas égale à 1

        html += "<tr style='color:red;'>"; // 🔴 toute la ligne en rouge

        for (int j = 0; j < model->columnCount(); ++j) {
            QString value;

            if (j == 4 || j == 5) { // Date fabrication ou expiration
                QDate date = model->data(model->index(i, j)).toDate();
                value = date.isValid() ? date.toString("dd-MM-yyyy") : "—";
            }  else {
                value = model->data(model->index(i, j)).toString();
            }

            html += "<td>" + value + "</td>";
        }

        html += "</tr>";
    }



    html += "</tbody></table>";

    doc.setHtml(html);
    doc.setPageSize(QSizeF(pdf.width(), pdf.height()));  // ✅ Échelle correcte
    doc.drawContents(&painter);
    painter.end();

    QMessageBox::information(this, "PDF généré", "Le rapport PDF a été généré avec succès !");
}




void MainWindow::on_TableViewP_6_clicked(const QModelIndex &index)
{
    QAbstractItemModel* model = ui->TableViewP_6->model();

    int row = index.row();



    QString nomP = model->data(model->index(row, 0)).toString();
    QString categorie = model->data(model->index(row, 1)).toString();
    QString quantite = model->data(model->index(row, 2)).toString();
    QString dateF = model->data(model->index(row, 3)).toString();
    QString dateE = model->data(model->index(row, 4)).toString();
    QString nomF = model->data(model->index(row, 5)).toString();



    // Using QrCodegen to create a QR code from Facture attributes
    QString text = "nom Produit: " + nomP + "\n"
                                            "categorie: " + categorie + "\n"
                                 "quantite: " + quantite + "\n"
                                "date de fabrucation: " + dateF + "\n"
                             "date d'expiration: " + dateE + "\n"
                             "nom furnisseur: " + nomF + "\n";

    using namespace qrcodegen;
    QrCode qr = QrCode::encodeText(text.toUtf8().data(), QrCode::Ecc::MEDIUM);

    qint32 sz = qr.getSize();
    QImage im(sz, sz, QImage::Format_RGB32);
    QRgb black = qRgb(0, 0, 0);
    QRgb white = qRgb(255, 255, 255);

    for (int y = 0; y < sz; y++) {
        for (int x = 0; x < sz; x++) {
            im.setPixel(x, y, qr.getModule(x, y) ? black : white);
        }
    }
    ui->qrcode_6->setPixmap(QPixmap::fromImage(im.scaled(200, 200, Qt::KeepAspectRatio, Qt::FastTransformation), Qt::MonoOnly));



    ui->Affichage_31->setCurrentIndex(3);


}

void MainWindow::on_telecharger_qr_code_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save QR Code"), "", tr("PNG Files (*.png);;All Files (*)"));

    if (fileName.isEmpty()) {
        return; // User cancelled the dialog
    }

    // Ensure the file name ends with .png
    if (!fileName.endsWith(".png", Qt::CaseInsensitive)) {
        fileName += ".png";
    }

    // Get the QR code image from the label
    QLabel *qrLabel = ui->qrcode_6; // Assuming your QLabel is named qrcode
    QPixmap pixmap = qrLabel->pixmap();

    // Check if the pixmap is valid
    if (!pixmap.isNull()) {
        // Define the border size
        int borderSize = 10; // Adjust as needed
        int newWidth = pixmap.width() + 2 * borderSize;
        int newHeight = pixmap.height() + 2 * borderSize;

        // Create a new image with a white background
        QImage borderedImage(newWidth, newHeight, QImage::Format_RGB32);
        borderedImage.fill(Qt::white); // Fill with white

        // Draw the original QR code onto the new image
        QPainter painter(&borderedImage);
        painter.drawPixmap(borderSize, borderSize, pixmap);
        painter.end();

        // Save the bordered image as a PNG file
        if (borderedImage.save(fileName, "PNG")) {
            QMessageBox::information(this, tr("Success"), tr("QR Code saved successfully!"));
        } else {
            QMessageBox::warning(this, tr("Error"), tr("Failed to save QR Code."));
        }
    } else {
        QMessageBox::warning(this, tr("Error"), tr("No QR Code to save."));
    }
}
void MainWindow::on_StatP_clicked()
{
    // 👉 Afficher l'onglet Statistiques
    ui->Affichage_31->setCurrentIndex(2);

    // 📦 Utiliser la méthode métier
    Produit produit;
    QMap<QString, int> tauxStock = produit.calculerTauxStock();

    // 🥧 Création de la série de données du graphique
    QPieSeries *series = new QPieSeries();

    // Calcul total des quantités
    int total = 0;
    for (const auto &value : tauxStock) {
        total += value;
    }

    // 🟣 Ajout dynamique selon les clés et calcul des pourcentages
    if (tauxStock.contains("Rupture")) {
        int rupture = tauxStock["Rupture"];
        double pourcentageRupture = (total != 0) ? (rupture / double(total)) * 100 : 0;
        series->append("🔴 Rupture: " + QString::number(pourcentageRupture, 'f', 2) + "%", rupture);
    }

    if (tauxStock.contains("StockSécurité")) {
        int stockSecurite = tauxStock["StockSécurité"];
        double pourcentageSecurite = (total != 0) ? (stockSecurite / double(total)) * 100 : 0;
        series->append("🟡 Stock de sécurité: " + QString::number(pourcentageSecurite, 'f', 2) + "%", stockSecurite);
    }

    if (tauxStock.contains("StockNormal")) {
        int stockNormal = tauxStock["StockNormal"];
        double pourcentageNormal = (total != 0) ? (stockNormal / double(total)) * 100 : 0;
        series->append("🟢 Stock suffisant: " + QString::number(pourcentageNormal, 'f', 2) + "%", stockNormal);
    }

    // 🎨 Appliquer les couleurs manuellement si les tranches existent
    for (auto slice : series->slices()) {
        if (slice->label().contains("Rupture"))
            slice->setBrush(Qt::red);
        else if (slice->label().contains("sécurité"))
            slice->setBrush(Qt::yellow);
        else
            slice->setBrush(Qt::green);
    }

    series->setLabelsVisible(true); // Afficher les labels

    // 🧩 Création du graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("📊 Analyse du stock (par quantité)");
    chart->legend()->setAlignment(Qt::AlignRight);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // 🔄 Nettoyer le layout avant d'ajouter le nouveau graphique
    QLayoutItem* child;
    while ((child = ui->layoutStatistiques_6->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
    chartView->setMinimumSize(800, 600);
    // ➕ Ajouter le nouveau graphique dans l'interface
    ui->layoutStatistiques_6->addWidget(chartView);
    QFont fontLabel;
    fontLabel.setPointSize(12);
    for (auto slice : series->slices()) {
        slice->setLabelFont(fontLabel);
    }

    // 🏷️ Police du titre
    QFont fontTitre;
    fontTitre.setPointSize(14);
    fontTitre.setBold(true);
    chart->setTitleFont(fontTitre);

    // 📜 Police de la légende
    QFont fontLegende;
    fontLegende.setPointSize(12);
    chart->legend()->setFont(fontLegende);
}
// eya
void MainWindow::on_pushButton_ajouter_v_clicked() {
    QString nom = ui->lineEdit_nom_2->text();
    QString type = ui->lineEdit_typev_2->currentText();
    QDate date_creation = ui->dateEdit_creation_2->date();
    QDate date_expiration = ui->dateEdit_expiration_2->date();
    QString statut = ui->comboBox_status_2->currentText();
    QString certification = ui->lineEdit_certification_2->currentText();

    qDebug() << "🔍 Avant modification, nomAModifier =" << nomAModifier;

    // ✅ Vérification des erreurs avant validation
    if (!modeModification_v && (!ui->labelErrorNom->text().isEmpty() ||
                              !ui->labelErrorType->text().isEmpty() ||
                              !ui->labelErrorCertification->text().isEmpty())) {
        QMessageBox::warning(this, "Erreur", "Veuillez corriger les erreurs avant de continuer !");
        return;
    }

    // 🔴 Vérification du statut
    if (statut != "effectuer" && statut != "en cour") {
        QMessageBox::warning(this, "Erreur", "Veuillez choisir un statut valide !");
        return;
    }

    // Vérifier que la date d'expiration est après la date de création
    if (date_expiration <= date_creation) {
        QMessageBox::warning(this, "Date invalide", "La date d'expiration doit être après la date de création !");
        return;
    }

    Vaccin v(0, nom, type, date_creation, date_expiration, statut, certification);

    if (modeModification_v) {
        if (nomAModifier.isEmpty()) {
            qDebug() << "❌ ERREUR: nomAModifier est vide lors de la modification !";
        }
        // 🔹 Mode Modification : Mise à jour du vaccin
        if (v.modifier(nom)) {
            QMessageBox::information(this, "Succès", "Vaccin modifié avec succès !");
            modeModification_v = false;  // Désactiver le mode modification
            connect(ui->lineEdit_nom_2, &QLineEdit::textChanged, this, &MainWindow::verifierNom);
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la modification !");
            return;
        }
    } else {
        // 🔹 Mode Ajout : Ajout d'un nouveau vaccin
        qDebug() << "Mode Ajout - Nouveau vaccin";

        if (v.ajouter()) {
            QMessageBox::information(this, "Succès", "Vaccin ajouté avec succès !");
            QMetaObject::invokeMethod(this, [=]() {
                ui->stackedWidget->setCurrentIndex(3);
                ui->tab_5->setCurrentIndex(1); // 1 correspond à l'onglet "Ajout"
                QApplication::processEvents();
            }, Qt::QueuedConnection);
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de l'ajout !");
            return;
        }
    }

    // ✅ Réinitialisation des `QComboBox` sans les vider complètement
    ui->lineEdit_typev_2->setCurrentIndex(0);
    ui->comboBox_status_2->setCurrentIndex(0);
    ui->lineEdit_certification_2->setCurrentIndex(0);

    // ✅ Remettre les options de la `QComboBox` (si elles sont vides après modification)
    if (ui->lineEdit_typev_2->count() == 0) {
        ui->lineEdit_typev_2->addItems({"ARNm", "Viral", "Inactivé", "Sous-unité"});
    }

    if (ui->lineEdit_certification_2->count() == 0) {
        ui->lineEdit_certification_2->addItems({"OMS", "EMA", "FDA", "Aucune"});
    }

    // ✅ Mise à jour de l'affichage
    ui->tableView->setModel(v.afficher());
    ui->lineEdit_nom_2->setDisabled(false);
    ui->lineEdit_nom_2->clear();
    ui->dateEdit_creation_2->setDate(QDate::currentDate());
    ui->dateEdit_expiration_2->setDate(QDate::currentDate());
}


void MainWindow::on_pushButton_suppv_clicked() {
    QString nomv = ui->lineEdit->text().trimmed();

    if (nomv.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un NOM valide !");
        return;
    }

    // 🔍 Vérifier si le vaccin existe avant de tenter de le supprimer
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM VACCIN WHERE NOM = :nom");
    query.bindValue(":nom", nomv);

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL lors de la vérification de l'existence du vaccin :" << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Erreur lors de la vérification de l'existence du vaccin !");
        return;
    }

    query.next();
    int count = query.value(0).toInt();

    if (count == 0) {
        QMessageBox::warning(this, "Nom inexistant", "⚠️ Le vaccin avec ce nom n'existe pas !");
        return;
    }

    // 🗑️ Demander confirmation avec "Oui" et "Non"
    QMessageBox confirmationBox;
    confirmationBox.setWindowTitle("Confirmation");
    confirmationBox.setText("Voulez-vous vraiment supprimer ce vaccin ?");
    QPushButton *ouiButton = confirmationBox.addButton("Oui", QMessageBox::YesRole);
    QPushButton *nonButton = confirmationBox.addButton("Non", QMessageBox::NoRole);
    confirmationBox.exec();

    if (confirmationBox.clickedButton() == ouiButton) {
        Vaccin v;
        if (v.supprimer(nomv)) {  // ✅ Suppression si le vaccin existe
            QMessageBox::information(this, "Succès", "Vaccin supprimé avec succès !");
            ui->tableView->setModel(v.afficher());  // 🔄 Mettre à jour l'affichage
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression du vaccin !");
        }
    }
}

void MainWindow::on_pushButton_modifier_2_clicked() {
    QString nomv = ui->lineEdit->text().trimmed();

    if (nomv.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un NOM valide !");
        return;
    }

    // 🔍 Vérifier si le vaccin existe avant de permettre la modification
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM VACCIN WHERE NOM = :nom");
    query.bindValue(":nom", nomv);

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL lors de la vérification de l'existence du vaccin :" << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Erreur lors de la vérification du vaccin !");
        return;
    }

    query.next();
    int count = query.value(0).toInt();

    if (count == 0) {
        QMessageBox::warning(this, "Nom inexistant", "⚠️ Le vaccin avec ce nom n'existe pas !");
        return;
    }

    // 📌 Si le vaccin existe, on remplit les champs pour modification
    qDebug() << "📢 Tentative de modification pour le vaccin :" << nomv;
    remplirChampsModification(nomv);

    // 🔄 Redirection vers la page d'ajout/modification
    ui->stackedWidget->setCurrentIndex(3);
}





void MainWindow::remplirChampsModification(QString nomv) {
    QString nom, type, statut, certification;
    QDate date_creation, date_expiration;

    qDebug() << "🔍 Avant récupération, nomAModifier =" << nomAModifier;
    Vaccin v;
    if (v.remplirChampsModification(nomv, nom, type, date_creation, date_expiration, statut, certification)) {
        disconnect(ui->lineEdit_nom_2, &QLineEdit::textChanged, this, &MainWindow::verifierNom);

        // Remplir les champs
        ui->lineEdit_nom_2->setText(nom);
        if (ui->lineEdit_typev_2->findText(type) != -1)
            ui->lineEdit_typev_2->setCurrentText(type);
        else
            ui->lineEdit_typev_2->addItem(type);
        ui->dateEdit_creation_2->setDate(date_creation);
        ui->dateEdit_expiration_2->setDate(date_expiration);
        ui->comboBox_status_2->setCurrentText(statut);
        if (ui->lineEdit_certification_2->findText(certification) != -1)
            ui->lineEdit_certification_2->setCurrentText(certification);
        else
            ui->lineEdit_certification_2->addItem(certification);
        // ui->lineEdit_nom_2->setDisabled(true);
        ancienType = type;
        ancienneDateCreation = date_creation;
        ancienneDateExpiration = date_expiration;
        ancienStatut = statut;
        ancienneCertification = certification;

        nomAModifier = nom;
        modeModification_v = true;

        qDebug() << "✅ Après récupération, nomAModifier =" << nomAModifier;
        qDebug() << "Type récupéré :" << type;
        qDebug() << "Certification récupérée :" << certification;

        // ✅ Affichage du message d'information
        QMessageBox::information(this, "Modification", "Données chargées, vous pouvez modifier !");

        // ✅ Redirection forcée avec QMetaObject::invokeMethod
        QMetaObject::invokeMethod(this, [=]() {
            qDebug() << "🔄 Forçage de la redirection vers la page d'ajout";
            ui->stackedWidget->setCurrentIndex(3);
            ui->tab_5->setCurrentIndex(0); // 0 correspond à l'onglet "Ajout"

            QApplication::processEvents(); // ✅ Force l'UI à traiter les événements
            qDebug() << "📌 Vérification après redirection : Page active =" << ui->stackedWidget->currentIndex();

        }, Qt::QueuedConnection);


    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de charger les données du vaccin !");
    }
}

void MainWindow::on_lineEdit_recherche_2_textChanged(const QString &arg1)
{
    qDebug() << "Recherche en cours... Texte saisi :" << arg1;

    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    QString searchText = arg1.trimmed();

    QString baseQuery = R"(
        SELECT NOM, TYPE, DATE_CREATION, DATE_EXPIRATION, STATUT, CERTIFICATION_VACCIN
        FROM VACCIN
    )";

    if (!searchText.isEmpty()) {
        baseQuery += " WHERE LOWER(NOM) LIKE LOWER(:val) "
                     "OR LOWER(TYPE) LIKE LOWER(:val) "
                     "OR LOWER(CERTIFICATION_VACCIN) LIKE LOWER(:val)";
        query.prepare(baseQuery);
        query.bindValue(":val", "%" + searchText + "%");
    } else {
        query.prepare(baseQuery);
    }

    if (!query.exec()) {
        qDebug() << "Erreur SQL :" << query.lastError().text();
        return;
    }

    model->setQuery(std::move(query));
    ui->tableView_2->setModel(model);
}

void MainWindow::verifierVaccinsExpires() {
    QSqlQuery query;
    query.prepare("SELECT NOM, DATE_EXPIRATION FROM SMARTVACC.VACCIN WHERE DATE_EXPIRATION < SYSDATE");

    if (!query.exec()) {
        qDebug() << "❌ Erreur lors de la vérification des vaccins expirés :" << query.lastError().text();
        return;
    }

    QString message = "Les vaccins suivants sont expirés :\n";
    bool hasExpired = false;

    while (query.next()) {
        QString nom = query.value(0).toString();
        QDate date_expiration = query.value(1).toDate();
        message += "📌 " + nom + " - Expiré le : " + date_expiration.toString("dd/MM/yyyy") + "\n";
        hasExpired = true;
    }

    if (hasExpired) {
        // 🔹 Créer une notification système
        QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
        // Définir une icône valide pour la barre des tâches
        trayIcon->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            // Assurez-vous que le fichier existe !
            // Remplacez par votre icône
        trayIcon->show();

        // 🔹 Envoyer la notification
        trayIcon->showMessage("⚠️ Vaccins Expirés", message, QSystemTrayIcon::Warning, 100000);

        qDebug() << "🔔 Notification envoyée pour vaccins expirés.";
    } else {
        qDebug() << "✅ Aucun vaccin expiré.";
    }
}

void MainWindow::updateCovidStats(QString country, int confirmed, int deaths, int recovered, int active)
{
    QString message = QString(
                          "<div style='background-color: #ffffff; padding: 15px; border-radius: 10px; border: 2px solid #B22222; font-size: 14px; color: #333;'>"
                          "<h3 style='color:#B22222;'>📊 Données COVID-19 pour %1</h3>"
                          "<p><b>✅ Cas confirmés :</b> <span style='color: #008000;'>%2</span></p>"
                          "<p><b>❌ Décès :</b> <span style='color: #FF0000;'>%3</span></p>"
                          "<p><b>💪 Guérisons :</b> <span style='color: #008000;'>%4</span></p>"
                          "<p><b>🔥 Cas actifs :</b> <span style='color: #B22222;'>%5</span></p>"
                          "</div>")
                          .arg(country)
                          .arg(confirmed)
                          .arg(deaths)
                          .arg(recovered)
                          .arg(active);

    ui->predictionLabel->setTextFormat(Qt::RichText);
    ui->predictionLabel->setText(message);
    ui->tab_5->setCurrentIndex(3);
}

void MainWindow::showError(QString error)
{
    QMessageBox::warning(this, "Erreur API", "Impossible de récupérer les données : " + error);
}
void MainWindow::updatePrediction(const QString &predictionResult) {


    ui->tab_5->setCurrentIndex(3);  // Sélectionne l'onglet "Prediction"
    ui->predictionLabel->setText(predictionResult);  // ✅ Met à jour le texte dans un QLabel

}

void MainWindow::lancerPrediction()
{
    QString paysChoisi = ui->comboBoxPays->currentText(); // Nom visible dans la combo
    qDebug() << "📡 Lancement de la prédiction pour : " << paysChoisi;

    if (paysChoisi.isEmpty()) {
        QMessageBox::warning(this, "⚠ Sélectionner un pays", "Veuillez choisir un pays avant de lancer la prédiction.");
        return;
    }

    if (!medApi) {
        qDebug() << "❌ ERREUR : medApi est NULL, impossible de récupérer les données.";
        return;
    }

    // Nouvelle map avec codes ISO 3 lettres
    QMap<QString, QString> countryISOMap = {
        {"Tunisie", "TUN"}, {"France", "FRA"}, {"Italie", "ITA"},
        {"Espagne", "ESP"}, {"Allemagne", "DEU"}, {"États-Unis", "USA"},
        {"Canada", "CAN"}, {"Maroc", "MAR"}, {"Algérie", "DZA"},
        {"Chine", "CHN"}
    };

    QString isoCode = countryISOMap.value(paysChoisi, "");

    if (isoCode.isEmpty()) {
        QMessageBox::warning(this, "🌍 Pays non supporté", "Aucun code ISO trouvé pour ce pays.");
        return;
    }

    qDebug() << "📩 Envoi de la requête à l'API avec code ISO : " << isoCode;

    ui->predictionLabel->setTextFormat(Qt::RichText);
    ui->predictionLabel->setText(
        "<div style='text-align:center; color:#444; font-size:16px;'>"
        "⏳ <b>Récupération des données en cours...</b><br>"
        "<img src=':/loader.gif' width='64' height='64'/>"
        "</div>"
        );
    ui->tab_5->setCurrentIndex(3); // aller à l’onglet prédiction


    medApi->fetchCovidData(isoCode); // Appel avec le bon code
}


void MainWindow::verifierNom() {
    QString nom = ui->lineEdit_nom_2->text().trimmed();

    qDebug() << "🔍 Vérification du nom :" << nom;
    qDebug() << "📌 Valeur actuelle de nomAModifier :" << nomAModifier;

    // ✅ Désactiver la vérification si on est en mode modification
    /* if (modeModification) {
        qDebug() << "✅ Mode modification actif, validation du nom ignorée.";
        ui->labelErrorNom->clear();
        ui->labelErrorNom->setStyleSheet("color: transparent; background-color: transparent; border: none;");
        return;  // ⛔ Quitter immédiatement la fonction
    }*/

    if (nom.isEmpty()) {
        ui->labelErrorNom->setText("⚠ Le nom ne peut pas être vide !");
        ui->labelErrorNom->setStyleSheet("color: red; background-color: transparent; border: none;");
    }
    else if (nom.length() < 3) {
        ui->labelErrorNom->setText("⚠ Le nom doit contenir au moins 3 caractères !");
        ui->labelErrorNom->setStyleSheet("color: red; background-color: transparent; border: none;");
    }
    else if (nomExisteDeja(nom)) {
        ui->labelErrorNom->setText("⚠ Ce nom de vaccin existe déjà !");
        ui->labelErrorNom->setStyleSheet("color: red; background-color: transparent; border: none;");
    }
    else {
        ui->labelErrorNom->clear();
        ui->labelErrorNom->setStyleSheet("color: transparent; background-color: transparent; border: none;");
    }
}





bool MainWindow::nomExisteDeja(const QString &nom) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM SMARTVACC.VACCIN WHERE LOWER(NOM) = LOWER(:nom)");
    query.bindValue(":nom", nom);

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL lors de la vérification du nom :" << query.lastError().text();
        return false; // En cas d'erreur SQL, on ne bloque pas
    }

    query.next();
    int count = query.value(0).toInt();
    return (count > 0);
}

void MainWindow::on_pushButton_annuler_2_clicked()
{
    if (modeModification) {
        qDebug() << "🔄 Annulation en mode modification - Restauration des anciennes valeurs.";

        // 🔹 Remettre les anciennes valeurs
        ui->lineEdit_nom_2->setText(nomAModifier);
        ui->lineEdit_typev_2->setCurrentText(ancienType);
        ui->dateEdit_creation_2->setDate(ancienneDateCreation);
        ui->dateEdit_expiration_2->setDate(ancienneDateExpiration);
        ui->comboBox_status_2->setCurrentText(ancienStatut);
        ui->lineEdit_certification_2->setCurrentText(ancienneCertification);

        QMessageBox::information(this, "Annulation", "Les valeurs avant modification ont été restaurées.");
    } else {
        qDebug() << "🧹 Annulation en mode ajout - Effacement des champs.";

        // 🔹 Vider tous les champs
        ui->lineEdit_nom_2->clear();
        ui->lineEdit_typev_2->setCurrentIndex(0);
        ui->comboBox_status_2->setCurrentIndex(0);
        ui->dateEdit_creation_2->setDate(QDate::currentDate());
        ui->dateEdit_expiration_2->setDate(QDate::currentDate());
        ui->lineEdit_certification_2->setCurrentIndex(0);

        QMessageBox::information(this, "Annulation", "Les champs ont été effacés.");
    }
}






void MainWindow::on_pushButton_pdf_2_clicked() {
    QString nomVaccin = ui->lineEdit->text().trimmed();

    if (nomVaccin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un NOM valide pour le vaccin !");
        return;
    }

    // 🔍 Rechercher les détails du vaccin dans la base de données
    QSqlQuery query;
    query.prepare("SELECT NOM, TYPE, DATE_CREATION, DATE_EXPIRATION, CERTIFICATION_VACCIN, STATUT "
                  "FROM SMARTVACC.VACCIN WHERE NOM = :nom");
    query.bindValue(":nom", nomVaccin);

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL lors de la récupération du vaccin :" << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer les informations du vaccin !");
        return;
    }

    if (!query.next()) {
        QMessageBox::warning(this, "Erreur", "Le vaccin avec ce nom n'existe pas !");
        return;
    }

    // ✅ Récupérer les informations
    QString type = query.value("TYPE").toString();
    QDate dateCreation = query.value("DATE_CREATION").toDate();
    QDate dateExpiration = query.value("DATE_EXPIRATION").toDate();
    QString certification = query.value("CERTIFICATION_VACCIN").toString();
    QString statut = query.value("STATUT").toString();

    // 🏥 Définition du statut du vaccin
    QString etatVaccin;
    QString colorVaccin;
    if (dateExpiration < QDate::currentDate()) {
        etatVaccin = "⚠ Expiré ❌";
        colorVaccin = "red";
    } else if (dateExpiration <= QDate::currentDate().addMonths(3)) {
        etatVaccin = "⚠ Expiration Proche";
        colorVaccin = "orange";
    } else {
        etatVaccin = "✅ Valide";
        colorVaccin = "green";
    }

    // 📌 Définition du statut de la certification
    QString etatCertification = certification.isEmpty() ? "Non certifié ❌" : "Certifié ✅";
    QString colorCertification = certification.isEmpty() ? "red" : "green";

    // 📂 Demander où enregistrer le fichier PDF
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer le fichier PDF",
                                                    nomVaccin + "_certification.pdf",
                                                    "Fichiers PDF (*.pdf)");

    if (filePath.isEmpty()) {
        return; // L'utilisateur a annulé
    }

    // 🖨️ Configuration de l'impression vers PDF
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setPageSize(QPageSize(QPageSize::A4));

    // 📝 Création du document PDF
    QTextDocument document;
    QString html = QString(
                       "<h1 style='color:#B22222; text-align:center;'>🩺 Rapport de Certification du Vaccin</h1>"
                       "<hr style='border:1px solid #B22222;'>"
                       "<h2 style='color:#333;'>📌 Détails du Vaccin</h2>"
                       "<p><b>🆔 Nom :</b> %1</p>"
                       "<p><b>🧬 Type :</b> %2</p>"
                       "<p><b>📅 Date de Création :</b> %3</p>"
                       "<p><b>⏳ Date d'Expiration :</b> %4</p>"
                       "<p><b>🩹 Statut :</b> %5</p>"
                       "<h2 style='color:#333;'>🏥 Certification</h2>"
                       "<p><b>📜 Nom du Certificat :</b> %6</p>"
                       "<p style='color:%7;'><b>🔍 État de la Certification :</b> %8</p>"
                       "<h2 style='color:#333;'>🔎 Analyse de l'État du Vaccin</h2>"
                       "<p style='color:%9;'><b>⚠ Statut :</b> %10</p>"
                       "<h2 style='color:#333;'>📊 Recommandations et Avis Médical</h2>"
                       "<p>🔵 Ce vaccin est recommandé pour une utilisation jusqu'à sa date d'expiration.</p>"
                       "<p>⚠ Si le vaccin est expiré, ne pas utiliser et éliminer selon les règles sanitaires.</p>"
                       "<p>🩺 Consultez un professionnel de santé pour toute question sur ce vaccin.</p>"
                       "<hr style='border:1px solid #B22222;'>"
                       "<p style='text-align:center;'>📅 Rapport généré le %11</p>"
                       ).arg(nomVaccin)
                       .arg(type)
                       .arg(dateCreation.toString("dd/MM/yyyy"))
                       .arg(dateExpiration.toString("dd/MM/yyyy"))
                       .arg(statut)
                       .arg(certification.isEmpty() ? "Non disponible" : certification)
                       .arg(colorCertification)
                       .arg(etatCertification)
                       .arg(colorVaccin)
                       .arg(etatVaccin)
                       .arg(QDate::currentDate().toString("dd/MM/yyyy"));

    document.setHtml(html);
    document.print(&printer);

    // 🔔 Message de succès
    QMessageBox::information(this, "Succès", "Le rapport de certification a été généré avec succès !");
}

void MainWindow::on_pushButtonStat_18_clicked()
{
    ui->tab_5->setCurrentIndex(2);

    if (!ui->Statistique_8) {
        qDebug() << "Erreur : L'onglet Statistique_8 n'existe pas";
        return;
    }

    // 🔁 Supprimer ancien layout proprement
    QLayout *oldLayout = ui->Statistique_8->layout();
    if (oldLayout) {
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            if (item->widget()) item->widget()->deleteLater();
            delete item;
        }
        delete oldLayout;
        ui->Statistique_8->setLayout(nullptr);
    }

    // 📦 Requête Oracle (groupement par année)
    QSqlQuery query;
    if (!query.exec("SELECT TO_CHAR(date_creation, 'YYYY') AS annee, COUNT(*) FROM Vaccin GROUP BY TO_CHAR(date_creation, 'YYYY') ORDER BY annee")) {
        qDebug() << "Erreur SQL :" << query.lastError().text();
        return;
    }

    // 📊 Préparer données
    QBarSet *set0 = new QBarSet("Vaccinations");
    set0->setColor(QColor("#2e86c1"));  // 🔹 Couleur personnalisée
    QStringList categories;
    QList<int> valeurs;
    int totalVaccinations = 0;

    while (query.next()) {
        QString annee = query.value(0).toString();
        int nb = query.value(1).toInt();
        categories << annee;
        *set0 << nb;
        valeurs << nb;
        totalVaccinations += nb;
        qDebug() << "Année:" << annee << "Nombre:" << nb;
    }

    if (categories.isEmpty()) {
        qDebug() << "Aucune donnée dans la base pour les statistiques.";
        return;
    }

    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->setLabelsVisible(true);  // 🔢 Afficher les valeurs au-dessus des barres

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("📊 Statistiques de nombre des vaccins par année");
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignTop);

    // 🧭 Axe X
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // 📈 Axe Y
    int maxValue = *std::max_element(valeurs.begin(), valeurs.end()) + 5;
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, maxValue);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // 👁 Vue graphique
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(700, 50);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartView->setContentsMargins(0, 0, 0, 0);


    // 🧱 Encadré dans un QGroupBox
    QGroupBox *graphBox = new QGroupBox("Vue Globale des vaccinations");
    graphBox->setStyleSheet(R"(
    QGroupBox {
        font-weight: bold;
        border: 2px solid #ccc;
        border-radius: 10px;
        padding: 20px;
        margin: 20px auto;
        background-color: #ffffff;
    }
)");





    QVBoxLayout *boxLayout = new QVBoxLayout();
    boxLayout->addWidget(chartView);
    graphBox->setLayout(boxLayout);

    // 💬 Résumé texte
    QLabel *summary = new QLabel("📅 Total années : " + QString::number(categories.size()) +
                                 " — 💉 Total vaccinations : " + QString::number(totalVaccinations));
    summary->setAlignment(Qt::AlignCenter);
    summary->setStyleSheet("font-style: italic; color: #2e86c1;");

    // 📐 Layout final
    QVBoxLayout *layout = new QVBoxLayout();

    QLabel *titleLabel = new QLabel("📊  Statistiques de nombre des vaccins par année");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #800000;");
    layout->addWidget(titleLabel);

    layout->addWidget(graphBox);
    layout->addWidget(summary);

    layout->setAlignment(graphBox, Qt::AlignHCenter);
    layout->setAlignment(summary, Qt::AlignHCenter);


    ui->Statistique_8->setLayout(layout);
    ui->Statistique_8->update();
    ui->Statistique_8->repaint();

    graphBox->setMinimumWidth(1300);


}







void MainWindow::on_comboBox_tri_2_currentIndexChanged(int index)
{
    qDebug() << "Tri demandé par :" << index;

    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    QString orderColumn;

    if (index == 0)
        orderColumn = "DATE_CREATION";
    else if (index == 1)
        orderColumn = "DATE_EXPIRATION";
    else if (index == 2)
        orderColumn = "STATUT";
    else
        orderColumn = "DATE_CREATION"; // Valeur par défaut

    // ⚠️ On exclut l’ID dans les colonnes affichées
    QString queryString = QString(
                              "SELECT NOM, TYPE, DATE_CREATION, DATE_EXPIRATION, STATUT, CERTIFICATION_VACCIN "
                              "FROM VACCIN ORDER BY %1 ASC").arg(orderColumn);

    if (!query.prepare(queryString)) {
        qDebug() << "Erreur préparation requête :" << query.lastError().text();
        return;
    }

    if (!query.exec()) {
        qDebug() << "Erreur exécution requête :" << query.lastError().text();
        return;
    }

    model->setQuery(std::move(query));
    ui->tableView_2->setModel(model);
}





void MainWindow::on_pushButton_afficherRDV_clicked()
{
    QSqlQuery query;

    // ✅ Requête complète : NOM + PRENOM pour les RDV du jour
    query.prepare("SELECT NOM, PRENOM FROM CARNETS WHERE TRUNC(DATE_RDV) = TRUNC(SYSDATE)");

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL :" << query.lastError().text();
        return;
    }

    bool found = false;

    while (query.next()) {
        QString nom = query.value(0).toString().trimmed();
        QString prenom = query.value(1).toString().trimmed();

        QString fullName = prenom + " " + nom;

        qDebug() << "✅ Nom à envoyer :" << fullName;

        A.write_to_arduino(fullName.toUtf8() + "\n");

        QThread::sleep(2); // Pause pour bien voir chaque nom
        found = true;
    }

    if (!found) {
        qDebug() << "⚠️ Aucun RDV aujourd'hui.";
        A.write_to_arduino("Aucun RDV\n");
    } else {
        QThread::sleep(2); // ✅ Permet d'afficher le DERNIER nom avant la fin
    }
}
/*void MainWindow::lireDonneesSerie() {
    QByteArray data = A.read_from_arduino();
    bufferSerie += QString::fromUtf8(data); // concatène les morceaux

    // Tant qu'on a des lignes complètes
    while (bufferSerie.contains('\n')) {
        int pos = bufferSerie.indexOf('\n');
        QString ligne = bufferSerie.left(pos).trimmed();  // extrait la ligne complète
        bufferSerie.remove(0, pos + 1);  // supprime la ligne du buffer

        qDebug() << "Ligne complète reçue : " << ligne;

        if (ligne == "ALERTE_COURANT=0") {
            QSqlQuery query;
            if (query.exec("SELECT NOM_PRODUIT, QUANTITE FROM PRODUITS WHERE CATEGORIE = 'Vaccin'")) {
                QString message = "⚠️ Coupure de courant détectée !\n\nLes produits suivants nécessitent une réfrigération :\n\n";

                while (query.next()) {
                    QString nom = query.value(0).toString();
                    QString quantite = query.value(1).toString();
                    message += "Produit : " + nom + " - Quantité : " + quantite + "\n";
                }

                QMessageBox::critical(this, "Alerte Produits Sensibles", message);
                QString message2=produitTmp.MessageAlerte();
                email.sendEmail("najoua.dahmen18@gmail.com", "Alerte coupure courant ",message2);
            } else {
                QMessageBox::warning(this, "Erreur SQL", "Échec lors de la requête SQL !");
            }
        }
    }
}
*/
void MainWindow::lireDonneesSerie() {
    QByteArray data = A.read_from_arduino();
    bufferSerie += QString::fromUtf8(data); // Accumule les données série

    // Tant qu'on a une ligne complète
    while (bufferSerie.contains('\n') || bufferSerie.contains('#')) {
        int pos = bufferSerie.indexOf('\n');
        int posSharp = bufferSerie.indexOf('#');

        if (posSharp != -1 && (posSharp < pos || pos == -1)) {
            // 🎯 Traitement du clavier RFID
            QString code = bufferSerie.left(posSharp).trimmed();
            bufferSerie.remove(0, posSharp + 1);

            qDebug() << "✅ Code clavier reçu : " << code;

            // Requête SQL avec le code RFID
            QSqlQuery query;
            query.prepare(R"(
                SELECT E.NOM, E.POSTE, CP.NOM_COMPAGNE
                FROM EMPLOYES E
                JOIN CONTRIBUER C ON E.ID_EMPLOYE = C.ID_EMPLOYE
                JOIN COMPAGNE CP ON CP.ID_COMPAGNE = C.ID_COMPAGNE
                WHERE E.RFID_ID = :code
            )");
            query.bindValue(":code", code);

            if (query.exec() && query.next()) {
                QString nom = query.value("NOM").toString();
                QString poste = query.value("POSTE").toString();
                QString nomCompagne = query.value("NOM_COMPAGNE").toString();

                QString nomAffiche = nom.toUpper();
                QString message = QString(
                    "<div style='font-size:15px;'>"
                    "<p><b style='font-size:18px;'>🎉 BIENVENUE %1</b></p>"
                    "<p><b>Poste :</b> %2</p>"
                    "<p><b>Compagne :</b> %3</p>"
                    "<hr>"
                    "<p style='color:green;'>✅ Bonne mission !<br>✊ Merci d’être là pour sauver des vies.</p>"
                    "</div>"
                ).arg(nomAffiche).arg(poste).arg(nomCompagne);

                QMessageBox msgBox;
                msgBox.setWindowTitle("✔️ Let's save lives");
                msgBox.setTextFormat(Qt::RichText);
                msgBox.setText(message);
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setButtonText(QMessageBox::Ok, "Let's save lives");
                msgBox.setStyleSheet(
                    "QLabel { min-width: 300px; font-family: Arial; font-size: 14px; } "
                    "QPushButton { background-color: darkgreen; color: white; font-weight: bold; padding: 8px 18px; border-radius: 8px; } "
                    "QPushButton:hover { background-color: green; }"
                );
                msgBox.exec();
            } else {
                QMessageBox::warning(this, "❌ Accès refusé", "Code invalide ou employé non affecté à une compagne !");
            }
        }
        else if (pos != -1) {
            // 🎯 Traitement du capteur de courant
            QString ligne = bufferSerie.left(pos).trimmed();
            bufferSerie.remove(0, pos + 1);

            qDebug() << "🔌 Ligne capteur reçue : " << ligne;

            if (ligne == "ALERTE_COURANT=0") {
                QSqlQuery query;
                if (query.exec("SELECT NOM_PRODUIT, QUANTITE FROM PRODUITS WHERE CATEGORIE = 'Vaccin'")) {
                    QString message = "⚠️ Coupure de courant détectée !\n\nLes produits suivants nécessitent une réfrigération :\n\n";

                    while (query.next()) {
                        QString nom = query.value(0).toString();
                        QString quantite = query.value(1).toString();
                        message += "Produit : " + nom + " - Quantité : " + quantite + "\n";
                    }

                    QMessageBox::critical(this, "Alerte Produits Sensibles", message);
                    QString message2 = produitTmp.MessageAlerte();
                    email.sendEmail("najoua.dahmen18@gmail.com", "Alerte coupure courant", message2);
                } else {
                    QMessageBox::warning(this, "Erreur SQL", "Échec lors de la requête SQL !");
                }
            }
        } else {
            break;
        }
    }
}



