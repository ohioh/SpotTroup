void tx_power_init();
ssize_t read_tx_power_level(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, u16_t len, u16_t offset);
u8_t get_tx_power_level();
