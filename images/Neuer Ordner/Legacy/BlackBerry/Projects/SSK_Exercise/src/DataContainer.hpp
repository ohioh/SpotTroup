
#ifndef DATACONTAINER
#define DATACONTAINER
#include <QMap>
#include <QQueue>
#include <QObject>
#include <QVariant>
#include <QString>
#include <btapi/btgatt.h>
#include <btapi/btdevice.h>

class DataContainer: public QObject {
	Q_OBJECT

private:
	DataContainer();
	static DataContainer* _instance;
	DataContainer* _dc;
	unsigned long getSysTimeMs();
	static QString KEY_DEVICE_NAME;
	static QString KEY_DEVICE_ADDR;
	static QString KEY_MIN_RSSI_NEAR;
    static QString KEY_MIN_RSSI_MEDIUM;
    static QString KEY_RSSI_FLUCTUATION_SENSITIVITY;

public:
	static DataContainer* getInstance();
	QString _current_device_name;
	QString _current_device_addr;
	bt_remote_device_t* _current_device;
	bool _device_connected;
	uint16_t _immediate_alert_level_handle;
	uint16_t _immediate_alert_level_value_handle;
    uint16_t _link_loss_alert_level_handle;
    uint16_t _link_loss_alert_level_value_handle;
    uint16_t _tx_power_level_handle;
    uint16_t _tx_power_level_value_handle;
    uint16_t _client_proximity_handle;
    uint16_t _client_proximity_value_handle;
	int _alert_service_instance;
    int _link_loss_service_instance;
    int _tx_power_service_instance;
    int _proximity_monitoring_service_instance;
	QMap<QString, QVariant> _proximity_device;
	QList<QMap<QString, QVariant> > _list_of_proximity_devices;
	void addProximityDevice(char* device_name, char* device_addr, int device_class, int device_type, bool paired, bool encrypted);
	QList<QVariantList> getDeviceIdList();
	QList<QVariantMap> getDeviceList();
	int _tx_power_level_dbm;
	int _rssi;
	int _proximity_indicator;
	int _min_rssi_near;
	int _min_rssi_medium;
	int _rssi_fluctuation_sensitivity;

	bool isDeviceConnected();
	void setDeviceConnected(bool state);

	void setImmediateAlertLevelHandle(uint16_t handle);
	uint16_t getImmediateAlertLevelHandle();
	void setImmediateAlertLevelValueHandle(uint16_t handle);
	uint16_t getImmediateAlertLevelValueHandle();

	void setLinkLossAlertLevelHandle(uint16_t handle);
    uint16_t getLinkLossAlertLevelHandle();
    void setLinkLossAlertLevelValueHandle(uint16_t handle);
    uint16_t getLinkLossAlertLevelValueHandle();

    void setTxPowerLevelHandle(uint16_t handle);
    uint16_t getTxPowerLevelHandle();
    void setTxPowerLevelValueHandle(uint16_t handle);
    uint16_t getTxPowerLevelValueHandle();

    void setClientProximityHandle(uint16_t handle);
    uint16_t getClientProximityHandle();
    void setClientProximityValueHandle(uint16_t handle);
    uint16_t getClientProximityValueHandle();

    void setAlertServiceInstance(int instance);
	int getAlertServiceInstance();

	void setLinkLossServiceInstance(int instance);
    int getLinkLossServiceInstance();

    void setTxPowerServiceInstance(int instance);
    int getTxPowerServiceInstance();

    void setProximityMonitoringServiceInstance(int instance);
    int getProximityMonitoringServiceInstance();

    void setTxPowerLevel(int level);
    int getTxPowerLevel();

    void setRssi(int level);
    int getRssi();

    void setProximityIndicator(int proximity_indicator);
    int getProximityIndicator();

    void setMinRssiNear(int rssi_threshold);
    int getMinRssiNear();

    void setMinRssiMedium(int rssi_threshold);
    int getMinRssiMedium();

    void setRssiFluctuationSensitivity(int sensitivity);
    int getRssiFluctuationSensitivity();

	QString intToHex(int decimal);

// device data
	void clearDeviceList();
	Q_INVOKABLE	QString getCurrentDeviceName();
	Q_INVOKABLE	QString getCurrentDeviceAddr();
	Q_INVOKABLE	void setCurrentDeviceName(QString device_name);
	Q_INVOKABLE	void setCurrentDeviceAddr(QString device_addr);

	bt_remote_device_t* getCurrentDevice();
	void setCurrentDevice(bt_remote_device_t* device);

};

#endif // ifndef DATACONTAINER
