
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

#include <services/link_loss.h>
#include <hardware/led/led.h>

#define BT_UUID_LINK_LOSS BT_UUID_DECLARE_16(0x1803)
#define BT_UUID_ALERT_LEVEL BT_UUID_DECLARE_16(0x2A06)

u8_t link_loss_alert_level = 0;

/* TODO Link Loss Service Declaration */

void link_loss_init()
{
	// TODO register service
}

ssize_t read_link_loss_alert_level(struct bt_conn *conn,
																	 const struct bt_gatt_attr *attr, void *buf,
																	 u16_t len, u16_t offset)
{
	// TODO implement handling of reading the Alert Level characteristic
	return 1;
}

ssize_t write_link_loss_alert_level(struct bt_conn *conn,
																		const struct bt_gatt_attr *attr,
																		const void *buf, u16_t len, u16_t offset,
																		u8_t flags)
{
	// TODO implement handling of writing the Alert Level characteristic
	return len;
}

u8_t get_link_loss_alert_level()
{
	return link_loss_alert_level;
}