#ifndef BLUETOOTHHANDLER_HPP_
#define BLUETOOTHHANDLER_HPP_

#include <QObject>
#include <btapi/btdevice.h>
#include <btapi/btgatt.h>
#include <btapi/btle.h>
#include <btapi/btspp.h>
#include <QFuture>
#include <QMutex>
#include <QFutureWatcher>
#include "LocalDeviceInfo.hpp"
#include "RemoteDeviceInfo.hpp"
#include "DeviceListing.hpp"

namespace bb
{
    namespace cascades
    {
        class Application;
    }
}

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class BluetoothHandler: public QObject
{
Q_OBJECT

Q_PROPERTY(DeviceListing* deviceListing READ deviceListing CONSTANT)
Q_PROPERTY(LocalDeviceInfo* localDeviceInfo READ localDeviceInfo CONSTANT)
Q_PROPERTY(RemoteDeviceInfo* remoteDeviceInfo READ remoteDeviceInfo CONSTANT)

public:
    BluetoothHandler(QObject *obj);
    virtual ~BluetoothHandler();
    enum AlertLevel
    {
        Off, Mid, High
    };
    AlertLevel _alertLevel;
    void setAlertLevel(const AlertLevel level);
    Q_INVOKABLE void sendAlertRequest();
    Q_INVOKABLE void sendAlertOff();
    Q_INVOKABLE void sendAlertMid();
    Q_INVOKABLE void sendAlertHigh();
    Q_INVOKABLE void disconnect();

    void startProximityMonitoring();
    void startRssiPolling();
    void pollRssi();
    void quit();
    void sendClientProximityData(uint8_t proximity_indicator, int rssi);

    int calculateProximityIndicator(int *rssi);

    void emitSignalLinkEstablished();
    void emitSignalLostLink();
    void emitSignalSetMessage(QString msd);
    void emitSignalEnableProximityVisualisation();
    void emitSignalDisableProximityVisualisation();

signals:
    void signalLinkEstablished();
    void signalLostLink();
    void signalSetMessage(QString msg);
    void signalRssi(QVariant proximity_indicator, QVariant rssi);
    void signalProximityChange(QVariant proximity_indicator);

private:
    LocalDeviceInfo *_localDeviceInfo;
    DeviceListing *_deviceListing;
    RemoteDeviceInfo *_remoteDeviceInfo;

    DeviceListing *deviceListing() const;
    LocalDeviceInfo *localDeviceInfo() const;
    RemoteDeviceInfo *remoteDeviceInfo() const;

    QFuture<void> *_future;
    QFutureWatcher<void> *_watcher;

public slots:
    void setRemoteDevice(const QString &address);
    void finishedRssiPolling();

};

#endif /* BLUETOOTHHANDLER_HPP_ */
