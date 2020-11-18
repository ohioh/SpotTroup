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

// TODO define advertising data

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

//TODO implement connected state change callback function

//TODO implement disconnected state change callback function

//TODO define connection state change callbacks

//TODO implement callback function for post Bluetooth stack initialisation

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

	//TODO enable the Bluetooth stack

	//TODO register connection state change callback functions

	lcd_init();
	led_init();
	buzzer_init();
	thermistor_init();

	clear_lcd();
	write_string("ready", 5, 0, 0);

	// io_test();
}
