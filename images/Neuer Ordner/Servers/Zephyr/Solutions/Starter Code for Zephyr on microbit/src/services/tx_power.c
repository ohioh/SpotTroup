
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

#include <services/tx_power.h>

#define BT_UUID_TX_POWER BT_UUID_DECLARE_16(0x1804)
#define BT_UUID_TX_POWER_LEVEL BT_UUID_DECLARE_16(0x2A07)

// the Zephyr controller does not currently support the reporting of power levels so unfortunately, we're left to hard code a value here
s8_t tx_power_level = -55;

/* TX Power Service Declaration */
static struct bt_gatt_attr attrs[] = {
		BT_GATT_PRIMARY_SERVICE(BT_UUID_TX_POWER),
		BT_GATT_CHARACTERISTIC(BT_UUID_TX_POWER_LEVEL, BT_GATT_CHRC_READ,
													 BT_GATT_PERM_READ, read_tx_power_level, NULL, NULL),
};

static struct bt_gatt_service tx_power_svc = BT_GATT_SERVICE(attrs);

void tx_power_init()
{
	bt_gatt_service_register(&tx_power_svc);
}

ssize_t read_tx_power_level(struct bt_conn *conn,
														const struct bt_gatt_attr *attr, void *buf,
														u16_t len, u16_t offset)
{
	printk("read_tx_power(%d)\n", tx_power_level);

	return bt_gatt_attr_read(conn, attr, buf, len, offset, &tx_power_level, sizeof(tx_power_level));
}

u8_t get_tx_power_level()
{
	return tx_power_level;
}