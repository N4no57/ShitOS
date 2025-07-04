#include <ps2.h>
#include <terminal.h>
#include <stdio.h>

#define CIRCULAR_BUFFER_SIZE 256

// raw scancode buffer
unsigned char input_buff[CIRCULAR_BUFFER_SIZE];
unsigned char read_ptr = 0;
unsigned char write_ptr = 0;

// parsed input buffer
volatile unsigned char parsed_buff[CIRCULAR_BUFFER_SIZE];
volatile unsigned char parsed_read_ptr = 0;
volatile unsigned char parsed_write_ptr = 0;

// key event buffer for non-ascii keys
volatile unsigned char key_event[CIRCULAR_BUFFER_SIZE];
volatile unsigned char key_event_read_ptr = 0;
volatile unsigned char key_event_write_ptr = 0;

// lookup tables for parser
const unsigned char scancode_table[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b', // 0x0–0xE
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',     // 0xF–0x1C
    0,  'a','s','d','f','g','h','j','k','l',';','\'','`',          // 0x1D–0x29
    0,  '#','z','x','c','v','b','n','m',',','.','/',0,             // 0x2A–0x35
    '*', 0,  ' ', 0, 0, 0, 0, 0, 0, 0,                             // 0x36–0x3F
};

const unsigned char scancode_shift_table[128] = {
    0,  27, '!','"','£','$','%','^','&','*','(',')','_','+', '\b', // 0x0–0xE
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',     // 0xF–0x1C
    0,  'A','S','D','F','G','H','J','K','L',':','@','¬',           // 0x1D–0x29
    0,  '~','Z','X','C','V','B','N','M','<','>','?',0,             // 0x2A–0x35
    0, 0,  ' ', 0, 0, 0, 0, 0, 0, 0,                               // 0x36–0x3F
};

unsigned char input_avail() {
    return read_ptr != write_ptr;
}

unsigned char shift_pressed = 0;
unsigned char extended = 0;
unsigned char break_code = 0;

void parse_input_buffer() {
    while (input_avail()) {
        unsigned char scancode = input_buff[read_ptr++];

        // check if scancode is an extended scancode
        if (scancode == 0xE0) {
            extended = 1;
            continue;
        }

        // Handle key release
        if (!extended) {
            if (scancode == 0xAA || scancode == 0xB6) { // Left/Right shift
                shift_pressed = 0;
            }
        }

        // Handle key press
        if (extended) { // extended scan codes
            switch (scancode) {
                case 0x47: key_event[key_event_write_ptr++] = KEY_HOME; break;
                case 0x4F: key_event[key_event_write_ptr++] = KEY_END;  break;
                case 0x49: key_event[key_event_write_ptr++] = KEY_PGUP; break;
                case 0x51: key_event[key_event_write_ptr++] = KEY_PGDN; break;
                default: break; // unhandled
            }
            extended = 0;
            continue;
        }

        // Handle normal keys
        if (scancode == 0x2A || scancode == 0x36) { // Shift press
            shift_pressed = 1;
            continue;
        }

        if (scancode > 128) continue;

        unsigned char ascii = shift_pressed ? scancode_shift_table[scancode] : scancode_table[scancode];
        if (ascii) {
            parsed_buff[parsed_write_ptr++] = ascii;
        }
    }
}

// input raw scancode to an input buffer
void keyboard_interrupt_handler() {
    unsigned char scancode = inb(0x60);

    if (scancode == 0xE0) { // extended scancodes
        input_buff[write_ptr++] = scancode;
        scancode = inb(0x60);
        input_buff[write_ptr++] = scancode;
        return;
    }

    // normal scancodes
    input_buff[write_ptr++] = scancode;
}