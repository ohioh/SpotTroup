
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

#include <services/health_thermometer.h>
#include <hardware/thermistor/thermistor.h>

#define BT_UUID_HEALTH_THERMOMETER BT_UUID_DECLARE_16(0x1809)
#define BT_UUID_TEMPERATURE_MEASUREMENT BT_UUID_DECLARE_16(0x2A1C)

unsigned char temperature_measurement[5] = {0, 0, 0, 0, 0};
u8_t flags;
float celsius;
int celsius_times_10;

static struct bt_gatt_ccc_cfg tm_ccc_cfg[BT_GATT_CCC_MAX] = {};
static struct bt_gatt_indicate_params indicate_params;

static u8_t temperature_subscribed;
struct bt_conn *the_conn;
void (*start_sampling_function)();

/* TODO Health Thermometer Service Declaration */
static struct bt_gatt_attr attrs[] = {
		BT_GATT_PRIMARY_SERVICE(BT_UUID_HEALTH_THERMOMETER),
		BT_GATT_CHARACTERISTIC(BT_UUID_TEMPERATURE_MEASUREMENT, BT_GATT_CHRC_INDICATE,
													 BT_GATT_PERM_NONE, NULL, NULL, NULL),
		BT_GATT_CCC(tm_ccc_cfg, tm_ccc_cfg_changed),
};

static struct bt_gatt_service health_thermometer_svc = BT_GATT_SERVICE(attrs);

void health_thermometer_init(void (*start_sampling)())
{
	flags = 0x00;
	bt_gatt_service_register(&health_thermometer_svc);
	start_sampling_function = start_sampling;
}

void set_connection(struct bt_conn *conn) {
	the_conn = conn;
}

void tm_ccc_cfg_changed(const struct bt_gatt_attr *attr, u16_t value)
{
	temperature_subscribed = (value == BT_GATT_CCC_INDICATE) ? 1 : 0;
	printk("temperature_subscribed set to %d\n", temperature_subscribed);
	if (temperature_subscribed == 1) {
		(*start_sampling_function)();
	}
}

static void indicate_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr,
												u8_t err)
{

	if (err != 0)
	{
		printk("Indication failed: %d\n", err);
	}
}

void indicate_temperature_measurement_celsius(int celsius)
{

	celsius_times_10 = celsius * 10;
	// all our values are to one decimal place so we're sending the mantissa as 10 x celsius value with an exponent of -1
	// temperature_measurement[0] is the FLAGS field and already contains the required value of 0x00

	// little endian
	temperature_measurement[4] = 0xFF; // exponent of -1
	temperature_measurement[3] = (celsius_times_10 >> 16);
	temperature_measurement[2] = (celsius_times_10 >> 8) & 0xFF;
	temperature_measurement[1] = celsius_times_10 & 0xFF;

	indicate_params.attr = &attrs[2];
	indicate_params.data = &temperature_measurement;
	indicate_params.len = 5;
	indicate_params.func = indicate_cb;
	bt_gatt_indicate(the_conn, &indicate_params);
}

u8_t get_temperature_subscribed()
{
	return temperature_subscribed;
}