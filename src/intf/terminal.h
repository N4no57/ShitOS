#include <stddef.h>
#include <stdint.h>

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

#define VGA_WIDTH   80
#define VGA_HEIGHT  26
#define TRUE_VGA_HEIGHT 25
#define VGA_MEMORY  0xB8000

#define CURSOR_INDEX_REGISTER 0x3D4
#define CURSOR_DATA_REGISTER 0x3D5

#define CURSOR_LOW_BYTE 0x0F
#define CURSOR_HIGH_BYTE 0x0E

#define TAB_SIZE 4

#define SCROLLBACK_MAX_LINES 1024

void terminal_initialize(void);
void terminal_writestring(const char* data);
void terminal_write(const char* data, size_t size);
void terminal_putchar(char c);
void terminal_update_cursor(void);
void terminal_clear(void);

void scroll_to_line(uint16_t line);
void terminal_scroll_up(void);
void terminal_scroll_down(void);