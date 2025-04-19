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
    "on_pushButtonStat_14_clicked",
    "on_pushButton_32_clicked",
    "displayLaboratoires",
    "on_pushButton_37_clicked",
    "on_pushButton_34_clicked",
    "validateFields",
    "populateResponsableComboBox",
    "rechercherLabo",
    "searchText",
    "on_comboBox_Tri_currentIndexChanged_M",
    "onTabWidgetPageChanged",
    "index",
    "onExcelClicked",
    "populateNomlabComboBox",
    "showMap",
    "on_selectLab_clicked",
    "on_sendMessage_clicked",
    "Read_Data_From_Socket",
    "clearAjoutFields",
    "on_ajouter_6_clicked",
    "validerChampsP",
    "on_supprimer_6_clicked",
    "on_Affichage_31_currentChanged",
    "remplirChampsModificationP",
    "nomProduit",
    "verifierNomProduit",
    "on_modifier_6_clicked",
    "on_pushButton_26_clicked",
    "on_rechercherP_clicked",
    "on_comboBox_11_activated",
    "on_pdf_6_clicked",
    "on_StatP_clicked",
    "on_TableViewP_6_clicked",
    "QModelIndex",
    "on_telecharger_qr_code_clicked",
    "on_pushButton_ajouter_v_clicked",
    "on_pushButton_suppv_clicked",
    "on_pushButton_modifier_2_clicked",
    "remplirChampsModification",
    "nomv",
    "on_lineEdit_recherche_2_textChanged",
    "verifierVaccinsExpires",
    "updateCovidStats",
    "country",
    "confirmed",
    "deaths",
    "recovered",
    "active",
    "showError",
    "error",
    "updatePrediction",
    "predictionResult",
    "lancerPrediction",
    "verifierNom",
    "nomExisteDeja",
    "nom",
    "on_pushButton_annuler_2_clicked",
    "on_pushButton_pdf_2_clicked",
    "on_pushButtonStat_18_clicked",
    "on_comboBox_tri_2_currentIndexChanged"
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
      88,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  542,    2, 0x08,    1 /* Private */,
       3,    0,  543,    2, 0x08,    2 /* Private */,
       4,    0,  544,    2, 0x08,    3 /* Private */,
       5,    0,  545,    2, 0x08,    4 /* Private */,
       6,    0,  546,    2, 0x08,    5 /* Private */,
       7,    0,  547,    2, 0x08,    6 /* Private */,
       8,    0,  548,    2, 0x08,    7 /* Private */,
       9,    0,  549,    2, 0x08,    8 /* Private */,
      10,    0,  550,    2, 0x08,    9 /* Private */,
      11,    0,  551,    2, 0x08,   10 /* Private */,
      12,    0,  552,    2, 0x08,   11 /* Private */,
      13,    0,  553,    2, 0x08,   12 /* Private */,
      14,    0,  554,    2, 0x08,   13 /* Private */,
      15,    1,  555,    2, 0x08,   14 /* Private */,
      18,    0,  558,    2, 0x08,   16 /* Private */,
      19,    0,  559,    2, 0x08,   17 /* Private */,
      20,    0,  560,    2, 0x08,   18 /* Private */,
      21,    0,  561,    2, 0x08,   19 /* Private */,
      22,    0,  562,    2, 0x08,   20 /* Private */,
      23,    0,  563,    2, 0x08,   21 /* Private */,
      24,    0,  564,    2, 0x08,   22 /* Private */,
      25,    1,  565,    2, 0x08,   23 /* Private */,
      27,    0,  568,    2, 0x08,   25 /* Private */,
      28,    0,  569,    2, 0x08,   26 /* Private */,
      29,    0,  570,    2, 0x08,   27 /* Private */,
      30,    0,  571,    2, 0x08,   28 /* Private */,
      31,    0,  572,    2, 0x08,   29 /* Private */,
      32,    0,  573,    2, 0x08,   30 /* Private */,
      33,    0,  574,    2, 0x08,   31 /* Private */,
      34,    0,  575,    2, 0x08,   32 /* Private */,
      35,    0,  576,    2, 0x08,   33 /* Private */,
      36,    0,  577,    2, 0x08,   34 /* Private */,
      37,    0,  578,    2, 0x08,   35 /* Private */,
      38,    1,  579,    2, 0x08,   36 /* Private */,
      40,    1,  582,    2, 0x08,   38 /* Private */,
      41,    0,  585,    2, 0x08,   40 /* Private */,
      42,    1,  586,    2, 0x08,   41 /* Private */,
      45,    0,  589,    2, 0x08,   43 /* Private */,
      46,    0,  590,    2, 0x08,   44 /* Private */,
      47,    1,  591,    2, 0x08,   45 /* Private */,
      49,    1,  594,    2, 0x08,   47 /* Private */,
      51,    0,  597,    2, 0x08,   49 /* Private */,
      52,    0,  598,    2, 0x08,   50 /* Private */,
      53,    0,  599,    2, 0x08,   51 /* Private */,
      54,    0,  600,    2, 0x08,   52 /* Private */,
      55,    0,  601,    2, 0x08,   53 /* Private */,
      56,    0,  602,    2, 0x08,   54 /* Private */,
      57,    0,  603,    2, 0x08,   55 /* Private */,
      58,    1,  604,    2, 0x08,   56 /* Private */,
      60,    0,  607,    2, 0x08,   58 /* Private */,
      61,    1,  608,    2, 0x08,   59 /* Private */,
      63,    0,  611,    2, 0x08,   61 /* Private */,
      64,    0,  612,    2, 0x08,   62 /* Private */,
      65,    0,  613,    2, 0x08,   63 /* Private */,
      66,    0,  614,    2, 0x08,   64 /* Private */,
      67,    0,  615,    2, 0x08,   65 /* Private */,
      68,    0,  616,    2, 0x08,   66 /* Private */,
      69,    0,  617,    2, 0x08,   67 /* Private */,
      70,    0,  618,    2, 0x08,   68 /* Private */,
      71,    0,  619,    2, 0x08,   69 /* Private */,
      72,    0,  620,    2, 0x08,   70 /* Private */,
      73,    1,  621,    2, 0x08,   71 /* Private */,
      74,    1,  624,    2, 0x08,   73 /* Private */,
      76,    0,  627,    2, 0x08,   75 /* Private */,
      77,    0,  628,    2, 0x08,   76 /* Private */,
      78,    0,  629,    2, 0x08,   77 /* Private */,
      79,    0,  630,    2, 0x08,   78 /* Private */,
      80,    1,  631,    2, 0x08,   79 /* Private */,
      81,    0,  634,    2, 0x08,   81 /* Private */,
      82,    0,  635,    2, 0x08,   82 /* Private */,
      83,    1,  636,    2, 0x08,   83 /* Private */,
      85,    0,  639,    2, 0x08,   85 /* Private */,
      86,    0,  640,    2, 0x08,   86 /* Private */,
      87,    0,  641,    2, 0x08,   87 /* Private */,
      88,    0,  642,    2, 0x08,   88 /* Private */,
      89,    1,  643,    2, 0x08,   89 /* Private */,
      91,    1,  646,    2, 0x08,   91 /* Private */,
      92,    0,  649,    2, 0x08,   93 /* Private */,
      93,    5,  650,    2, 0x08,   94 /* Private */,
      99,    1,  661,    2, 0x08,  100 /* Private */,
     101,    1,  664,    2, 0x08,  102 /* Private */,
     103,    0,  667,    2, 0x08,  104 /* Private */,
     104,    0,  668,    2, 0x08,  105 /* Private */,
     105,    1,  669,    2, 0x08,  106 /* Private */,
     107,    0,  672,    2, 0x08,  108 /* Private */,
     108,    0,  673,    2, 0x08,  109 /* Private */,
     109,    0,  674,    2, 0x08,  110 /* Private */,
     110,    1,  675,    2, 0x08,  111 /* Private */,

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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   59,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   62,
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
    QMetaType::Void, QMetaType::Int,   62,
    QMetaType::Void, QMetaType::QString,   75,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   62,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 84,   62,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   90,
    QMetaType::Void, QMetaType::QString,   26,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   94,   95,   96,   97,   98,
    QMetaType::Void, QMetaType::QString,  100,
    QMetaType::Void, QMetaType::QString,  102,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QString,  106,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   62,

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
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_32_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'displayLaboratoires'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_37_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_34_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'validateFields'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'populateResponsableComboBox'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'rechercherLabo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_comboBox_Tri_currentIndexChanged_M'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTabWidgetPageChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onExcelClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'populateNomlabComboBox'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showMap'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_selectLab_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_sendMessage_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'Read_Data_From_Socket'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'clearAjoutFields'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_ajouter_6_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'validerChampsP'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_supprimer_6_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_Affichage_31_currentChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'remplirChampsModificationP'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'verifierNomProduit'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_modifier_6_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_26_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_rechercherP_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_comboBox_11_activated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_pdf_6_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_StatP_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_TableViewP_6_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        // method 'on_telecharger_qr_code_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_ajouter_v_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_suppv_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_modifier_2_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'remplirChampsModification'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'on_lineEdit_recherche_2_textChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'verifierVaccinsExpires'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateCovidStats'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'showError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'updatePrediction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'lancerPrediction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'verifierNom'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'nomExisteDeja'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_pushButton_annuler_2_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_pdf_2_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButtonStat_18_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_comboBox_tri_2_currentIndexChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
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
        case 42: _t->on_pushButton_32_clicked(); break;
        case 43: _t->displayLaboratoires(); break;
        case 44: _t->on_pushButton_37_clicked(); break;
        case 45: _t->on_pushButton_34_clicked(); break;
        case 46: _t->validateFields(); break;
        case 47: _t->populateResponsableComboBox(); break;
        case 48: _t->rechercherLabo((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 49: _t->on_comboBox_Tri_currentIndexChanged_M(); break;
        case 50: _t->onTabWidgetPageChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 51: _t->onExcelClicked(); break;
        case 52: _t->populateNomlabComboBox(); break;
        case 53: _t->showMap(); break;
        case 54: _t->on_selectLab_clicked(); break;
        case 55: _t->on_sendMessage_clicked(); break;
        case 56: _t->Read_Data_From_Socket(); break;
        case 57: _t->clearAjoutFields(); break;
        case 58: _t->on_ajouter_6_clicked(); break;
        case 59: _t->validerChampsP(); break;
        case 60: _t->on_supprimer_6_clicked(); break;
        case 61: _t->on_Affichage_31_currentChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 62: _t->remplirChampsModificationP((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 63: _t->verifierNomProduit(); break;
        case 64: _t->on_modifier_6_clicked(); break;
        case 65: _t->on_pushButton_26_clicked(); break;
        case 66: _t->on_rechercherP_clicked(); break;
        case 67: _t->on_comboBox_11_activated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 68: _t->on_pdf_6_clicked(); break;
        case 69: _t->on_StatP_clicked(); break;
        case 70: _t->on_TableViewP_6_clicked((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 71: _t->on_telecharger_qr_code_clicked(); break;
        case 72: _t->on_pushButton_ajouter_v_clicked(); break;
        case 73: _t->on_pushButton_suppv_clicked(); break;
        case 74: _t->on_pushButton_modifier_2_clicked(); break;
        case 75: _t->remplirChampsModification((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 76: _t->on_lineEdit_recherche_2_textChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 77: _t->verifierVaccinsExpires(); break;
        case 78: _t->updateCovidStats((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[5]))); break;
        case 79: _t->showError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 80: _t->updatePrediction((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 81: _t->lancerPrediction(); break;
        case 82: _t->verifierNom(); break;
        case 83: { bool _r = _t->nomExisteDeja((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 84: _t->on_pushButton_annuler_2_clicked(); break;
        case 85: _t->on_pushButton_pdf_2_clicked(); break;
        case 86: _t->on_pushButtonStat_18_clicked(); break;
        case 87: _t->on_comboBox_tri_2_currentIndexChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
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
        if (_id < 88)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 88;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 88)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 88;
    }
    return _id;
}
QT_WARNING_POP
