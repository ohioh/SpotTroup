
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
static struct bt_gatt_attr attrs[] = {
		BT_GATT_PRIMARY_SERVICE(BT_UUID_IMMEDIATE_ALERT),
		BT_GATT_CHARACTERISTIC(BT_UUID_ALERT_LEVEL, BT_GATT_CHRC_WRITE_WITHOUT_RESP,
													 BT_GATT_PERM_WRITE, NULL, write_immediate_alert_alert_level, NULL),
};

static struct bt_gatt_service immediate_alert_svc = BT_GATT_SERVICE(attrs);

void immediate_alert_init()
{
	// TODO register service
	bt_gatt_service_register(&immediate_alert_svc);
}

ssize_t write_immediate_alert_alert_level(struct bt_conn *conn,
																					const struct bt_gatt_attr *attr,
																					const void *buf, u16_t len, u16_t offset,
																					u8_t flags)
{
	const u8_t *new_alert_level = buf;

	if (!len)
	{
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}

	if (*new_alert_level >= 0 && *new_alert_level <= 2)
	{
		immediate_alert_alert_level = *new_alert_level;
	}
	else
	{
		return BT_GATT_ERR(BT_ATT_ERR_VALUE_NOT_ALLOWED);
	}

	printk("write_immediate_alert_alert_level(%d)\n", immediate_alert_alert_level);

	switch (immediate_alert_alert_level)
	{
	case 0:
		flash(true, true, true, 3);
		break;
	case 1:
		flash(true, true, true, 4);
		buzz(4);
		break;
	case 2:
		flash(true, true, true, 5);
		buzz(5);
		break;
	}
	return len;
}

u8_t get_immediate_alert_alert_level()
{
	return immediate_alert_alert_level;
}