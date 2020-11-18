
#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <misc/byteorder.h>
#include <zephyr.h>
#include <board.h>
#include <pwm.h>

#include <hardware/buzzer/buzzer.h>

static K_TIMER_DEFINE(buzzer_timer, buzz_control, NULL);

struct device *pwm_dev;
static volatile bool buzzing;
static volatile bool buzzer_on;
static u32_t period = PERIOD_INIT;
static volatile int buzz_control_count = 6;

void buzzer_init()
{
	pwm_dev = device_get_binding(CONFIG_PWM_NRF5_SW_0_DEV_NAME);
}

void buzz_control(struct k_timer *timer_id)
{
	if (buzzer_on)
	{
		printk("buzz on\r\n");
		pwm_pin_set_usec(pwm_dev, BUZZER_PIN, period, period / 2);
	}
	else
	{
		printk("buzz off\r\n");
		pwm_pin_set_usec(pwm_dev, BUZZER_PIN, 0, 0);
	}
	buzz_control_count--;
	buzzer_on = !buzzer_on;
	if (buzz_control_count == 0)
	{
		buzzing = false;
		printk("stopping buzzing\n");
		k_timer_stop(&buzzer_timer);
	}
}

void buzz(u8_t times)
{
	if (buzzing)
	{
		return;
	}
	buzz_control_count = times * 2;
	buzzer_on = true;
	buzzing = true;
	k_timer_start(&buzzer_timer, K_MSEC(0), K_MSEC(BEEP_DURATION));
}