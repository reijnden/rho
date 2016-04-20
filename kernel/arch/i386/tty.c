#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/vga.h>

/*
 * Macro for scrolling
 * If the number of rows reaches a certain maximum the scroll function is called
 */
#define __NEWLINE(H) terminal_column = 0; \
		     if( ++terminal_row == H ) \
		             terminal_scroll()

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

/*
 * Dubious function, I think we should remove this one, or rewrite this file.
 */
static uint8_t terminal_setcolor(uint8_t color) {
	terminal_color = color;
	return terminal_color;
}

/*
 * See http://wiki.osdev.org/Text_Mode_Cursor
 */
void upd_c() {
	unsigned short p = (terminal_row*VGA_WIDTH)+terminal_column;

	outportb(0x3D4,0x0F);
	outportb(0x3D5,(unsigned char)(p&0xFF));
	outportb(0x3D4,0x0E);
	outportb(0x3D5,(unsigned char)((p>>8)&0xFF));
}

/*
 * Prepare the terminal for output.
 */
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

/*
 * Place a character at given position in the buffer.
 */
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

/*
 * Write one character to the buffer
 */
void terminal_putchar(char c) {
	if (c == '\n') {
		__NEWLINE(VGA_HEIGHT);
		return;
	}
	/*
	 * Backspace means put a space on the previous position
	 * Unless that is a newline
	 */
	if (c == '\b') {
		if (terminal_column == 0) { /* beginning of line */
			terminal_column = VGA_WIDTH;
			terminal_row--;
		}
		terminal_column--;
		c = ' ';
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	} else {
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		if ( ++terminal_column == VGA_WIDTH ){
			__NEWLINE(VGA_HEIGHT);
		}
	}
}

/*
 * Write a string with given size to the buffer
 */
void terminal_write(const char* data, size_t size) {
	for ( size_t i = 0; i < size; i++ )
		terminal_putchar(data[i]);
}

/*
 * Write a null-terminated string to the buffer
 */
void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}
