void lcd_init();
void move_lcd_cursor(u8_t row, u8_t col);
void clear_lcd();
void backlight();
void write_string(const char *message, u8_t length, u8_t row, u8_t col);
void uart_tx(void);