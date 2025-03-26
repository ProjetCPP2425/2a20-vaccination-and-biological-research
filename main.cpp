#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Connection c;
    bool test = c.createconnect(); // ✅ ouvrir la base D’ABORD

    if (test)
    {
        QMessageBox::information(nullptr, QObject::tr("✅ Base ouverte"),
                                 QObject::tr("Connexion réussie.\nCliquez sur Annuler pour quitter."), QMessageBox::Cancel);

        MainWindow w; // ✅ Ensuite on lance la fenêtre
        w.show();
        return a.exec();
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("❌ Base non ouverte"),
                              QObject::tr("Échec de connexion.\nCliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        return -1;
    }
}
