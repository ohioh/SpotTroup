/****************************************************************************
** Meta object code from reading C++ file 'LocalDeviceInfo.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/LocalDeviceInfo.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LocalDeviceInfo.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LocalDeviceInfo[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
      15,   19, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,

 // properties: name, type, flags
      35,   27, 0x0a495001,
      40,   27, 0x0a495001,
      48,   27, 0x0a495001,
      60,   27, 0x0a495001,
      73,   27, 0x0a495001,
      81,   27, 0x0a495001,
      92,   27, 0x0a495001,
     103,   27, 0x0a495001,
     122,   27, 0x0a495001,
     141,   27, 0x0a495001,
     157,   27, 0x0a495001,
     174,   27, 0x0a495001,
     194,   27, 0x0a495001,
     211,   27, 0x0a495001,
     231,   27, 0x0a495001,

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

       0        // eod
};

static const char qt_meta_stringdata_LocalDeviceInfo[] = {
    "LocalDeviceInfo\0\0changed()\0QString\0"
    "name\0address\0deviceClass\0discoverable\0"
    "enabled\0deviceType\0apiVersion\0"
    "masterSlaveAllowed\0maximumConnections\0"
    "maximumL2capMtu\0maximumRfcommMtu\0"
    "incomingInquiryScan\0incomingPageScan\0"
    "outgoingInquiryScan\0outgoingPageScan\0"
};

void LocalDeviceInfo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LocalDeviceInfo *_t = static_cast<LocalDeviceInfo *>(_o);
        switch (_id) {
        case 0: _t->changed(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData LocalDeviceInfo::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LocalDeviceInfo::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_LocalDeviceInfo,
      qt_meta_data_LocalDeviceInfo, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LocalDeviceInfo::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LocalDeviceInfo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LocalDeviceInfo::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LocalDeviceInfo))
        return static_cast<void*>(const_cast< LocalDeviceInfo*>(this));
    return QObject::qt_metacast(_clname);
}

int LocalDeviceInfo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
        case 0: *reinterpret_cast< QString*>(_v) = name(); break;
        case 1: *reinterpret_cast< QString*>(_v) = address(); break;
        case 2: *reinterpret_cast< QString*>(_v) = deviceClass(); break;
        case 3: *reinterpret_cast< QString*>(_v) = discoverable(); break;
        case 4: *reinterpret_cast< QString*>(_v) = enabled(); break;
        case 5: *reinterpret_cast< QString*>(_v) = deviceType(); break;
        case 6: *reinterpret_cast< QString*>(_v) = apiVersion(); break;
        case 7: *reinterpret_cast< QString*>(_v) = masterSlaveAllowed(); break;
        case 8: *reinterpret_cast< QString*>(_v) = maximumConnections(); break;
        case 9: *reinterpret_cast< QString*>(_v) = maximumL2capMtu(); break;
        case 10: *reinterpret_cast< QString*>(_v) = maximumRfcommMtu(); break;
        case 11: *reinterpret_cast< QString*>(_v) = incomingInquiryScan(); break;
        case 12: *reinterpret_cast< QString*>(_v) = incomingPageScan(); break;
        case 13: *reinterpret_cast< QString*>(_v) = outgoingInquiryScan(); break;
        case 14: *reinterpret_cast< QString*>(_v) = outgoingPageScan(); break;
        }
        _id -= 15;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 15;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 15;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 15;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 15;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 15;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 15;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 15;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void LocalDeviceInfo::changed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
