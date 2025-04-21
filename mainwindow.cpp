#include "mainwindow.h"
#include "produit.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QRegularExpressionValidator>
#include <QPdfWriter>
#include <QPainter>
#include <QTextDocument>
#include <QFileDialog>
#include <QDateTime>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QPieSlice>
#include "Qrcode.hpp"
#include "email.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(6);
    ui->frame->setVisible(false);

    ui->labelErreurNom->clear();
    ui->labelErreurNom->setVisible(false);
    ui->labelErreurFournisseur->clear();
    ui->labelErreurFournisseur->setVisible(false);
    ui->labelErreurDateExpiration->clear();
    ui->labelErreurDateExpiration->setVisible(false);

    QRegularExpression regexNom("^[A-Za-zÀ-ÿ ]+$");
    ui->nomProduit->setValidator(new QRegularExpressionValidator(regexNom, this));
    ui->nomFournisseur->setValidator(new QRegularExpressionValidator(regexNom, this));
    ui->dateFabrication->setDate(QDate::currentDate());
    ui->dateExpiration->setDate(QDate::currentDate());
    ui->champRecherche->setPlaceholderText("🔍 Rechercher par nom, catégorie ou fournisseur");



    connect(ui->nomProduit, &QLineEdit::textChanged, this, &MainWindow::verifierNomProduit);
    connect(ui->nomFournisseur, &QLineEdit::textChanged, this, &MainWindow::validerChampsP);
    connect(ui->dateExpiration, &QDateEdit::dateChanged, this, &MainWindow::validerChampsP);
    connect(ui->quantite, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::validerChampsP);
    connect(ui->rechercher, &QPushButton::clicked, this, &MainWindow::on_rechercherP_clicked);
    connect(ui->Stat, &QPushButton::clicked, this, &MainWindow::on_StatP_clicked);

    auto updateSidebarVisibility = [=](int index) {
        ui->frame->setVisible(index != 6);
    };

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

    connect(ui->employe, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(0);
        ui->frame->setVisible(true);
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
QString message=produitTmp.MessageDeExpiration();
qDebug()<<message;
if(message!="")
{
     email.sendEmail("najoua.dahmen18@gmail.com", "Notification d'Expiration",message );
}
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
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::verifierNomProduit()
{
    QString nomProduit = ui->nomProduit->text().trimmed();

    ui->labelErreurNom->clear();
    ui->labelErreurNom->setVisible(false);
    ui->nomProduit->setStyleSheet("");

    QRegularExpression regexNom("^[A-Za-zÀ-ÿ ]+$");

    if (nomProduit.isEmpty() || nomProduit.length() < 3 || !regexNom.match(nomProduit).hasMatch()) {
        ui->labelErreurNom->setText("❌ Le nom doit contenir au moins 3 lettres et pas de caractères spéciaux.");
        ui->labelErreurNom->setStyleSheet("color: black; font-size: 12px; font-style: italic;");
        ui->labelErreurNom->setVisible(true);
        ui->nomProduit->setStyleSheet("border: 2px solid red;");
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
        ui->labelErreurNom->setText("❌ Ce nom de produit est déjà utilisé !");
        ui->labelErreurNom->setStyleSheet("color: red; font-size: 12px; font-style: italic;");
        ui->labelErreurNom->setVisible(true);
        ui->nomProduit->setStyleSheet("border: 2px solid red;");
    } else {
        ui->labelErreurNom->setText("✔ Ce nom de produit est valide !");
        ui->labelErreurNom->setStyleSheet("color: black; font-size: 12px; font-style: italic;");
        ui->labelErreurNom->setVisible(true);
        ui->nomProduit->setStyleSheet("border: 2px solid green;");
    }
}
void MainWindow::validerChampsP()
{
    QString nom = ui->nomProduit->text().trimmed();
    QString nomFournisseur = ui->nomFournisseur->text().trimmed();
    QDate dateFabrication = ui->dateFabrication->date();
    QDate dateExpiration = ui->dateExpiration->date();
    int quantite = ui->quantite->value();

    QRegularExpression regexNom("^[A-Za-zÀ-ÿ ]+$");

    if (nom.isEmpty() || nom.length() < 3 || !regexNom.match(nom).hasMatch()) {
        ui->nomProduit->setStyleSheet("border: 2px solid red;");
        ui->labelErreurNom->setText("❌ Le nom doit contenir au moins 3 lettres.");
        ui->labelErreurNom->setStyleSheet("color: black; font-size: 12px; font-style: italic;");
        ui->labelErreurNom->setVisible(true);
    } else {
        ui->nomProduit->setStyleSheet("border: 2px solid green;");
        ui->labelErreurNom->clear();
        ui->labelErreurNom->setVisible(false);
    }

    if (nomFournisseur.isEmpty() || nomFournisseur.length() < 3 || !regexNom.match(nomFournisseur).hasMatch()) {
        ui->nomFournisseur->setStyleSheet("border: 2px solid red;");
        ui->labelErreurFournisseur->setText("❌ Le nom du fournisseur doit contenir au moins 3 lettres.");
        ui->labelErreurFournisseur->setStyleSheet("color: black; font-size: 12px; font-style: italic;");
        ui->labelErreurFournisseur->setVisible(true);
    } else {
        ui->nomFournisseur->setStyleSheet("border: 2px solid green;");
        ui->labelErreurFournisseur->clear();
        ui->labelErreurFournisseur->setVisible(false);
    }

    if (dateExpiration <= dateFabrication) {
        ui->dateExpiration->setStyleSheet("border: 2px solid red;");
        ui->labelErreurDateExpiration->setText("❌ La date d'expiration doit être après la fabrication.");
        ui->labelErreurDateExpiration->setStyleSheet("color: black; font-size: 12px; font-style: italic;");
        ui->labelErreurDateExpiration->setVisible(true);
    } else {
        ui->dateExpiration->setStyleSheet("border: 2px solid green;");
        ui->labelErreurDateExpiration->clear();
        ui->labelErreurDateExpiration->setVisible(false);
    }
    if (quantite <= 0) {
        ui->quantite->setStyleSheet("border: 2px solid red;");
        ui->labelErreurQuantite->setText("❌ La quantité doit être supérieure à 0.");
        ui->labelErreurQuantite->setStyleSheet("color: black; font-size: 12px; font-style: italic;");
        ui->labelErreurQuantite->setVisible(true);
    } else {
        ui->quantite->setStyleSheet("border: 2px solid green;");
        ui->labelErreurQuantite->clear();
        ui->labelErreurQuantite->setVisible(false);
    }
}






void MainWindow::on_supprimer_clickedP()
{
    QString nomProduit = ui->champRecherche->text().trimmed();

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
        ui->TableViewP->setModel(nullptr);
        ui->TableViewP->setModel(produit.afficher());
        ui->TableViewP->resizeColumnsToContents();

        // 🔄 Nettoyer le champ de saisie
        ui->champRecherche->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression du produit !");
    }
}


