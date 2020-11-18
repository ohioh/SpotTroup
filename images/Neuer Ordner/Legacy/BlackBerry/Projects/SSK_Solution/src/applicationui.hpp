#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include "BluetoothHandler.hpp"

namespace bb
{
    namespace cascades
    {
        class Application;
    }
}

class ApplicationUI: public QObject
{
Q_OBJECT
public:
    ApplicationUI(bb::cascades::Application *app);
    virtual ~ApplicationUI()
    {
    }

    Q_INVOKABLE
    void monitorProximity(QString device_addr, QString device_name);Q_INVOKABLE
    Q_INVOKABLE
    int getRssiNear();
    Q_INVOKABLE
    int getRssiMedium();
    Q_INVOKABLE
    int getRssiFluctuationSensitivity();
    Q_INVOKABLE
    void startSharing();
    Q_INVOKABLE
    void stopSharing();

public slots:
    void onLinkEstablished();
    void onLostLink();
    void onSetMessage(QString message);
    void cleanUpOnExit();
    void onRssi(QVariant proximity_indicator, QVariant rssi);
    void onProximityChange(QVariant proximity_indicator);

private:
    BluetoothHandler* _handler;
    bool sharing;

};

#endif /* ApplicationUI_HPP_ */
