/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSDragDropListWidgetENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSDragDropListWidgetENDCLASS = QtMocHelpers::stringData(
    "DragDropListWidget",
    "afficherMenuContextuel",
    "",
    "pos"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSDragDropListWidgetENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   20,    2, 0x08,    1 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QPoint,    3,

       0        // eod
};

Q_CONSTINIT const QMetaObject DragDropListWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QListWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSDragDropListWidgetENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSDragDropListWidgetENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSDragDropListWidgetENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<DragDropListWidget, std::true_type>,
        // method 'afficherMenuContextuel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>
    >,
    nullptr
} };

void DragDropListWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DragDropListWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->afficherMenuContextuel((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *DragDropListWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DragDropListWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSDragDropListWidgetENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QListWidget::qt_metacast(_clname);
}

int DragDropListWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QListWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "on_pushButton_supprimer_employe_clicked",
    "",
    "on_annuler_clicked",
    "on_ajouter_employe_clicked",
    "on_button_modifier_clicked",
    "appliquerTriEmployes",
    "lancerRechercheEmploye",
    "reinitialiserTableEmployes",
    "chargerNomsDansComboBoxPDF",
    "exporterPlanningHoraireHTML",
    "afficherStatistiquesEmployes",
    "afficherTableauAbsencesDetaillees",
    "on_button_chatbot_icon_clicked",
    "on_button_envoyer_clicked",
    "receptionReponseGPT",
    "QNetworkReply*",
    "reply",
    "on_button_fermer_chatbot_clicked",
    "afficherHistoriqueConnexions",
    "on_pushButtonStat_15_clicked",
    "on_pushButton_pdf_3_clicked",
    "on_ajout_carnet_clicked",
    "displayCarnet",
    "on_supprimerC_clicked",
    "on_rechercheC_textChanged",
    "arg1",
    "on_modifierC_clicked",
    "on_btnPredire_clicked",
    "on_annulerr_clicked",
    "on_btnGeneratePDF_clicked",
    "on_btnStat_clicked",
    "on_pushButton_ajouter_clicked",
    "on_button_modifierD_clicked",
    "on_Button_supprimer_clicked",
    "on_pushButton_annuler_clicked",
    "displayCompagne",
    "navigateToCampaigns",
    "on_calendar_campaigns_clicked",
    "date",
    "rechercherParDate",
    "on_btn_export_campaigns_clicked",
    "genererPDFApprovisionnement",
    "Compagne",
    "compagne",
    "validerAffectations",
    "afficherStatistiquesVaccinationParZone",
    "rechercherCompagnes",
    "texte",
    "trierCompagnes",
    "critere",
    "on_pushButtonStat_14_clicked"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      42,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  266,    2, 0x08,    1 /* Private */,
       3,    0,  267,    2, 0x08,    2 /* Private */,
       4,    0,  268,    2, 0x08,    3 /* Private */,
       5,    0,  269,    2, 0x08,    4 /* Private */,
       6,    0,  270,    2, 0x08,    5 /* Private */,
       7,    0,  271,    2, 0x08,    6 /* Private */,
       8,    0,  272,    2, 0x08,    7 /* Private */,
       9,    0,  273,    2, 0x08,    8 /* Private */,
      10,    0,  274,    2, 0x08,    9 /* Private */,
      11,    0,  275,    2, 0x08,   10 /* Private */,
      12,    0,  276,    2, 0x08,   11 /* Private */,
      13,    0,  277,    2, 0x08,   12 /* Private */,
      14,    0,  278,    2, 0x08,   13 /* Private */,
      15,    1,  279,    2, 0x08,   14 /* Private */,
      18,    0,  282,    2, 0x08,   16 /* Private */,
      19,    0,  283,    2, 0x08,   17 /* Private */,
      20,    0,  284,    2, 0x08,   18 /* Private */,
      21,    0,  285,    2, 0x08,   19 /* Private */,
      22,    0,  286,    2, 0x08,   20 /* Private */,
      23,    0,  287,    2, 0x08,   21 /* Private */,
      24,    0,  288,    2, 0x08,   22 /* Private */,
      25,    1,  289,    2, 0x08,   23 /* Private */,
      27,    0,  292,    2, 0x08,   25 /* Private */,
      28,    0,  293,    2, 0x08,   26 /* Private */,
      29,    0,  294,    2, 0x08,   27 /* Private */,
      30,    0,  295,    2, 0x08,   28 /* Private */,
      31,    0,  296,    2, 0x08,   29 /* Private */,
      32,    0,  297,    2, 0x08,   30 /* Private */,
      33,    0,  298,    2, 0x08,   31 /* Private */,
      34,    0,  299,    2, 0x08,   32 /* Private */,
      35,    0,  300,    2, 0x08,   33 /* Private */,
      36,    0,  301,    2, 0x08,   34 /* Private */,
      37,    0,  302,    2, 0x08,   35 /* Private */,
      38,    1,  303,    2, 0x08,   36 /* Private */,
      40,    1,  306,    2, 0x08,   38 /* Private */,
      41,    0,  309,    2, 0x08,   40 /* Private */,
      42,    1,  310,    2, 0x08,   41 /* Private */,
      45,    0,  313,    2, 0x08,   43 /* Private */,
      46,    0,  314,    2, 0x08,   44 /* Private */,
      47,    1,  315,    2, 0x08,   45 /* Private */,
      49,    1,  318,    2, 0x08,   47 /* Private */,
      51,    0,  321,    2, 0x08,   49 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   26,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QDate,   39,
    QMetaType::Void, QMetaType::QDate,   39,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 43,   44,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   48,
    QMetaType::Void, QMetaType::QString,   50,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'on_pushButton_supprimer_employe_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_annuler_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_ajouter_employe_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_button_modifier_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'appliquerTriEmployes'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'lancerRechercheEmploye'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'reinitialiserTableEmployes'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'chargerNomsDansComboBoxPDF'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'exporterPlanningHoraireHTML'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'afficherStatistiquesEmployes'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'afficherTableauAbsencesDetaillees'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_button_chatbot_icon_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_button_envoyer_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'receptionReponseGPT'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QNetworkReply *, std::false_type>,
        // method 'on_button_fermer_chatbot_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'afficherHistoriqueConnexions'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButtonStat_15_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_pdf_3_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_ajout_carnet_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'displayCarnet'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_supprimerC_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_rechercheC_textChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_modifierC_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnPredire_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_annulerr_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnGeneratePDF_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnStat_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_ajouter_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_button_modifierD_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_Button_supprimer_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_annuler_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'displayCompagne'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'navigateToCampaigns'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_calendar_campaigns_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QDate &, std::false_type>,
        // method 'rechercherParDate'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QDate &, std::false_type>,
        // method 'on_btn_export_campaigns_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'genererPDFApprovisionnement'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const Compagne &, std::false_type>,
        // method 'validerAffectations'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'afficherStatistiquesVaccinationParZone'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'rechercherCompagnes'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'trierCompagnes'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_pushButtonStat_14_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_pushButton_supprimer_employe_clicked(); break;
        case 1: _t->on_annuler_clicked(); break;
        case 2: _t->on_ajouter_employe_clicked(); break;
        case 3: _t->on_button_modifier_clicked(); break;
        case 4: _t->appliquerTriEmployes(); break;
        case 5: _t->lancerRechercheEmploye(); break;
        case 6: _t->reinitialiserTableEmployes(); break;
        case 7: _t->chargerNomsDansComboBoxPDF(); break;
        case 8: _t->exporterPlanningHoraireHTML(); break;
        case 9: _t->afficherStatistiquesEmployes(); break;
        case 10: _t->afficherTableauAbsencesDetaillees(); break;
        case 11: _t->on_button_chatbot_icon_clicked(); break;
        case 12: _t->on_button_envoyer_clicked(); break;
        case 13: _t->receptionReponseGPT((*reinterpret_cast< std::add_pointer_t<QNetworkReply*>>(_a[1]))); break;
        case 14: _t->on_button_fermer_chatbot_clicked(); break;
        case 15: _t->afficherHistoriqueConnexions(); break;
        case 16: _t->on_pushButtonStat_15_clicked(); break;
        case 17: _t->on_pushButton_pdf_3_clicked(); break;
        case 18: _t->on_ajout_carnet_clicked(); break;
        case 19: _t->displayCarnet(); break;
        case 20: _t->on_supprimerC_clicked(); break;
        case 21: _t->on_rechercheC_textChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 22: _t->on_modifierC_clicked(); break;
        case 23: _t->on_btnPredire_clicked(); break;
        case 24: _t->on_annulerr_clicked(); break;
        case 25: _t->on_btnGeneratePDF_clicked(); break;
        case 26: _t->on_btnStat_clicked(); break;
        case 27: _t->on_pushButton_ajouter_clicked(); break;
        case 28: _t->on_button_modifierD_clicked(); break;
        case 29: _t->on_Button_supprimer_clicked(); break;
        case 30: _t->on_pushButton_annuler_clicked(); break;
        case 31: _t->displayCompagne(); break;
        case 32: _t->navigateToCampaigns(); break;
        case 33: _t->on_calendar_campaigns_clicked((*reinterpret_cast< std::add_pointer_t<QDate>>(_a[1]))); break;
        case 34: _t->rechercherParDate((*reinterpret_cast< std::add_pointer_t<QDate>>(_a[1]))); break;
        case 35: _t->on_btn_export_campaigns_clicked(); break;
        case 36: _t->genererPDFApprovisionnement((*reinterpret_cast< std::add_pointer_t<Compagne>>(_a[1]))); break;
        case 37: _t->validerAffectations(); break;
        case 38: _t->afficherStatistiquesVaccinationParZone(); break;
        case 39: _t->rechercherCompagnes((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 40: _t->trierCompagnes((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 41: _t->on_pushButtonStat_14_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QNetworkReply* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 42)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 42;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 42)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 42;
    }
    return _id;
}
QT_WARNING_POP
