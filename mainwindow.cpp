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
#include <QShortcut>
#include <QToolBar>
#include <QAction>     // aussi nécessaire si tu ajoutes des actions
#include <QIcon>       // pour utiliser des icônes
#include <QKeySequence>  // pour les raccourcis clavier
#include <QProcess>








// Constructeur de MainWindow
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->frame_chatbox->hide();    // ✅ Pas setVisible(false), mais bien hide()


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
    //ui->button_chatbot_icon->setGraphicsEffect(nullptr); // assure aucun effet parasite*/

    manager = new QNetworkAccessManager(this);
   // connect(ui->button_chatbot_icon, &QPushButton::clicked, this, &MainWindow::on_button_chatbot_icon_clicked);
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::receptionReponseGPT);

    connect(ui->button_fermer_chatbot, &QPushButton::clicked, this, &MainWindow::on_button_fermer_chatbot_clicked);
    connect(ui->button_envoyer, &QPushButton::clicked, this, &MainWindow::on_button_envoyer_clicked);
    ui->textEdit_chat->setStyleSheet("color: black; background-color: white; border-radius: 10px;");


    ui->textEdit_chat->setWordWrapMode(QTextOption::WordWrap);

   // connect(ui->button_chatbot_icon, &QPushButton::clicked, this, &MainWindow::on_button_chatbot_icon_clicked);




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





}

MainWindow::~MainWindow()
{
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

    modeModification = true;  // ✅ Active le mode modification
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
    std::sort(anciens.begin(), anciens.end(), [](auto &a, auto &b) { return a.second < b.second; });

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
        if (tauxPoste > 50.0)
            statistiques += "<br><span style='color:orange;'>⚠️ " + it.key() + " : " + QString::number(tauxPoste, 'f', 1) + "% absents</span>";
    }

    statistiques += "<br><br>📈 <b>Taux d’absentéisme :</b> <span style='color:" + QString(taux > 50 ? "red" : "green") + ";'>" + QString::number(taux, 'f', 1) + "%</span>";
    statistiques += "<br><br>👴 <b>Top 3 Anciens Employés :</b><br>" + top3;
    ui->label_statistiques->setText(statistiques);

    // 🔵 Création du camembert avec % et flèches
    QPieSeries *series = new QPieSeries();
    if (total > 0) {
        double dispoPct = (double)dispo / total * 100.0;
        double absPct = (double)indispo / total * 100.0;

        QPieSlice *sliceDispo = series->append("Disponibles (" + QString::number(dispoPct, 'f', 1) + "%)", dispo);
        QPieSlice *sliceIndispo = series->append("Absents (" + QString::number(absPct, 'f', 1) + "%)", indispo);

        sliceDispo->setBrush(QColor("#2ecc71"));
        sliceIndispo->setBrush(QColor("#e74c3c"));
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


/*void MainWindow::on_button_chatbot_icon_clicked()
{
    QPropertyAnimation *animation = new QPropertyAnimation(ui->frame_chatbox, "maximumHeight");
    animation->setDuration(300);
    animation->setEasingCurve(QEasingCurve::OutCubic);

    if (ui->frame_chatbox->isVisible()) {
        animation->setStartValue(ui->frame_chatbox->height());
        animation->setEndValue(0);
        connect(animation, &QPropertyAnimation::finished, ui->frame_chatbox, &QWidget::hide);
    } else {
        ui->frame_chatbox->show();
        animation->setStartValue(0);
        animation->setEndValue(200); // ou la taille désirée
    }
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}*/


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
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString contenu = in.readAll();
        ui->textEdit_historique->setPlainText(contenu);
        file.close();
    }
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

