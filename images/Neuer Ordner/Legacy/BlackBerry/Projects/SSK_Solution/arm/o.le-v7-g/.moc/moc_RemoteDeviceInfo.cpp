/****************************************************************************
** Meta object code from reading C++ file 'RemoteDeviceInfo.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/RemoteDeviceInfo.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RemoteDeviceInfo.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RemoteDeviceInfo[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
      20,   19, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,

 // properties: name, type, flags
      53,   28, 0x00095409,
      67,   59, 0x0a495001,
      72,   59, 0x0a495001,
      80,   59, 0x0a495001,
      92,   59, 0x0a495001,
     103,   59, 0x0a495001,
     113,   59, 0x0a495001,
     120,   59, 0x0a495001,
     128,   59, 0x0a495001,
     133,   59, 0x0a495001,
     159,   59, 0x0a495001,
     185,   59, 0x0a495001,
     193,   59, 0x0a495001,
     212,   59, 0x0a495001,
     223,   59, 0x0a495001,
     229,   59, 0x0a495001,
     245,  241, 0x02495001,
     260,  241, 0x02495001,
     279,  274, 0x01495001,
     293,  274, 0x01495001,

 // properties: notify_signal_id
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,

       0        // eod
};

static const char qt_meta_stringdata_RemoteDeviceInfo[] = {
    "RemoteDeviceInfo\0\0changed()\0"
    "bb::cascades::DataModel*\0model\0QString\0"
    "name\0address\0deviceClass\0deviceType\0"
    "encrypted\0paired\0trusted\0rssi\0"
    "minimumConnectionInterval\0"
    "maximumConnectionInterval\0latency\0"
    "supervisoryTimeout\0appearance\0flags\0"
    "connectable\0int\0deviceClassInt\0"
    "deviceTypeInt\0bool\0encryptedBool\0"
    "pairedBool\0"
};

void RemoteDeviceInfo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        RemoteDeviceInfo *_t = static_cast<RemoteDeviceInfo *>(_o);
        switch (_id) {
        case 0: _t->changed(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData RemoteDeviceInfo::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject RemoteDeviceInfo::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RemoteDeviceInfo,
      qt_meta_data_RemoteDeviceInfo, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RemoteDeviceInfo::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RemoteDeviceInfo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RemoteDeviceInfo::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RemoteDeviceInfo))
        return static_cast<void*>(const_cast< RemoteDeviceInfo*>(this));
    return QObject::qt_metacast(_clname);
}

int RemoteDeviceInfo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bb::cascades::DataModel**>(_v) = model(); break;
        case 1: *reinterpret_cast< QString*>(_v) = name(); break;
        case 2: *reinterpret_cast< QString*>(_v) = address(); break;
        case 3: *reinterpret_cast< QString*>(_v) = deviceClass(); break;
        case 4: *reinterpret_cast< QString*>(_v) = deviceType(); break;
        case 5: *reinterpret_cast< QString*>(_v) = encrypted(); break;
        case 6: *reinterpret_cast< QString*>(_v) = paired(); break;
        case 7: *reinterpret_cast< QString*>(_v) = trusted(); break;
        case 8: *reinterpret_cast< QString*>(_v) = rssi(); break;
        case 9: *reinterpret_cast< QString*>(_v) = minimumConnectionInterval(); break;
        case 10: *reinterpret_cast< QString*>(_v) = maximumConnectionInterval(); break;
        case 11: *reinterpret_cast< QString*>(_v) = latency(); break;
        case 12: *reinterpret_cast< QString*>(_v) = supervisoryTimeout(); break;
        case 13: *reinterpret_cast< QString*>(_v) = appearance(); break;
        case 14: *reinterpret_cast< QString*>(_v) = flags(); break;
        case 15: *reinterpret_cast< QString*>(_v) = connectable(); break;
        case 16: *reinterpret_cast< int*>(_v) = deviceClassInt(); break;
        case 17: *reinterpret_cast< int*>(_v) = deviceTypeInt(); break;
        case 18: *reinterpret_cast< bool*>(_v) = encryptedBool(); break;
        case 19: *reinterpret_cast< bool*>(_v) = pairedBool(); break;
        }
        _id -= 20;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 20;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 20;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 20;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 20;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 20;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 20;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 20;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void RemoteDeviceInfo::changed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
