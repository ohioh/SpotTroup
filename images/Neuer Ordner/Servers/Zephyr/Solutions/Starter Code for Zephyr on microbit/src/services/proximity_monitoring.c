
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

void proximity_monitoring_init()
{
	//TODO register service
}

ssize_t write_client_proximity(struct bt_conn *conn,
															 const struct bt_gatt_attr *attr,
															 const void *buf, u16_t len, u16_t offset,
															 u8_t flags)
{
	//TODO implement handling of write to the client_proximity characteristic
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