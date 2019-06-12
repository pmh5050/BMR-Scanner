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
    QByteArrayData data[43];
    char stringdata0[682];
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
QT_MOC_LITERAL(21, 228, 15), // "ToggleTableFlag"
QT_MOC_LITERAL(22, 244, 26), // "SaveScannerCalibrationData"
QT_MOC_LITERAL(23, 271, 25), // "LoadCameraCalibrationPath"
QT_MOC_LITERAL(24, 297, 31), // "UpdateCameraCalibrationFromPath"
QT_MOC_LITERAL(25, 329, 26), // "LoadScannerCalibrationPath"
QT_MOC_LITERAL(26, 356, 32), // "UpdateScannerCalibrationFromPath"
QT_MOC_LITERAL(27, 389, 20), // "UpdateLabelFromFrame"
QT_MOC_LITERAL(28, 410, 10), // "ELabelType"
QT_MOC_LITERAL(29, 421, 9), // "LabelType"
QT_MOC_LITERAL(30, 431, 4), // "Mat&"
QT_MOC_LITERAL(31, 436, 5), // "Frame"
QT_MOC_LITERAL(32, 442, 18), // "UpdateRedThreshold"
QT_MOC_LITERAL(33, 461, 9), // "SpinValue"
QT_MOC_LITERAL(34, 471, 16), // "UpdateWindowSize"
QT_MOC_LITERAL(35, 488, 13), // "UpdateLimitXY"
QT_MOC_LITERAL(36, 502, 10), // "SlideValue"
QT_MOC_LITERAL(37, 513, 12), // "UpdateLimitZ"
QT_MOC_LITERAL(38, 526, 36), // "UpdateOdometryCharacteristicC..."
QT_MOC_LITERAL(39, 563, 39), // "UpdateMeasurementCharacterist..."
QT_MOC_LITERAL(40, 603, 22), // "ClearReferenceScanData"
QT_MOC_LITERAL(41, 626, 31), // "ProcessingFromReferenceScanData"
QT_MOC_LITERAL(42, 658, 23) // "InitializeScanTransform"

    },
    "AUserInterface\0VisibleBoxSlot\0\0Index\0"
    "State\0UpdateBoxSlot\0ScanBoxSlot\0"
    "SetHLine\0Mat\0HLineParams\0SetVLine\0"
    "VLineParams\0SetLaserPlane\0LaserPlaneParams\0"
    "SerialConnect\0IsSerialOpened\0TimerInit\0"
    "PeriodMs\0TimerStop\0StepOnce\0IsTimerRunning\0"
    "ToggleTableFlag\0SaveScannerCalibrationData\0"
    "LoadCameraCalibrationPath\0"
    "UpdateCameraCalibrationFromPath\0"
    "LoadScannerCalibrationPath\0"
    "UpdateScannerCalibrationFromPath\0"
    "UpdateLabelFromFrame\0ELabelType\0"
    "LabelType\0Mat&\0Frame\0UpdateRedThreshold\0"
    "SpinValue\0UpdateWindowSize\0UpdateLimitXY\0"
    "SlideValue\0UpdateLimitZ\0"
    "UpdateOdometryCharacteristicConstant\0"
    "UpdateMeasurementCharacteristicConstant\0"
    "ClearReferenceScanData\0"
    "ProcessingFromReferenceScanData\0"
    "InitializeScanTransform"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AUserInterface[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      28,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,  154,    2, 0x0a /* Public */,
       5,    2,  159,    2, 0x0a /* Public */,
       6,    2,  164,    2, 0x0a /* Public */,
       7,    1,  169,    2, 0x0a /* Public */,
      10,    1,  172,    2, 0x0a /* Public */,
      12,    1,  175,    2, 0x0a /* Public */,
      14,    0,  178,    2, 0x0a /* Public */,
      15,    0,  179,    2, 0x0a /* Public */,
      16,    1,  180,    2, 0x0a /* Public */,
      18,    0,  183,    2, 0x0a /* Public */,
      19,    0,  184,    2, 0x0a /* Public */,
      20,    0,  185,    2, 0x0a /* Public */,
      21,    0,  186,    2, 0x0a /* Public */,
      22,    0,  187,    2, 0x0a /* Public */,
      23,    0,  188,    2, 0x0a /* Public */,
      24,    0,  189,    2, 0x0a /* Public */,
      25,    0,  190,    2, 0x0a /* Public */,
      26,    0,  191,    2, 0x0a /* Public */,
      27,    2,  192,    2, 0x0a /* Public */,
      32,    1,  197,    2, 0x0a /* Public */,
      34,    1,  200,    2, 0x0a /* Public */,
      35,    1,  203,    2, 0x0a /* Public */,
      37,    1,  206,    2, 0x0a /* Public */,
      38,    1,  209,    2, 0x0a /* Public */,
      39,    1,  212,    2, 0x0a /* Public */,
      40,    0,  215,    2, 0x0a /* Public */,
      41,    0,  216,    2, 0x0a /* Public */,
      42,    0,  217,    2, 0x0a /* Public */,

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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 28, 0x80000000 | 30,   29,   31,
    QMetaType::Void, QMetaType::Int,   33,
    QMetaType::Void, QMetaType::Int,   33,
    QMetaType::Void, QMetaType::Int,   36,
    QMetaType::Void, QMetaType::Int,   36,
    QMetaType::Void, QMetaType::Int,   36,
    QMetaType::Void, QMetaType::Int,   36,
    QMetaType::Void,
    QMetaType::Void,
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
        case 13: _t->SaveScannerCalibrationData(); break;
        case 14: _t->LoadCameraCalibrationPath(); break;
        case 15: _t->UpdateCameraCalibrationFromPath(); break;
        case 16: _t->LoadScannerCalibrationPath(); break;
        case 17: _t->UpdateScannerCalibrationFromPath(); break;
        case 18: _t->UpdateLabelFromFrame((*reinterpret_cast< ELabelType(*)>(_a[1])),(*reinterpret_cast< Mat(*)>(_a[2]))); break;
        case 19: _t->UpdateRedThreshold((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->UpdateWindowSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->UpdateLimitXY((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->UpdateLimitZ((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->UpdateOdometryCharacteristicConstant((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 24: _t->UpdateMeasurementCharacteristicConstant((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 25: _t->ClearReferenceScanData(); break;
        case 26: _t->ProcessingFromReferenceScanData(); break;
        case 27: _t->InitializeScanTransform(); break;
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
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 28)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 28;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
