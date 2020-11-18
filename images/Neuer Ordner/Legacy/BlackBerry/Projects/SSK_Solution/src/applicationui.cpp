
#include "ApplicationUI.hpp"
#include "Timer.hpp"
#include "DataContainer.hpp"
#include "Utilities.hpp"
#include "BluetoothHandler.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

using namespace bb::cascades;

static AbstractPane *_root = 0;

ApplicationUI::ApplicationUI(bb::cascades::Application *app)
: QObject(app)
, _handler(new BluetoothHandler(this))
{

	qmlRegisterType<DeviceListing>();
    qmlRegisterType<Timer>("CustomTimer", 1, 0, "Timer");

    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

	qml->setContextProperty("_app", this);
	qml->setContextProperty("_bt", _handler);

    QObject::connect(_handler, SIGNAL(signalLinkEstablished()), this, SLOT(onLinkEstablished()), Qt::QueuedConnection);
    QObject::connect(_handler, SIGNAL(signalLostLink()), this, SLOT(onLostLink()), Qt::QueuedConnection);
    QObject::connect(_handler, SIGNAL(signalSetMessage(QString)), this, SLOT(onSetMessage(QString)), Qt::QueuedConnection);
    QObject::connect(_handler, SIGNAL(signalRssi(QVariant,QVariant)), this, SLOT(onRssi(QVariant,QVariant)), Qt::QueuedConnection);
    QObject::connect(_handler, SIGNAL(signalProximityChange(QVariant)), this, SLOT(onProximityChange(QVariant)), Qt::QueuedConnection);

    DataContainer* dc = DataContainer::getInstance();
    dc->setMinRssiNear(-50);
    dc->setMinRssiMedium(-80);
    dc->setRssiFluctuationSensitivity(10);

    sharing = false;

	_root = qml->createRootObject<AbstractPane>();

    app->setScene(_root);

}

void ApplicationUI::onLinkEstablished() {
	qDebug() << "XXXX onLinkEstablished";
	QMetaObject::invokeMethod(_root, "linkEstablished");
}

void ApplicationUI::onLostLink() {
    qDebug() << "XXXX onLostLink";
    DataContainer* dc = DataContainer::getInstance();
    dc->setProximityIndicator(-1);
    QMetaObject::invokeMethod(_root, "lostLink");
}

void ApplicationUI::onSetMessage(QString message) {
    qDebug() << "XXXX onSetMessage";
    QMetaObject::invokeMethod(_root, "setMessage", Q_ARG(QVariant, message));
}

void ApplicationUI::onRssi(QVariant proximity_indicator, QVariant rssi) {
    qDebug() << "XXXX onRssi";
    if (sharing) {
        qDebug() << "XXXX sharing proximity data";
        _handler->sendClientProximityData(proximity_indicator.toInt(), rssi.toInt());
    } else {
        qDebug() << "XXXX not sharing proximity data";
    }
    QMetaObject::invokeMethod(_root, "onRssi", Q_ARG(QVariant, proximity_indicator), Q_ARG(QVariant, rssi));
}

void ApplicationUI::onProximityChange(QVariant proximity_indicator) {
    qDebug() << "XXXX onProximityChange";
    QMetaObject::invokeMethod(_root, "onProximityChange", Q_ARG(QVariant, proximity_indicator));
}

void ApplicationUI::monitorProximity(QString device_addr, QString device_name) {
	qDebug() << "YYYY monitoring proximity of device " << device_addr;

	DataContainer* dc = DataContainer::getInstance();
	dc->setCurrentDeviceAddr(device_addr);
	dc->setCurrentDeviceName(device_name);
	_handler->startProximityMonitoring();

}

void ApplicationUI::cleanUpOnExit() {
    qDebug() << "XXXX Clean up on application exit";
    _handler->quit();
}

int ApplicationUI::getRssiNear() {
    qDebug() << "XXXX getRssiNear";
    DataContainer* dc = DataContainer::getInstance();
    return dc->getMinRssiNear();
}

int ApplicationUI::getRssiMedium() {
    qDebug() << "XXXX getRssiMedium";
    DataContainer* dc = DataContainer::getInstance();
    return dc->getMinRssiMedium();
}

int ApplicationUI::getRssiFluctuationSensitivity() {
    qDebug() << "XXXX getRssiFluctuationSensitivity";
    DataContainer* dc = DataContainer::getInstance();
    return dc->getRssiFluctuationSensitivity();
}

void ApplicationUI::startSharing() {
    qDebug() << "XXXX startSharing";
    sharing = true;
}

void ApplicationUI::stopSharing() {
    qDebug() << "XXXX stopSharing";
    sharing = false;
    // this will inform the peripheral that we've stopped sharing so it can (e.g.) switch off LEDs previously set when sharing was enabled
    _handler->sendClientProximityData(-1, 0);
}
