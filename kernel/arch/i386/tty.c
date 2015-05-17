#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/vga.h>

#define __SCROLL(H) if( ++terminal_row == H ) \
				 terminal_scroll()

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

static uint8_t terminal_setcolor(uint8_t color) {
	terminal_color = color;
	return terminal_color;
}

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_setcolor(make_color(COLOR_LIGHT_GREY, COLOR_BLACK));
	terminal_buffer = VGA_MEMORY;
	for ( size_t y = 0; y < VGA_HEIGHT; y++ ) {
		for ( size_t x = 0; x < VGA_WIDTH; x++ ) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}

static void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}

/*
 * Move each character moved VGA_WIDTH positions up in the buffer.
 * This effectively scrolls the whole buffer 1 line up.
 * Clear the last line.
 */
static void terminal_scroll() {
	for (size_t src = VGA_WIDTH; src < VGA_WIDTH*VGA_HEIGHT; src++)
		terminal_buffer[src-VGA_WIDTH] = terminal_buffer[src];
	for (size_t x = 0; x < VGA_WIDTH; x++)
		terminal_putentryat(' ', terminal_color, x, VGA_HEIGHT-1);
	terminal_row--;
}

void terminal_putchar(char c) {
	if (c == '\n') {
		terminal_column = 0;
		__SCROLL(VGA_HEIGHT);
		return;
	}
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if ( ++terminal_column == VGA_WIDTH ) {
		terminal_column = 0;
		__SCROLL(VGA_HEIGHT);
	}
}

void terminal_write(const char* data, size_t size) {
	for ( size_t i = 0; i < size; i++ )
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}
