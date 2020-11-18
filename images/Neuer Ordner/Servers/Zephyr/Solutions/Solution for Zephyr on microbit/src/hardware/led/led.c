
#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <misc/byteorder.h>
#include <zephyr.h>
#include <board.h>
#include <gpio.h>

#include <hardware/led/led.h>

struct device *gpio_dev;
static volatile char flash_led[3] = {0, 0, 0};
static volatile bool led_states[3] = {false, false, false};
static volatile int flash_control_count = 8;
static volatile bool flash_on;
static volatile bool flashing;
const u32_t led_pins[3] = {LED0, LED1, LED2};

static K_TIMER_DEFINE(flash_timer, flash_control, NULL);
static K_TIMER_DEFINE(state_timer, state_control, NULL);

void led_init()
{
	flashing = false;
	flash_on = true;
	gpio_dev = device_get_binding(PORT);
	gpio_pin_configure(gpio_dev, LED0, GPIO_DIR_OUT);
	gpio_pin_configure(gpio_dev, LED1, GPIO_DIR_OUT);
	gpio_pin_configure(gpio_dev, LED2, GPIO_DIR_OUT);
}

void flash_control(struct k_timer *timer_id)
{
	int i = 0;
	for (i = 0; i < 3; i++)
	{
		if (flash_led[i] == 1)
		{
			if (flash_on)
			{
				// // printk("LED %d on\r\n", i);
				gpio_pin_write(gpio_dev, led_pins[i], 1);
				led_states[i] = true;
			}
			else
			{
				// // printk("LED %d off\r\n", i);
				gpio_pin_write(gpio_dev, led_pins[i], 0);
				led_states[i] = false;
			}
		}
	}
	flash_on = !flash_on;
	flash_control_count--;
	if (flash_control_count == 0)
	{
		for (i = 0; i < 3; i++)
		{
			flash_led[i] = 0;
			led_states[i] = false;
		}
		flashing = false;
		// // printk("stopping flashing\n");
		k_timer_stop(&flash_timer);
	}
}

void flash(bool led0, bool led1, bool led2, u8_t times)
{
	if (flashing)
	{
		return;
	}
	flash_led[0] = (led0 == true) ? 1 : 0;
	flash_led[1] = (led1 == true) ? 1 : 0;
	flash_led[2] = (led2 == true) ? 1 : 0;
	flash_control_count = times * 2;
	flash_on = true;
	flashing = true;
	k_timer_start(&flash_timer, K_MSEC(0), K_MSEC(LED_SLEEP_TIME));
}

void state_control(struct k_timer *timer_id)
{
	int i = 0;
	for (i = 0; i < 3; i++)
	{
		if (led_states[i])
		{
			// printk("LED %d on\r\n", i);
			gpio_pin_write(gpio_dev, led_pins[i], 1);
		}
		else
		{
			// printk("LED %d off\r\n", i);
			gpio_pin_write(gpio_dev, led_pins[i], 0);
		}
	}
}

void set_led_states(bool led0, bool led1, bool led2)
{
	led_states[0] = led0;
	led_states[1] = led1;
	led_states[2] = led2;
	k_timer_start(&state_timer, K_MSEC(0), 0);
}

void set_led_state(int led, bool state)
{
	led_states[led] = state;
	k_timer_start(&state_timer, K_MSEC(0), 0);
}