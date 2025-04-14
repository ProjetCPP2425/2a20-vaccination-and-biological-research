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





// Constructeur de MainWindow
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   // ui->frame_chatbox->hide(); // caché au départ
    //ui->frame_chatbox->hide();  // ✅ Pas setVisible(false), mais bien hide()
    connect(ui->button_chatbot_icon, &QPushButton::clicked, this, []() {
        qDebug() << "Icône chatbot cliquée !";
    });

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
        ui->tableView->setModel(e.afficher());
        chargerNomsDansComboBoxPDF();

        // Ajustements d'affichage
        ui->tableView->verticalHeader()->setDefaultSectionSize(35);
        ui->tableView->horizontalHeader()->setDefaultSectionSize(150);
        ui->tableView->resizeColumnsToContents();
        ui->tableView->setStyleSheet("QTableView::item { padding: 10px; }");
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

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


    ui->button_chatbot_icon->setGraphicsEffect(nullptr); // assure aucun effet parasite*/

    manager = new QNetworkAccessManager(this);
    connect(ui->button_chatbot_icon, &QPushButton::clicked, this, &MainWindow::on_button_chatbot_icon_clicked);
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



}

MainWindow::~MainWindow()
{
    delete ui;
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

        QString poste = e.getPosteFromCIN(cin).toLower();

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

    QString html = "<html><head><meta charset='utf-8'><title>Planning</title>"
                   "<style>"
                   "body { font-family: 'Segoe UI', sans-serif; background-color: #f4f7fa; padding: 30px; }"
                   "h2 { text-align: center; color: #2c3e50; font-size: 26px; }"
                   "table { width: 100%; border-collapse: collapse; margin-top: 20px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }"
                   "th { background-color: #2980b9; color: white; padding: 12px; font-size: 15px; }"
                   "td { padding: 10px; text-align: center; border-bottom: 1px solid #ccc; font-size: 14px; }"
                   "tr:nth-child(even) { background-color: #ecf0f1; }"
                   ".green { color: green; font-weight: bold; }"
                   ".red { color: red; font-weight: bold; }"
                   ".summary { margin-top: 20px; font-weight: bold; background: #ecf0f1; padding: 15px; border-radius: 8px; }"
                   ".actions { text-align: center; margin-top: 20px; }"
                   "button { padding: 10px 20px; font-size: 14px; border: none; background-color: #3498db; color: white; border-radius: 5px; cursor: pointer; }"
                   "button:hover { background-color: #2980b9; }"
                   "</style></head><body>";

    html += "<h2>📅 Planning Hebdomadaire des Employés</h2>";
    html += "<p style='text-align:center; font-size:16px;'><strong>Période : </strong>" +
            dateDebut.toString("dd/MM/yyyy") + " ➡️ " + dateFin.toString("dd/MM/yyyy") + "</p>";

    // Tableau des jours
    html += "<table border='1'><tr><th>CIN</th><th>Nom</th><th>Poste</th><th>Disponibilité</th><th>Absence</th>";
    QStringList jours;
    for (QDate d = dateDebut; d <= dateFin; d = d.addDays(1)) {
        jours << d.toString("dd/MM");
        html += "<th>" + d.toString("ddd<br>dd/MM") + "</th>";
    }
    html += "</tr>";

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
        if (horaire.isEmpty()) horaire = horairesParPoste.value(poste, "Non défini");

        if (disponibilite == "Oui") dispo++;

        html += "<tr>";
        html += "<td>" + cin + "</td>";
        html += "<td>" + nom + " " + prenom + "</td>";
        html += "<td>" + poste + "</td>";
        html += QString("<td class='%1'>%2</td>").arg((disponibilite == "Oui" ? "green" : "red"), disponibilite);

        html += "<td>" + absence + "</td>";
        for (int i = 0; i < jours.size(); ++i)
            html += "<td>" + horaire + "</td>";
        html += "</tr>";
    }

    double taux = total > 0 ? (double)dispo / total * 100 : 0.0;

    html += "</table>";
    html += "<div class='summary'>Total d'employés : " + QString::number(total) +
            " | Disponibles : " + QString::number(dispo) +
            " | Taux de disponibilité : " + QString::number(taux, 'f', 1) + "%</div>";

    html += "<div class='actions'><button onclick='window.print()'>🖨️ Imprimer / Exporter en PDF</button></div>";
    html += "</body></html>";

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << html;
        file.close();
        QMessageBox::information(this, "Succès", "Planning exporté avec succès !");
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible d'écrire le fichier.");
    }
}







