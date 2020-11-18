#define THERMISTOR_SAMPLE_PERIOD 1000
void health_thermometer_init();
void indicate_temperature_measurement_celsius(int celsius);
void tm_ccc_cfg_changed(const struct bt_gatt_attr *attr, u16_t value);
u8_t get_temperature_subscribed();
void set_connection(struct bt_conn *conn);