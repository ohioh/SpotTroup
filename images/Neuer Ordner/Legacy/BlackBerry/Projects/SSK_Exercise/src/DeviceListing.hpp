
#ifndef DEVICELISTING_HPP
#define DEVICELISTING_HPP

#include <QObject>

#include <bb/cascades/GroupDataModel>

#include <bb/system/SystemDialog>
#include <bb/system/SystemToast>

#include <btapi/btdevice.h>
#include <btapi/btspp.h>

class DeviceListing : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bb::cascades::DataModel* model READ model CONSTANT)

public:
    DeviceListing(QObject *parent = 0);

public Q_SLOTS:
    void listPairedDevices();

private:
    bb::cascades::DataModel* model() const;
    bb::cascades::GroupDataModel* _model;
    bool isProximityDevice(bt_remote_device_t * remoteDevice);
    QString IMMEDIATE_ALERT_SERVICE_UUID;
    QString LINK_LOSS_SERVICE_UUID;
    QString TX_POWER_SERVICE_UUID;
};

#endif
