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

// Constructeur de MainWindow
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

    connect(ui->lineeditposte, &QLineEdit::textChanged, this, [=]() {
        validateInput(ui->lineeditposte, ui->posteErrorLabel,
                      QRegularExpression("^[A-Za-zÀ-ÿ\\s-]+$"),
                      "Le poste doit contenir uniquement des lettres.");
    });

    connect(ui->combobox_abscences, &QComboBox::currentTextChanged, this, [=]() {
        if (ui->combobox_abscences->currentIndex() == 0) {
            ui->abscenceErrorLabel->setText("❌ Veuillez sélectionner un type d'absence.");
            ui->abscenceErrorLabel->setStyleSheet("color: red; font-weight: bold;");
        } else {
            ui->abscenceErrorLabel->setText("✔️ Valide");
            ui->abscenceErrorLabel->setStyleSheet("color: green; font-weight: bold;");
        }
    });

    // ✅ Réinitialisation des champs avec le bouton Annuler
    connect(ui->annuler, &QPushButton::clicked, this, &MainWindow::on_annuler_clicked);

    // ✅ Fonction pour gérer l'affichage du sidebar selon la page active
    auto updateSidebarVisibility = [=](int index) {
        ui->frame->setVisible(index != 6);
    };

    // ✅ Bouton retour à la connexion
    connect(ui->pushButton_3, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(6);
        updateSidebarVisibility(6);
    });

    connect(ui->pushButton_6, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(6);
        ui->frame->setVisible(false);
    });

    connect(ui->pushButton_3, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);
        ui->frame->setVisible(true);
    });

    // ✅ Navigation entre les pages
    connect(ui->employe, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(0);
        ui->frame->setVisible(true);

        // Mettre à jour immédiatement l'affichage des employés
        Employe e;
        ui->tableView->setModel(e.afficher());

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ajouter_employe_clicked()
{
    QString cin = ui->lineedit_cin_employe->text().trimmed();
    int id_employe = ui->lineedit_id_employe->text().toInt();
    QString nom = ui->lineedit_nom_employe->text().trimmed();
    QString prenom = ui->lineedit_prenom_employe->text().trimmed();
    QString poste = ui->lineeditposte->text().trimmed();
    float salaire = ui->lineditsalaire->text().toFloat();
    QString contact = ui->lineeditcontact->text().trimmed();
    QDate date_embauche = ui->dateEdit->date();
    int disponibilite = (ui->lineeditdisponibilite->text().toLower() == "oui") ? 1 : 0;
    QString type_absences = ui->combobox_abscences->currentText();

    QString sexe;
    if (ui->radiobutton_homme->isChecked()) {
        sexe = "Homme";
    } else if (ui->radiobutton_femme->isChecked()) {
        sexe = "Femme";
    } else {
        QMessageBox::warning(this, "Champ manquant", "Veuillez sélectionner un sexe !");
        return;
    }

    // 🔹 Mode Modification 🛠
    if (modeModification) {
        Employe emp(cinOriginal, id_employe, nom, prenom, poste, sexe, salaire, contact, date_embauche, disponibilite, type_absences);

        if (emp.modifier(cinOriginal)) {
            QMessageBox::information(this, "Succès", "L'employé a été modifié avec succès !");
            modeModification = false;  // Désactiver le mode modification
            cinOriginal.clear();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la modification de l'employé !");
            return;
        }
    }
    // 🔹 Mode Ajout 🆕
    else {
        Employe emp(cin, id_employe, nom, prenom, poste, sexe, salaire, contact, date_embauche, disponibilite, type_absences);

        if (emp.ajouter()) {
            QMessageBox::information(this, "Succès", "L'employé a été ajouté avec succès !");
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de l'ajout de l'employé !");
            return;
        }
    }

    // 🔄 Mise à jour de la tableView après Ajout/Modification
    ui->tableView->setModel(employe.afficher());

    // 🧹 Effacer les champs après Ajout/Modification
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
    ui->radiobutton_homme->setChecked(false);
    ui->radiobutton_femme->setChecked(false);

    // 🔙 Revenir à la liste des employés après modification/ajout
    ui->Affichage->setCurrentWidget(ui->lirect);
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




















void MainWindow::on_button_modifier_clicked()
{
    QString cin = ui->lineedit_cin_employe_test->text().trimmed();

    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un CIN !");
        return;
    }

    // 🔹 Vérifier si l'employé existe avant de modifier
    if (!employe.chargerEmploye(cin)) {
        QMessageBox::critical(this, "Erreur", "Aucun employé trouvé avec ce CIN !");
        return;
    }

    modeModification = true;  // Active le mode modification
    cinOriginal = cin;  // ✅ Stocke l'ancien CIN pour la mise à jour

    // 🔹 Pré-remplissage des champs avec les données existantes
    ui->lineedit_cin_employe->setText(employe.getCIN());  // ✅ Correction
    ui->lineedit_id_employe->setText(QString::number(employe.getIdEmploye()));  // ✅ Correction
    ui->lineedit_nom_employe->setText(employe.getNom());
    ui->lineedit_prenom_employe->setText(employe.getPrenom());
    ui->lineeditposte->setText(employe.getPoste());
    ui->radiobutton_homme->setChecked(employe.getSexe() == "Homme");
    ui->radiobutton_femme->setChecked(employe.getSexe() == "Femme");
    ui->lineditsalaire->setText(QString::number(employe.getSalaire()));
    ui->lineeditcontact->setText(employe.getContact());
    ui->dateEdit->setDate(employe.getDateEmbauche());
    ui->lineeditdisponibilite->setText(employe.getDisponibilite() ? "Oui" : "Non");
    ui->combobox_abscences->setCurrentText(employe.getTypeAbsences());

    // ✅ Redirige vers la page d'édition
    ui->Affichage->setCurrentWidget(ui->ajoutct);
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

    // Vérification du CIN
    if (!QRegularExpression("^[0-9]{8}$").match(ui->lineedit_cin_employe->text().trimmed()).hasMatch()) {
        ui->cinErrorLabel->setText("❌ Le CIN doit contenir exactement 8 chiffres.");
        ui->cinErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->cinErrorLabel->setText("✔️ Valide");
        ui->cinErrorLabel->setStyleSheet(styleValide);
    }

    // Vérification de l'ID Employé
    bool ok;
    int id = ui->lineedit_id_employe->text().toInt(&ok);
    if (!ok || id <= 0) {
        ui->idErrorLabel->setText("❌ L'ID employé doit être un nombre positif.");
        ui->idErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->idErrorLabel->setText("✔️ Valide");
        ui->idErrorLabel->setStyleSheet(styleValide);
    }

    // Vérification du salaire
    float salaire = ui->lineditsalaire->text().toFloat(&ok);
    if (!ok || salaire <= 0) {
        ui->salaireErrorLabel->setText("❌ Le salaire doit être un nombre strictement positif.");
        ui->salaireErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->salaireErrorLabel->setText("✔️ Valide");
        ui->salaireErrorLabel->setStyleSheet(styleValide);
    }

    // Vérification du contact
    if (!QRegularExpression("^[0-9]{8}$").match(ui->lineeditcontact->text().trimmed()).hasMatch()) {
        ui->contactErrorLabel->setText("❌ Le contact doit contenir exactement 8 chiffres.");
        ui->contactErrorLabel->setStyleSheet(styleErreur);
        valide = false;
    } else {
        ui->contactErrorLabel->setText("✔️ Valide");
        ui->contactErrorLabel->setStyleSheet(styleValide);
    }

    return valide;}

void MainWindow::on_annuler_clicked()
{
    if (modeModification) {
        // 🔹 Si on est en mode modification, remettre les anciennes valeurs
        QSqlQuery query;
        query.prepare("SELECT CIN, ID_EMPLOYE, NOM, PRENOM, POSTE, SEXE, SALAIRE, CONTACT, DATE_EMBAUCHE, DISPONIBILITE, TYPE_ABSENCES FROM SMARTVACC.EMPLOYES WHERE CIN = :cin");
        query.bindValue(":cin", ui->lineedit_cin_employe->text().trimmed());

        if (query.exec() && query.next()) {
            // ✅ Remettre toutes les anciennes valeurs, y compris le CIN
            ui->lineedit_cin_employe->setText(query.value("CIN").toString());
            ui->lineedit_id_employe->setText(query.value("ID_EMPLOYE").toString());
            ui->lineedit_nom_employe->setText(query.value("NOM").toString());
            ui->lineedit_prenom_employe->setText(query.value("PRENOM").toString());
            ui->lineeditposte->setText(query.value("POSTE").toString());
            ui->radiobutton_homme->setChecked(query.value("SEXE").toString() == "Homme");
            ui->radiobutton_femme->setChecked(query.value("SEXE").toString() == "Femme");
            ui->lineditsalaire->setText(query.value("SALAIRE").toString());
            ui->lineeditcontact->setText(query.value("CONTACT").toString());
            ui->dateEdit->setDate(query.value("DATE_EMBAUCHE").toDate());
            ui->lineeditdisponibilite->setText(query.value("DISPONIBILITE").toInt() == 1 ? "Oui" : "Non");
            ui->combobox_abscences->setCurrentText(query.value("TYPE_ABSENCES").toString());
        } else {
            QMessageBox::warning(this, "Erreur", "Impossible de récupérer les données de l'employé.");
        }
    } else {
        // 🔹 Si on est en mode ajout, vider les champs
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
        ui->radiobutton_homme->setChecked(false);
        ui->radiobutton_femme->setChecked(false);
    }
}




