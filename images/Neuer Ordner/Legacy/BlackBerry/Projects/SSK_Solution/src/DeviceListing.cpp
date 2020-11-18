#include "DeviceListing.hpp"
#include "Utilities.hpp"

DeviceListing::DeviceListing(QObject *parent) :
        QObject(parent), _model(
                new bb::cascades::GroupDataModel(
                        QStringList() << "deviceName" << "deviceAddress" << "deviceClass"
                                << "deviceType", this)), IMMEDIATE_ALERT_SERVICE_UUID("1802"), TX_POWER_SERVICE_UUID(
                "1804"), LINK_LOSS_SERVICE_UUID("1803")

{
    _model->setSortingKeys(QStringList() << "deviceType");
    _model->setGrouping(bb::cascades::ItemGrouping::ByFullValue);

    if (!Utilities::getOSVersion().startsWith("10.0")) {
        IMMEDIATE_ALERT_SERVICE_UUID.prepend("0x");
        TX_POWER_SERVICE_UUID.prepend("0x");
        LINK_LOSS_SERVICE_UUID.prepend("0x");
    }

}

void DeviceListing::listPairedDevices()
{
    qDebug() << "YYYY DeviceListing::listPairedDevices() - clearing model";

    _model->clear();

    bt_remote_device_t **remoteDeviceArray;
    bt_remote_device_t *nextRemoteDevice = 0;

    qDebug()
            << "YYYY DeviceListing::listPairedDevices() - about to call bt_disc_retrieve_devices(BT_DISCOVERY_PREKNOWN, 0)";

    remoteDeviceArray = bt_disc_retrieve_devices(BT_DISCOVERY_PREKNOWN, 0);

    qDebug()
            << "YYYY DeviceListing::listPairedDevices() - returned from bt_disc_retrieve_devices(BT_DISCOVERY_PREKNOWN, 0)";

    if (remoteDeviceArray) {
        qDebug() << "YYYY DeviceListing::listPairedDevices() - found devices";
        for (int i = 0; (nextRemoteDevice = remoteDeviceArray[i]); ++i) {
            QVariantMap map;
            char buffer[128];
            const int bufferSize = sizeof(buffer);

            qDebug() << "YYYY DeviceListing::listPairedDevices() - checking device";

            if (isProximityDevice(nextRemoteDevice)) {
                qDebug() << "YYYY DeviceListing::listPairedDevices() - found a proximity device";
                bt_rdev_get_friendly_name(nextRemoteDevice, buffer, bufferSize);
                map["deviceName"] = QString::fromLatin1(buffer);
                qDebug() << "YYYY DeviceListing::listPairedDevices() - name=" << QString::fromLatin1(buffer);
                bt_rdev_get_address(nextRemoteDevice, buffer);
                map["deviceAddress"] = QString::fromLatin1(buffer);
                map["deviceClass"] = QString::number(
                        bt_rdev_get_device_class(nextRemoteDevice, BT_COD_DEVICECLASS));
                map["deviceType"] = tr("Proximity Devices");
                qDebug() << "YYYY DeviceListing::listPairedDevices() - address="
                        << QString::fromLatin1(buffer);
                _model->insert(map);
            } else {
                qDebug() << "YYYY DeviceListing::listPairedDevices() - not a proximity device";
            }
        }
        qDebug() << "YYYY DeviceListing::listPairedDevices() - freeing buffer";
        bt_rdev_free_array(remoteDeviceArray);
    }
    qDebug() << "YYYY DeviceListing::listPairedDevices() - returning";
}

bool DeviceListing::isProximityDevice(bt_remote_device_t * remoteDevice)
{

    qDebug() << "YYYY DeviceListing::isProximityDevice() - entering";

    bool foundProximityDevice = false;

    if (!remoteDevice) {
        qDebug() << "YYYY DeviceListing::isProximityDevice() - no devices - exiting";
        return foundProximityDevice;
    }
    qDebug() << "YYYY DeviceListing::isProximityDevice() - checking device type";

    const int deviceType = bt_rdev_get_type(remoteDevice);

    int proximity_related_services_count = 0;

    qDebug() << "YYYY DeviceListing::isProximityDevice() - deviceType=" << deviceType;
    if ((deviceType == BT_DEVICE_TYPE_LE_PUBLIC) || (deviceType == BT_DEVICE_TYPE_LE_PRIVATE)) {
        qDebug()
                << "YYYY DeviceListing::isProximityDevice() - BT_DEVICE_TYPE_LE_PUBLIC or  BT_DEVICE_TYPE_LE_PRIVATE";
        char **servicesArray = bt_rdev_get_services_gatt(remoteDevice);
        qDebug() << "YYYY DeviceListing::isProximityDevice() - checking services";
        if (servicesArray) {
            qDebug() << "YYYY DeviceListing::isProximityDevice() - found services";
            for (int i = 0; servicesArray[i]; i++) {
                qDebug() << "YYYY DeviceListing::isProximityDevice() - Checking type = "
                        << servicesArray[i];
                if (strcasecmp(servicesArray[i], IMMEDIATE_ALERT_SERVICE_UUID.toAscii().constData())
                        == 0) {
                    qDebug()
                            << "YYYY DeviceListing::isProximityDevice() - immediate alert service found";
                    proximity_related_services_count++;
                }
                if (strcasecmp(servicesArray[i], TX_POWER_SERVICE_UUID.toAscii().constData())
                        == 0) {
                    qDebug() << "YYYY DeviceListing::isProximityDevice() - TX power service found";
                    proximity_related_services_count++;
                }
                if (strcasecmp(servicesArray[i], LINK_LOSS_SERVICE_UUID.toAscii().constData())
                        == 0) {
                    qDebug() << "YYYY DeviceListing::isProximityDevice() - link loss service found";
                    proximity_related_services_count++;
                }
            }
            qDebug() << "YYYY DeviceListing::isProximityDevice() - freeing buffer";
            bt_rdev_free_services(servicesArray);
        } else {
            qDebug() << "YYYY DeviceListing::isProximityDevice() - no services found";

        }
    } else {
        qDebug()
                << "YYYY DeviceListing::isProximityDevice() - not BT_DEVICE_TYPE_LE_PUBLIC nor BT_DEVICE_TYPE_LE_PRIVATE";
    }
    qDebug() << "YYYY DeviceListing::isProximityDevice() - returning " << foundProximityDevice;

    if (proximity_related_services_count == 3) {
        foundProximityDevice = true;
    }

    return foundProximityDevice;
}

bb::cascades::DataModel* DeviceListing::model() const
{
    return _model;
}