void MainWindow::on_Affichage_2_currentChanged(int index)
{
    if (index == 1) {
        Produit produit;
         // Exemple : largeur 1000px, hauteur 600px

        ui->TableViewP->setModel(produit.afficher());
        ui->TableViewP->setMinimumSize(1000, 600);
        ui->TableViewP->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // 📐 Ajustement des colonnes et lignes
        ui->TableViewP->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->TableViewP->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    }
}



void MainWindow::on_ajouterP_clicked()
{ if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "Base de données non connectée !";

    }



    QString nomProduit = ui->nomProduit->text().trimmed();
    QString categorie = ui->categorie->currentText().trimmed();
    int quantite = ui->quantite->value();
    QDate dateFabrication = ui->dateFabrication->date();
    QDate dateExpiration = ui->dateExpiration->date();
    QString nomFournisseur = ui->nomFournisseur->text().trimmed();

    qDebug() << "Nom Produit:" << nomProduit << "| Catégorie:" << categorie << "| Quantité:" << quantite
             << "| Date Fabrication:" << dateFabrication << "| Date Expiration:" << dateExpiration
             << "| Nom Fournisseur:" << nomFournisseur;
    if (!ui->labelErreurNom->text().isEmpty() && ui->labelErreurNom->text().contains("déjà utilisé")) {
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
    ui->Affichage_2->setCurrentIndex(1);
    ui->TableViewP->setModel(nullptr);
    ui->TableViewP->setModel(produit.afficher());
    ui->TableViewP->resizeColumnsToContents();

    ui->nomProduit->clear();
    ui->categorie->setCurrentIndex(0);
    ui->quantite->setValue(0);
    ui->dateFabrication->setDate(QDate::currentDate());
    ui->dateExpiration->setDate(QDate::currentDate());
    ui->nomFournisseur->clear();
}




