/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtGui/qscreen.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.3. It"
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
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "updateStatus",
    "",
    "status",
    "plotData",
    "switchToLiveView",
    "onReadyRead",
    "on_liveViewButton_clicked",
    "on_calendarWidget_clicked",
    "date",
    "on_pushButton_import_clicked",
    "on_pushButton_export_clicked",
    "on_actionConexi_n_triggered",
    "on_actionPreferencias_triggered"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {
    uint offsetsAndSizes[28];
    char stringdata0[11];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[7];
    char stringdata4[9];
    char stringdata5[17];
    char stringdata6[12];
    char stringdata7[26];
    char stringdata8[26];
    char stringdata9[5];
    char stringdata10[29];
    char stringdata11[29];
    char stringdata12[28];
    char stringdata13[32];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSMainWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSMainWindowENDCLASS_t qt_meta_stringdata_CLASSMainWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWindow"
        QT_MOC_LITERAL(11, 12),  // "updateStatus"
        QT_MOC_LITERAL(24, 0),  // ""
        QT_MOC_LITERAL(25, 6),  // "status"
        QT_MOC_LITERAL(32, 8),  // "plotData"
        QT_MOC_LITERAL(41, 16),  // "switchToLiveView"
        QT_MOC_LITERAL(58, 11),  // "onReadyRead"
        QT_MOC_LITERAL(70, 25),  // "on_liveViewButton_clicked"
        QT_MOC_LITERAL(96, 25),  // "on_calendarWidget_clicked"
        QT_MOC_LITERAL(122, 4),  // "date"
        QT_MOC_LITERAL(127, 28),  // "on_pushButton_import_clicked"
        QT_MOC_LITERAL(156, 28),  // "on_pushButton_export_clicked"
        QT_MOC_LITERAL(185, 27),  // "on_actionConexi_n_triggered"
        QT_MOC_LITERAL(213, 31)   // "on_actionPreferencias_triggered"
    },
    "MainWindow",
    "updateStatus",
    "",
    "status",
    "plotData",
    "switchToLiveView",
    "onReadyRead",
    "on_liveViewButton_clicked",
    "on_calendarWidget_clicked",
    "date",
    "on_pushButton_import_clicked",
    "on_pushButton_export_clicked",
    "on_actionConexi_n_triggered",
    "on_actionPreferencias_triggered"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   74,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    0,   77,    2, 0x08,    3 /* Private */,
       5,    0,   78,    2, 0x08,    4 /* Private */,
       6,    0,   79,    2, 0x08,    5 /* Private */,
       7,    0,   80,    2, 0x08,    6 /* Private */,
       8,    1,   81,    2, 0x08,    7 /* Private */,
      10,    0,   84,    2, 0x08,    9 /* Private */,
      11,    0,   85,    2, 0x08,   10 /* Private */,
      12,    0,   86,    2, 0x08,   11 /* Private */,
      13,    0,   87,    2, 0x08,   12 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QDate,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
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
        // method 'updateStatus'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'plotData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'switchToLiveView'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onReadyRead'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_liveViewButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_calendarWidget_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QDate &, std::false_type>,
        // method 'on_pushButton_import_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_export_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionConexi_n_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionPreferencias_triggered'
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
        case 0: _t->updateStatus((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->plotData(); break;
        case 2: _t->switchToLiveView(); break;
        case 3: _t->onReadyRead(); break;
        case 4: _t->on_liveViewButton_clicked(); break;
        case 5: _t->on_calendarWidget_clicked((*reinterpret_cast< std::add_pointer_t<QDate>>(_a[1]))); break;
        case 6: _t->on_pushButton_import_clicked(); break;
        case 7: _t->on_pushButton_export_clicked(); break;
        case 8: _t->on_actionConexi_n_triggered(); break;
        case 9: _t->on_actionPreferencias_triggered(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)(bool );
            if (_t _q_method = &MainWindow::updateStatus; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
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
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::updateStatus(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
