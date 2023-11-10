#include "../include/asm/io.h"
#include "../include/asm/system.h"
#include "../include/linux/kernel.h"

#define INV 0
#define true 1
#define false 0

/* 
 * Each keymap member has 4 value:
 * un-shifted value, shifted value
 * ctrl status (unused), alt status (unused)
*/
static char keymap[][4] = {
        /* 0x00 */ {INV, INV, false, false},
        /* 0x01 */ {INV, INV, false, false},
        /* 0x02 */ {'1', '!', false, false},
        /* 0x03 */ {'2', '@', false, false},
        /* 0x04 */ {'3', '#', false, false},
        /* 0x05 */ {'4', '$', false, false},
        /* 0x06 */ {'5', '%', false, false},
        /* 0x07 */ {'6', '^', false, false},
        /* 0x08 */ {'7', '&', false, false},
        /* 0x09 */ {'8', '*', false, false},
        /* 0x0A */ {'9', '(', false, false},
        /* 0x0B */ {'0', ')', false, false},
        /* 0x0C */ {'-', '_', false, false},
        /* 0x0D */ {'=', '+', false, false},
        /* 0x0E */ {'\b', '\b', false, false},
        /* 0x0F */ {'\t', '\t', false, false},
        /* 0x10 */ {'q', 'Q', false, false},
        /* 0x11 */ {'w', 'W', false, false},
        /* 0x12 */ {'e', 'E', false, false},
        /* 0x13 */ {'r', 'R', false, false},
        /* 0x14 */ {'t', 'T', false, false},
        /* 0x15 */ {'y', 'Y', false, false},
        /* 0x16 */ {'u', 'U', false, false},
        /* 0x17 */ {'i', 'I', false, false},
        /* 0x18 */ {'o', 'O', false, false},
        /* 0x19 */ {'p', 'P', false, false},
        /* 0x1A */ {'[', '{', false, false},
        /* 0x1B */ {']', '}', false, false},
        /* 0x1C */ {'\n', '\n', false, false},
        /* 0x1D */ {INV, INV, false, false},
        /* 0x1E */ {'a', 'A', false, false},
        /* 0x1F */ {'s', 'S', false, false},
        /* 0x20 */ {'d', 'D', false, false},
        /* 0x21 */ {'f', 'F', false, false},
        /* 0x22 */ {'g', 'G', false, false},
        /* 0x23 */ {'h', 'H', false, false},
        /* 0x24 */ {'j', 'J', false, false},
        /* 0x25 */ {'k', 'K', false, false},
        /* 0x26 */ {'l', 'L', false, false},
        /* 0x27 */ {';', ':', false, false},
        /* 0x28 */ {'\'', '\"', false, false},
        /* 0x29 */ {'`', '~', false, false},
        /* 0x2A */ {INV, INV, false, false},
        /* 0x2B */ {'\\', '|', false, false},
        /* 0x2C */ {'z', 'Z', false, false},
        /* 0x2D */ {'x', 'X', false, false},
        /* 0x2E */ {'c', 'C', false, false},
        /* 0x2F */ {'v', 'V', false, false},
        /* 0x30 */ {'b', 'B', false, false},
        /* 0x31 */ {'n', 'N', false, false},
        /* 0x32 */ {'m', 'M', false, false},
        /* 0x33 */ {',', '<', false, false},
        /* 0x34 */ {'.', '>', false, false},
        /* 0x35 */ {'/', '?', false, false},
        /* 0x36 */ {INV, INV, false, false},
        /* 0x37 */ {'*', '*', false, false},
        /* 0x38 */ {INV, INV, false, false},
        /* 0x39 */ {' ', ' ', false, false},
        /* 0x3A */ {INV, INV, false, false},
        /* 0x3B */ {INV, INV, false, false},
        /* 0x3C */ {INV, INV, false, false},
        /* 0x3D */ {INV, INV, false, false},
        /* 0x3E */ {INV, INV, false, false},
        /* 0x3F */ {INV, INV, false, false},
        /* 0x40 */ {INV, INV, false, false},
        /* 0x41 */ {INV, INV, false, false},
        /* 0x42 */ {INV, INV, false, false}, 
        /* 0x43 */ {INV, INV, false, false},
        /* 0x44 */ {INV, INV, false, false},
        /* 0x45 */ {INV, INV, false, false},
        /* 0x46 */ {INV, INV, false, false},
        /* 0x47 */ {'7', INV, false, false},
        /* 0x48 */ {'8', INV, false, false},
        /* 0x49 */ {'9', INV, false, false},
        /* 0x4A */ {'-', '-', false, false},
        /* 0x4B */ {'4', INV, false, false},
        /* 0x4C */ {'5', INV, false, false},
        /* 0x4D */ {'6', INV, false, false},
        /* 0x4E */ {'+', '+', false, false},
        /* 0x4F */ {'1', INV, false, false},
        /* 0x50 */ {'2', INV, false, false},
        /* 0x51 */ {'3', INV, false, false},
        /* 0x52 */ {'0', INV, false, false},
        /* 0x53 */ {'.', 0x7F, false, false},
        /* 0x54 */ {INV, INV, false, false},
        /* 0x55 */ {INV, INV, false, false},
        /* 0x56 */ {INV, INV, false, false},
        /* 0x57 */ {INV, INV, false, false},
        /* 0x58 */ {INV, INV, false, false},
        /* 0x59 */ {INV, INV, false, false},
        /* 0x5A */ {INV, INV, false, false},
        /* 0x5B */ {INV, INV, false, false},
        /* 0x5C */ {INV, INV, false, false}, 
        /* 0x5D */ {INV, INV, false, false},
        /* 0x5E */ {INV, INV, false, false},
        /* 0x5F */ {INV, INV, false, false},
};

static unsigned char extcode = false;   /* extended code status */
static unsigned char shift = false;     /* shift status */

void keymap_handler(){
    outb(0x20, 0x20);      /* manually send EOI to PIC */

    unsigned char scan_code = inb(0x60);         /* get scan code */

    if (scan_code == 0xe0){                  /* extended key pressed */
        extcode = true;
        return;
    }

    if(extcode){                      /* extended key will jump into ISR twice */
        extcode = false;
        return;
    }

    if(scan_code == 0x2a){      /* left shift pressed */
        shift = true;
        return;
    }

    if(scan_code == 0xaa){          /* left shift released */
        shift = false;
        return;
    }

    if (scan_code & 0x80){                  /* ignore break code */
        return;
    }
    printk("%c", keymap[scan_code][shift]);     /* print make code */
}
