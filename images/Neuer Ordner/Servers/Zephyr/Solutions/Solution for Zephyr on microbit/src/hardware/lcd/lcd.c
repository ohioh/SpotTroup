#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <misc/byteorder.h>
#include <zephyr.h>
#include <board.h>
#include <uart.h>
#include <display/mb_display.h>

#include <hardware/lcd/lcd.h>

#define UART_DEVICE_NAME DT_UART_0_NAME
#define TEENY_DELAY 50
static char uart_buffer[20] = {
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
};

u8_t buffer_length = 0;

void lcd_init()
{
}

void uart_tx(void)
{
	int i;
	struct device *uart_dev = device_get_binding(UART_DEVICE_NAME);
	for (i = 0; i < buffer_length; i++)
	{
		uart_poll_out(uart_dev, uart_buffer[i]);
	}
	k_sleep(TEENY_DELAY);
}

void move_lcd_cursor(u8_t row, u8_t col)
{
	uart_buffer[0] = 254;
	uart_buffer[1] = 128;
	if (row == 1)
	{
		uart_buffer[1] += 64;
	}
	uart_buffer[1] = uart_buffer[1] + col;
	buffer_length = 2;
	uart_tx();
}

void clear_lcd()
{
	uart_buffer[0] = 254;
	uart_buffer[1] = 1;
	buffer_length = 2;
	uart_tx();
}

void backlight()
{
	uart_buffer[0] = 124;
	uart_buffer[1] = 157;
	buffer_length = 2;
	uart_tx();
}

void write_string(const char *message, u8_t length, u8_t row, u8_t col)
{
	move_lcd_cursor(row, col);
	strncpy(uart_buffer, message, length);
	buffer_length = length;
	uart_tx();
}