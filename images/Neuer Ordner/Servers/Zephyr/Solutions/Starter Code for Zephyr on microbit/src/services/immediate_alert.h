void immediate_alert_init();
ssize_t write_immediate_alert_alert_level(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, u16_t len, u16_t offset, u8_t flags);
u8_t get_immediate_alert_alert_level();