void MainWindow::afficherStatistiquesEmployes()
{
    int total = 0, dispo = 0, indispo = 0;
    QMap<QString, int> absentsParPoste;
    QMap<QString, int> totalParPoste;
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

    QString statistiques = "📊 <b>Total :</b> " + QString::number(total) +
                           "<br>✅ <b>Disponibles :</b> " + QString::number(dispo) +
                           "<br>❌ <b>Absents :</b> " + QString::number(indispo);

    if (taux > 50.0)
        statistiques += "<br><br><span style='color:red; font-weight:bold;'>🚨 Plus de 50% des employés sont absents !</span>";

    for (auto it = absentsParPoste.begin(); it != absentsParPoste.end(); ++it) {
        double tauxPoste = (double)it.value() / totalParPoste[it.key()] * 100;
        if (tauxPoste > 50.0) {
            statistiques += "<br><span style='color:orange;'>⚠️ " + it.key() + " : " + QString::number(tauxPoste, 'f', 1) + "% absents</span>";
        }
    }

    statistiques += "<br><br>📈 <b>Taux d’absentéisme :</b> <span style='color:" + QString(taux > 50 ? "red" : "green") + ";'>" + QString::number(taux, 'f', 1) + "%</span>";
    statistiques += "<br><br>👴 <b>Top 3 Anciens Employés :</b><br>" + top3;
    ui->label_statistiques->setText(statistiques);

    // 🔵 Camembert Disponibilité
    QPieSeries *series = new QPieSeries();
    series->append("Disponibles", dispo);
    series->append("Absents", indispo);
    series->setLabelsVisible();

    QPieSlice *dispoSlice = series->slices().at(0);
    QPieSlice *absentSlice = series->slices().at(1);
    dispoSlice->setBrush(QColor("#2ecc71"));
    absentSlice->setBrush(QColor("#e74c3c"));
    absentSlice->setExploded(true);
    absentSlice->setExplodeDistanceFactor(0.10);
    dispoSlice->setLabelColor(Qt::white);
    absentSlice->setLabelColor(Qt::white);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("📊 Répartition des employés par disponibilité");
    chart->setTitleFont(QFont("Segoe UI", 14, QFont::Bold));
    chart->setTitleBrush(Qt::white);
    chart->setBackgroundBrush(QColor("#2c3e50"));
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

    // 👉 Afficher aussi le tableau d'absences détaillées
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
void MainWindow::on_button_chatbot_icon_clicked()
{
    if (ui->frame_chatbox->isHidden()) {
        ui->frame_chatbox->show();  // ✅ Affiche correctement
        qDebug() << "Chatbot affiché";
    } else {
        ui->frame_chatbox->hide();  // ✅ Cache s’il est déjà visible
        qDebug() << "Chatbot caché";
    }
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

    envoyerRequeteChatGPT(question);  // Sinon GPT4All
}

void MainWindow::envoyerRequeteChatGPT(const QString &message)
{
    QString question = message.toLower();

    // Chercher dans la FAQ intégrée
    for (const QJsonValue &entry : faq) {
        QJsonObject obj = entry.toObject();
        QJsonArray keywords = obj["keywords"].toArray();
        for (const QJsonValue &kw : keywords) {
            QString keyword = kw.toString().toLower();
            if (question.contains(keyword)) {
                QString rep = obj["response"].toString();
                ui->textEdit_chat->append("🤖 RHBot : " + rep);
                return;
            }
        }
    }

    // Si aucune réponse dans la FAQ, faire appel à GPT (en français)
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

    json["model"] = "Mistral Instruct";  // ou celui que tu utilises localement
    json["messages"] = messages;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    QNetworkRequest request(QUrl("http://localhost:4891/v1/chat/completions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    manager->post(request, data);
}



void MainWindow::receptionReponseGPT(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        QString erreur = reply->errorString();
        ui->textEdit_chat->append("❌ Erreur API : " + erreur);
        reply->deleteLater();
        return;
    }

    QByteArray response = reply->readAll();
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        ui->textEdit_chat->append("❌ Erreur JSON : " + parseError.errorString());
        reply->deleteLater();
        return;
    }

    if (!doc.isObject()) {
        ui->textEdit_chat->append("❌ Erreur : Réponse inattendue.");
        reply->deleteLater();
        return;
    }

    QJsonObject obj = doc.object();
    QJsonArray choices = obj["choices"].toArray();

    if (!choices.isEmpty()) {
        QJsonObject message = choices[0].toObject()["message"].toObject();
        QString content = message["content"].toString().trimmed();

        ui->textEdit_chat->append("🤖 RHBot : " + content);
    } else {
        ui->textEdit_chat->append("❌ Erreur : Réponse vide.");
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
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString contenu = in.readAll();
        ui->textEdit_historique->setPlainText(contenu);
        file.close();
    }
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

// 🔹 Afficher la liste des carnets
void MainWindow::displayCarnet()
{
    QSqlQueryModel *model = carnetTmp.afficher();
    if (model) {
        ui->tableVieww->setModel(model);
        ui->tableVieww->resizeColumnsToContents();
    } else {
        QMessageBox::warning(this, "Erreur", "Échec du chargement des carnets.");
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

        /* Structure principale en tableau - Augmenté largeur à 100% */
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

        /* Colonne gauche pour les données - Réduit à 65% pour donner plus d'espace à la colonne droite */
        .data-column {
            width: 80%;
            padding-right: 30px;
        }

        /* Colonne droite pour le rendez-vous - Augmenté à 35% */
        .rdv-column {
            width: 70%;
        }

        /* Tableau des données patient - Largeur augmentée à 100% */
        .patient-table {
            border-collapse: collapse;
            width: 120%;
        }

        .patient-table td, .patient-table th {
            border: 4px solid black; /* Bordure plus épaisse */
            padding: 25px; /* Padding augmenté pour plus de hauteur */
            font-size: 42pt; /* Taille de police augmentée */
        }

        .signature {
            color: #800000;
            font-style: italic;
            margin-top: 120px;
            font-size: 38pt;
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
                    <div class='rdv-box'>
                        📅 <strong>Prochain Rendez-vous</strong><br><br>
                        )" + dateRdv + R"(
                    </div>
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
    ui->tab_2->setCurrentIndex(3);  // Change l’index si nécessaire

    if (!ui->stat_carnet) {
        qDebug() << "Erreur : L'onglet stat_carnet n'existe pas";
        return;
    }

    // Nettoyer l’ancien layout
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

    // Requête SQL pour compter les vaccinés/non vaccinés
    int totalV = 0, totalNV = 0, total = 0;
    QSqlQuery query("SELECT STATUT_VACCINAL FROM CARNETS");
    while (query.next()) {
        QString statut = query.value(0).toString().toLower();
        if (statut == "vacciné" || statut == "vacciner" || statut == "vaccine")
            totalV++;
        else
            totalNV++;
    }
    total = totalV + totalNV;

    // Calcul des pourcentages
    double pourcentageV = total > 0 ? (double(totalV) / total) * 100 : 0;
    double pourcentageNV = total > 0 ? (double(totalNV) / total) * 100 : 0;

    // Création du graphique camembert avec pourcentages
    QPieSeries *series = new QPieSeries();
    series->append(QString("Vaccinés (%1%)").arg(QString::number(pourcentageV, 'f', 1)), totalV);
    series->append(QString("Non Vaccinés (%1%)").arg(QString::number(pourcentageNV, 'f', 1)), totalNV);

    QPieSlice *sliceV = series->slices().at(0);
    QPieSlice *sliceNV = series->slices().at(1);
    sliceV->setBrush(Qt::green);
    sliceNV->setBrush(Qt::red);
    series->setLabelsVisible(true);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("📊 Couverture vaccinale totale (en %)");
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(500, 400);

    // Résumé en bas avec pourcentages
    QLabel *summary = new QLabel(
        QString("🟢 Vaccinés : %1% | 🔴 Non Vaccinés : %2%")
            .arg(QString::number(pourcentageV, 'f', 1))
            .arg(QString::number(pourcentageNV, 'f', 1)));
    summary->setAlignment(Qt::AlignCenter);
    summary->setStyleSheet("font-style: italic; font-size: 16px;");

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(chartView);
    layout->addWidget(summary);

    ui->stat_carnet->setLayout(layout);
    ui->stat_carnet->update();
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
