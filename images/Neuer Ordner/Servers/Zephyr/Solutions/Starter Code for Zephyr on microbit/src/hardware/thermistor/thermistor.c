#include <zephyr.h>
#include <zephyr/types.h>
#include <string.h>
#include <gpio.h>

#include <hardware/thermistor/thermistor.h>

u16_t sim_adc = 151;
int sim_delta = 1;

void thermistor_init()
{
	sim_adc = 151;
	sim_delta = 1;
}

float readTemperature()
{
	u16_t adc_reading = simulate_sensor_reading();

	// reference voltage is 1.2V and 3 x 1.2 is close enough to the micro:bit 3.3V. We used a gain (pre-scaling) of one third so we scale back up here.
	int mv = (int)adc_reading * 3600 / 1023;

	// 10 mV/°C scale factor
	// Spec for TMP36 says 750mv = 25 degrees celsius. So conversion to celsius from mV is mV / 10 - 50
	float celsius = (mv / 10) - 50;
	return celsius;
}

u16_t simulate_sensor_reading()
{
	sim_adc = sim_adc + sim_delta;
	if (sim_adc == 300)
	{
		sim_delta = -1;
	}
	else if (sim_adc == 150)
	{
		sim_delta = 1;
	}
	return sim_adc;
}
