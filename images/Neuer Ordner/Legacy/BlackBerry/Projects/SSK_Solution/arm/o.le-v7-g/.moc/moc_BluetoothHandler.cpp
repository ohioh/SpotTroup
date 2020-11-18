/****************************************************************************
** Meta object code from reading C++ file 'BluetoothHandler.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/BluetoothHandler.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BluetoothHandler.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BluetoothHandler[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       3,   74, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,
      42,   17,   17,   17, 0x05,
      63,   59,   17,   17, 0x05,
     114,   89,   17,   17, 0x05,
     164,  144,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
     204,  196,   17,   17, 0x0a,
     229,   17,   17,   17, 0x0a,

 // methods: signature, parameters, type, tag, flags
     251,   17,   17,   17, 0x02,
     270,   17,   17,   17, 0x02,
     285,   17,   17,   17, 0x02,
     300,   17,   17,   17, 0x02,
     316,   17,   17,   17, 0x02,

 // properties: name, type, flags
     344,  329, 0x00095409,
     375,  358, 0x00095409,
     409,  391, 0x00095409,

       0        // eod
};

static const char qt_meta_stringdata_BluetoothHandler[] = {
    "BluetoothHandler\0\0signalLinkEstablished()\0"
    "signalLostLink()\0msg\0signalSetMessage(QString)\0"
    "proximity_indicator,rssi\0"
    "signalRssi(QVariant,QVariant)\0"
    "proximity_indicator\0signalProximityChange(QVariant)\0"
    "address\0setRemoteDevice(QString)\0"
    "finishedRssiPolling()\0sendAlertRequest()\0"
    "sendAlertOff()\0sendAlertMid()\0"
    "sendAlertHigh()\0disconnect()\0"
    "DeviceListing*\0deviceListing\0"
    "LocalDeviceInfo*\0localDeviceInfo\0"
    "RemoteDeviceInfo*\0remoteDeviceInfo\0"
};

void BluetoothHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BluetoothHandler *_t = static_cast<BluetoothHandler *>(_o);
        switch (_id) {
        case 0: _t->signalLinkEstablished(); break;
        case 1: _t->signalLostLink(); break;
        case 2: _t->signalSetMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->signalRssi((*reinterpret_cast< QVariant(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        case 4: _t->signalProximityChange((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 5: _t->setRemoteDevice((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->finishedRssiPolling(); break;
        case 7: _t->sendAlertRequest(); break;
        case 8: _t->sendAlertOff(); break;
        case 9: _t->sendAlertMid(); break;
        case 10: _t->sendAlertHigh(); break;
        case 11: _t->disconnect(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData BluetoothHandler::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BluetoothHandler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BluetoothHandler,
      qt_meta_data_BluetoothHandler, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BluetoothHandler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BluetoothHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BluetoothHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BluetoothHandler))
        return static_cast<void*>(const_cast< BluetoothHandler*>(this));
    return QObject::qt_metacast(_clname);
}

int BluetoothHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< DeviceListing**>(_v) = deviceListing(); break;
        case 1: *reinterpret_cast< LocalDeviceInfo**>(_v) = localDeviceInfo(); break;
        case 2: *reinterpret_cast< RemoteDeviceInfo**>(_v) = remoteDeviceInfo(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void BluetoothHandler::signalLinkEstablished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void BluetoothHandler::signalLostLink()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void BluetoothHandler::signalSetMessage(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void BluetoothHandler::signalRssi(QVariant _t1, QVariant _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void BluetoothHandler::signalProximityChange(QVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
