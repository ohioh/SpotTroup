void proximity_monitoring_init();
ssize_t write_client_proximity(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, u16_t len, u16_t offset, u8_t flags);
u8_t get_proximity_band();
s8_t get_client_rssi();
