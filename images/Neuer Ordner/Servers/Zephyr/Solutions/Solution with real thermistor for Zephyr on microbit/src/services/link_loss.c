
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

static struct bt_gatt_attr attrs[] = {
		BT_GATT_PRIMARY_SERVICE(BT_UUID_LINK_LOSS),
		BT_GATT_CHARACTERISTIC(BT_UUID_ALERT_LEVEL, BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE,
													 BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_link_loss_alert_level, write_link_loss_alert_level, NULL),
};

static struct bt_gatt_service link_loss_svc = BT_GATT_SERVICE(attrs);

void link_loss_init()
{
	// TODO register service
	bt_gatt_service_register(&link_loss_svc);
}

ssize_t read_link_loss_alert_level(struct bt_conn *conn,
																	 const struct bt_gatt_attr *attr, void *buf,
																	 u16_t len, u16_t offset)
{
	// TODO implement handling of reading the Alert Level characteristic
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &link_loss_alert_level, sizeof(link_loss_alert_level));
}

ssize_t write_link_loss_alert_level(struct bt_conn *conn,
																		const struct bt_gatt_attr *attr,
																		const void *buf, u16_t len, u16_t offset,
																		u8_t flags)
{
	printk("write_link_loss_alert_level\n");
	const u8_t *new_alert_level = buf;

	if (!len)
	{
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}

	if (*new_alert_level >= 0 && *new_alert_level <= 2)
	{
		link_loss_alert_level = *new_alert_level;
	}
	else
	{
		return BT_GATT_ERR(BT_ATT_ERR_VALUE_NOT_ALLOWED);
	}

	printk("write_link_loss_alert_level(%d)\n", link_loss_alert_level);
	switch (link_loss_alert_level)
	{
	case 0:
		flash(true, false, false, 4);
		break;
	case 1:
		flash(false, true, false, 4);
		break;
	case 2:
		flash(false, false, true, 4);
		break;
	}

	return len;
}

u8_t get_link_loss_alert_level()
{
	return link_loss_alert_level;
}