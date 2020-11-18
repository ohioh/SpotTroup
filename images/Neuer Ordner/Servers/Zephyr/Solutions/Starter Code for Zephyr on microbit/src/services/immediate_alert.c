
#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <misc/printk.h>
#include <misc/byteorder.h>
#include <zephyr.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

#include <services/immediate_alert.h>
#include <hardware/led/led.h>
#include <hardware/buzzer/buzzer.h>

#define BT_UUID_IMMEDIATE_ALERT BT_UUID_DECLARE_16(0x1802)
#define BT_UUID_ALERT_LEVEL BT_UUID_DECLARE_16(0x2A06)

u8_t immediate_alert_alert_level = 0;

/* TODO Immediate Alert Service Declaration */

void immediate_alert_init()
{
	// TODO register service
}

ssize_t write_immediate_alert_alert_level(struct bt_conn *conn,
																					const struct bt_gatt_attr *attr,
																					const void *buf, u16_t len, u16_t offset,
																					u8_t flags)
{
	// TODO implement handling of write to the Alert Level characteristic
	return len;
}

u8_t get_immediate_alert_alert_level()
{
	return immediate_alert_alert_level;
}