void MainWindow::on_modifierP_clicked()
{
    QString nomProduit = ui->champRecherche->text().trimmed();

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
        ui->Affichage_2->setCurrentIndex(1);
        ui->champRecherche->clear();
        return;

    }

    remplirChampsModificationP(nomProduit);

    ui->stackedWidget->setCurrentIndex(1);
    ui->Affichage_2->setCurrentIndex(0);
    ui->champRecherche->clear();
}

void MainWindow::remplirChampsModificationP(QString nomProduit)
{
    QString categorie, nomFournisseur;
    int quantite;
    QDate dateFabrication, dateExpiration;

    Produit produit;
    if (produit.remplirChampsModification(nomProduit, categorie, quantite, dateFabrication, dateExpiration, nomFournisseur)) {
        ui->nomProduit->setText(nomProduit);
        ui->categorie->setCurrentText(categorie);
        ui->quantite->setValue(quantite);
        ui->dateFabrication->setDate(dateFabrication);
        ui->dateExpiration->setDate(dateExpiration);
        ui->nomFournisseur->setText(nomFournisseur);

        produitAModifier = nomProduit;
        modeModification=true;

        QMessageBox::information(this, "Modification", "Données chargées, vous pouvez modifier !");
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de charger les données du produit !");
    }
}




void MainWindow::on_pushButton_11_clicked()
{
    qDebug()<<"bouton annuler";


    ui->nomProduit->clear();
    ui->categorie->setCurrentIndex(0);
    ui->quantite->setValue(0);
    ui->dateFabrication->setDate(QDate::currentDate());
    ui->dateExpiration->setDate(QDate::currentDate());
    ui->nomFournisseur->clear();


    modeModification = false;
    produitAModifier.clear();


    ui->stackedWidget->setCurrentIndex(1);
    ui->Affichage_2->setCurrentIndex(1);
}



void MainWindow::on_rechercherP_clicked()
{ QString critere = ui->champRecherche->text().trimmed();

    Produit produit;
    QSqlQueryModel *model = produit.rechercherTout(critere);

    ui->TableViewP->setModel(model);
    ui->TableViewP->resizeColumnsToContents();
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
        ui->TableViewP->setModel(model);
        ui->TableViewP->resizeColumnsToContents();
    }312

}
*/

void MainWindow::on_comboBox_6P_activated(int index)
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
        //ui->TableViewP->setSortingEnabled(false);
        ui->TableViewP->setModel(model);
        ui->TableViewP->resizeColumnsToContents();
    }
}


void MainWindow::on_pdfP_clicked()
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




void MainWindow::on_TableViewP_clicked(const QModelIndex &index)
{
    QAbstractItemModel* model = ui->TableViewP->model();

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
                 ui->qrcode->setPixmap(QPixmap::fromImage(im.scaled(200, 200, Qt::KeepAspectRatio, Qt::FastTransformation), Qt::MonoOnly));



                 ui->Affichage_2->setCurrentIndex(3);


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
    QLabel *qrLabel = ui->qrcode; // Assuming your QLabel is named qrcode
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
    ui->Affichage_2->setCurrentIndex(2);

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
    while ((child = ui->layoutStatistiques->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
    chartView->setMinimumSize(800, 600);
    // ➕ Ajouter le nouveau graphique dans l'interface
    ui->layoutStatistiques->addWidget(chartView);
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
void MainWindow::lireDonneesSerie() {
    QByteArray data = A.read_from_arduino();


    if (data.contains("ALERTE_COURANT=0")) {
        // Lancer la requête SQL pour les vaccins
        QSqlQuery query;
        if (query.exec("SELECT NOM_PRODUIT, QUANTITE FROM PRODUITS WHERE CATEGORIE = 'vaccin'")) {
            QString message = "⚠️ Coupure de courant détectée !\n\nLes produits suivants nécessitent une réfrigération :\n\n";

            while (query.next()) {
                QString nom = query.value(0).toString();
                QString quantite = query.value(1).toString();
                message += "Produit : " + nom + " - Quantité : " + quantite + "\n";
            }

            QMessageBox::critical(this, "Alerte Produits Sensibles", message);
        } else {
            QMessageBox::warning(this, "Erreur SQL", "Échec lors de la requête SQL !");
        }
    }
}
