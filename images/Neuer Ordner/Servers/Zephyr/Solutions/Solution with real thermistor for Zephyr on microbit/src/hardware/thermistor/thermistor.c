#include <zephyr.h>
#include <zephyr/types.h>
#include <string.h>
#include <gpio.h>
#include <hardware/thermistor/thermistor.h>

// ADC

#include <adc.h>
#include <hal/nrf_adc.h>

#define ADC_DEVICE_NAME DT_ADC_0_NAME
#define ADC_RESOLUTION 10
#define ADC_GAIN ADC_GAIN_1_3
#define ADC_REFERENCE ADC_REF_INTERNAL
#define ADC_ACQUISITION_TIME ADC_ACQ_TIME_DEFAULT
#define ADC_1ST_CHANNEL_ID 0
#define ADC_1ST_CHANNEL_INPUT NRF_ADC_CONFIG_INPUT_3

#define BUFFER_SIZE 1
static u16_t m_sample_buffer[BUFFER_SIZE];

struct device *adc_dev;
static const struct adc_channel_cfg m_1st_channel_cfg = {
		.gain = ADC_GAIN,
		.reference = ADC_REFERENCE,
		.acquisition_time = ADC_ACQUISITION_TIME,
		.channel_id = ADC_1ST_CHANNEL_ID,
		.input_positive = ADC_1ST_CHANNEL_INPUT,
};

u16_t sim_adc = 151;
int sim_delta = 1;

static struct device *setup_adc(void)
{
	printk("setup_adc\n");
	int ret;
	struct device *the_adc_dev = device_get_binding(ADC_DEVICE_NAME);

	ret = adc_channel_setup(the_adc_dev, &m_1st_channel_cfg);

	(void)memset(m_sample_buffer, 0, sizeof(m_sample_buffer));

	return the_adc_dev;
}

void thermistor_init()
{
	sim_adc = 151;
	sim_delta = 1;
	// ADC
	adc_dev = setup_adc();
}

float readTemperature()
{
	// u16_t adc_reading = simulate_sensor_reading();
	u16_t adc_reading = get_sensor_reading();

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

u16_t get_sensor_reading()
{
	int ret;

	static struct adc_sequence_options options = {
			.callback = NULL,
	};

	const struct adc_sequence sequence = {
			.options = &options,
			.channels = BIT(ADC_1ST_CHANNEL_ID),
			.buffer = m_sample_buffer,
			.buffer_size = sizeof(m_sample_buffer),
			.resolution = ADC_RESOLUTION,
	};

	if (!adc_dev)
	{
		printk("ERROR: no ADC device!!\n");
		return -1;
	}

	ret = adc_read(adc_dev, &sequence);

	if (ret == 0)
	{
		return m_sample_buffer[0];
	}
	printk("ERROR reading from ADC %d\n", ret);
	return 0;
}
