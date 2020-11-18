void link_loss_init();
ssize_t read_link_loss_alert_level(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, u16_t len, u16_t offset);
ssize_t write_link_loss_alert_level(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, u16_t len, u16_t offset, u8_t flags);
u8_t get_link_loss_alert_level();
