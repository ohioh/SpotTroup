
#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <misc/printk.h>
#include <misc/byteorder.h>
#include <zephyr.h>
#include <stdio.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

#include <services/proximity_monitoring.h>
#include <hardware/lcd/lcd.h>
#include <hardware/led/led.h>
#include <hardware/buzzer/buzzer.h>

#define BT_UUID_PROXIMITY_MONITORING BT_UUID_DECLARE_128( 0xFD, 0x1D, 0x6D, 0xFE, 0xD0, 0xAF, 0xBD, 0x93, 0xE4, 0x11, 0x3F, 0x29, 0x10, 0x99, 0x09,0x3E)
#define BT_UUID_CLIENT_PROXIMITY BT_UUID_DECLARE_128(0xFD, 0x1D, 0x6D, 0xFE, 0xD0, 0xAF, 0xBD, 0x93, 0xE4, 0x11, 0x3F, 0x29, 0x11, 0x99, 0x09, 0x3E, )

u8_t proximity_band = 0;
s8_t client_rssi = 0;

/* TODO Proximity Monitoring Service Declaration */
static struct bt_gatt_attr attrs[] = {
		BT_GATT_PRIMARY_SERVICE(BT_UUID_PROXIMITY_MONITORING),
		BT_GATT_CHARACTERISTIC(BT_UUID_CLIENT_PROXIMITY, BT_GATT_CHRC_WRITE_WITHOUT_RESP,
													 BT_GATT_PERM_WRITE, NULL, write_client_proximity, NULL),
};

static struct bt_gatt_service proximity_monitoring_svc = BT_GATT_SERVICE(attrs);

void proximity_monitoring_init()
{
	bt_gatt_service_register(&proximity_monitoring_svc);
}

ssize_t write_client_proximity(struct bt_conn *conn,
															 const struct bt_gatt_attr *attr,
															 const void *buf, u16_t len, u16_t offset,
															 u8_t flags)
{
	printk("write_client_proximity\n");
	const u8_t *new_proximity_data = buf;

	if (len != 2)
	{
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}

	proximity_band = new_proximity_data[0];
	client_rssi = (s8_t)new_proximity_data[1];

	printk("proximity_band: (%d)\n", proximity_band);
	printk("client-rssi: (%d)\n", client_rssi);

	char lcdbuf[16];

	switch (proximity_band)
	{
	case 0:
		set_led_states(false, false, false);
		clear_lcd();
		return len;
	case 1:
		set_led_states(true, false, false);
		break;
	case 2:
		set_led_states(false, true, false);
		break;
	case 3:
		set_led_states(false, false, true);
		break;
	}

	sprintf(lcdbuf, "Client RSSI: %d  ", client_rssi);
	write_string(lcdbuf, 16, 0, 0);

	return len;
}

u8_t get_proximity_band()
{
	return proximity_band;
}

s8_t get_client_rssi()
{
	return client_rssi;
}