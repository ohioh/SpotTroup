/* main.c - Application main entry point */

/*
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <misc/printk.h>
#include <misc/byteorder.h>
#include <zephyr.h>
#include <board.h>
#include <device.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

#include <hardware/lcd/lcd.h>
#include <hardware/led/led.h>
#include <hardware/buzzer/buzzer.h>
#include <hardware/thermistor/thermistor.h>
#include <services/link_loss.h>
#include <services/immediate_alert.h>
#include <services/tx_power.h>
#include <services/proximity_monitoring.h>
#include <services/health_thermometer.h>

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

struct bt_conn *default_conn;
int is_sampling = 0;

struct k_delayed_work temperature_timer;

static const struct bt_data ad[] = {
		BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
		BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

static void sample_temperature()
{
	if (is_sampling == 1 && get_temperature_subscribed() == 1) {
		int celsius = (int)readTemperature();
		s64_t now = k_uptime_get();
		printk("%lld> temperature: %d degrees C\n", now, celsius);
		indicate_temperature_measurement_celsius(celsius);

		// and again in one second from now
		if (is_sampling == 1) {
			k_delayed_work_submit(&temperature_timer, K_SECONDS(1));
		}
	}
}

static void start_sampling_temperature()
{
	printk("starting temperature sampling\n");
	is_sampling = 1;
    k_delayed_work_init(&temperature_timer, sample_temperature);
	k_delayed_work_submit(&temperature_timer, K_SECONDS(1));
}

static void stop_sampling_temperature()
{
	printk("stopping temperature sampling\n");
	is_sampling = 0;
}

static void connected(struct bt_conn *conn, u8_t err)
{
	if (err)
	{
		printk("Connection failed (err %u)\n", err);
	}
	else
	{
		default_conn = bt_conn_ref(conn);
		set_connection(conn);
		write_string("connected       ", 16, 0, 0);
	}
}

static void disconnected(struct bt_conn *conn, u8_t reason)
{
	printk("Disconnected (reason %u)\n", reason);
	printk("Disconnected (alert_level %d)\n", get_link_loss_alert_level());

	write_string("disconnected    ", 16, 0, 0);
	set_led_states(false, false, false);

	stop_sampling_temperature();

	if (get_link_loss_alert_level() > 0)
	{
		// handle link loss per the alert level
		flash(true, true, true, get_link_loss_alert_level() * 15);
		buzz(get_link_loss_alert_level() * 15);
	}

	if (default_conn)
	{
		bt_conn_unref(default_conn);
		default_conn = NULL;
	}
}

static struct bt_conn_cb conn_callbacks = {
		.connected = connected,
		.disconnected = disconnected,
};

static void bt_ready(int err)
{
	if (err)
	{
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	link_loss_init();
	immediate_alert_init();
	tx_power_init();
    void (*f_ptr)() = &start_sampling_temperature; 
	health_thermometer_init(f_ptr);
	proximity_monitoring_init();

	err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), NULL, 0);
	if (err)
	{
		printk("Advertising failed to start (err %d)\n", err);
		return;
	}

	printk("Advertising successfully started\n");
}

static void io_test()
{
	flash(true, false, false, 2);
	k_sleep(2000);
	flash(false, true, false, 2);
	k_sleep(2000);
	flash(false, false, true, 2);
	k_sleep(2000);
	buzz(3);
	backlight();
	clear_lcd();
	write_string("hello", 5, 0, 0);
	write_string("martin", 6, 1, 0);
}

void main(void)
{

	printk("starting\n");

	int err;

	err = bt_enable(bt_ready);
	if (err)
	{
		return;
	}

	bt_conn_cb_register(&conn_callbacks);

	lcd_init();
	led_init();
	buzzer_init();
	thermistor_init();

	clear_lcd();
	write_string("ready", 5, 0, 0);

	// io_test();
}
