
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

void health_thermometer_init(void (*start_sampling)())
{
	flags = 0x00;
	start_sampling_function = start_sampling;
	// TODO register service
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

	// TODO formulate and transmit the temperature measurement indication
}

u8_t get_temperature_subscribed()
{
	return temperature_subscribed;
}