/****************************************************************************
** Meta object code from reading C++ file 'AUserInterface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../AUserInterface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AUserInterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AUserInterface_t {
    QByteArrayData data[22];
    char stringdata0[244];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AUserInterface_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AUserInterface_t qt_meta_stringdata_AUserInterface = {
    {
QT_MOC_LITERAL(0, 0, 14), // "AUserInterface"
QT_MOC_LITERAL(1, 15, 14), // "VisibleBoxSlot"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 5), // "Index"
QT_MOC_LITERAL(4, 37, 5), // "State"
QT_MOC_LITERAL(5, 43, 13), // "UpdateBoxSlot"
QT_MOC_LITERAL(6, 57, 11), // "ScanBoxSlot"
QT_MOC_LITERAL(7, 69, 8), // "SetHLine"
QT_MOC_LITERAL(8, 78, 3), // "Mat"
QT_MOC_LITERAL(9, 82, 11), // "HLineParams"
QT_MOC_LITERAL(10, 94, 8), // "SetVLine"
QT_MOC_LITERAL(11, 103, 11), // "VLineParams"
QT_MOC_LITERAL(12, 115, 13), // "SetLaserPlane"
QT_MOC_LITERAL(13, 129, 16), // "LaserPlaneParams"
QT_MOC_LITERAL(14, 146, 13), // "SerialConnect"
QT_MOC_LITERAL(15, 160, 14), // "IsSerialOpened"
QT_MOC_LITERAL(16, 175, 9), // "TimerInit"
QT_MOC_LITERAL(17, 185, 8), // "PeriodMs"
QT_MOC_LITERAL(18, 194, 9), // "TimerStop"
QT_MOC_LITERAL(19, 204, 8), // "StepOnce"
QT_MOC_LITERAL(20, 213, 14), // "IsTimerRunning"
QT_MOC_LITERAL(21, 228, 15) // "ToggleTableFlag"

    },
    "AUserInterface\0VisibleBoxSlot\0\0Index\0"
    "State\0UpdateBoxSlot\0ScanBoxSlot\0"
    "SetHLine\0Mat\0HLineParams\0SetVLine\0"
    "VLineParams\0SetLaserPlane\0LaserPlaneParams\0"
    "SerialConnect\0IsSerialOpened\0TimerInit\0"
    "PeriodMs\0TimerStop\0StepOnce\0IsTimerRunning\0"
    "ToggleTableFlag"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AUserInterface[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   79,    2, 0x0a /* Public */,
       5,    2,   84,    2, 0x0a /* Public */,
       6,    2,   89,    2, 0x0a /* Public */,
       7,    1,   94,    2, 0x0a /* Public */,
      10,    1,   97,    2, 0x0a /* Public */,
      12,    1,  100,    2, 0x0a /* Public */,
      14,    0,  103,    2, 0x0a /* Public */,
      15,    0,  104,    2, 0x0a /* Public */,
      16,    1,  105,    2, 0x0a /* Public */,
      18,    0,  108,    2, 0x0a /* Public */,
      19,    0,  109,    2, 0x0a /* Public */,
      20,    0,  110,    2, 0x0a /* Public */,
      21,    0,  111,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    3,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    3,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    3,    4,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 8,   11,
    QMetaType::Void, 0x80000000 | 8,   13,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void,

       0        // eod
};

void AUserInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AUserInterface *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->VisibleBoxSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->UpdateBoxSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: _t->ScanBoxSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 3: _t->SetHLine((*reinterpret_cast< Mat(*)>(_a[1]))); break;
        case 4: _t->SetVLine((*reinterpret_cast< Mat(*)>(_a[1]))); break;
        case 5: _t->SetLaserPlane((*reinterpret_cast< Mat(*)>(_a[1]))); break;
        case 6: _t->SerialConnect(); break;
        case 7: { bool _r = _t->IsSerialOpened();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->TimerInit((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->TimerStop(); break;
        case 10: _t->StepOnce(); break;
        case 11: { bool _r = _t->IsTimerRunning();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 12: _t->ToggleTableFlag(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject AUserInterface::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_AUserInterface.data,
    qt_meta_data_AUserInterface,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *AUserInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AUserInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AUserInterface.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int AUserInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
