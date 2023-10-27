#include "../include/asm/io.h"

/* The CRT Controller (CRTC) Registers are accessed via a pair of registers. */
#define CRTC_ADDR_REG 0x3d4     /* CRTC Address Register */
#define CRTC_DATA_REG 0x3d5     /* CRTC Data Register */

#define START_ADDR_H 0xc        /* Start Address High Register (15-8 bit) */
#define START_ADDR_L 0xd        /* Start Address Low Register (7-0 bit) */

#define CURSOR_LOC_H 0xe        /* Cursor Location High Register (15-8 bit) */
#define CURSOR_LOC_L 0xf        /* Cursor Location Low Register 7-0 bit) */


#define MEM_BASE 0xb8000
#define SCREEN_SIZE 4000
#define MEM_END (MEM_BASE+SCREEN_SIZE)

#define SCREEN_INIT 0
#define CURSOR_INIT 0

static u16 screen;
static u16 cursor;

/* set screen start location */
static void set_screen(u16 screen){
    outb(CRTC_ADDR_REG, START_ADDR_H);
    outb(CRTC_DATA_REG, screen >> 8);
    outb(CRTC_ADDR_REG, START_ADDR_L);
    outb(CRTC_DATA_REG, screen);
}

/* set cursor location */
static void set_cursor(u16 cursor){
    outb(CRTC_ADDR_REG, CURSOR_LOC_H);
    outb(CRTC_DATA_REG, cursor >> 8);
    outb(CRTC_ADDR_REG, CURSOR_LOC_L);
    outb(CRTC_DATA_REG, cursor);
}

/* print string buf to specific location */
void console_write(u8 *buf, u32 count, u16 pos){
    u8* ptr = (u8*)(pos + MEM_BASE);
    while(count--){
	*ptr = *buf;
	ptr += 2;
	buf++;
    }
}

/* print CLI prompt */
void write_cmd_prompt(u16 pos){
   console_write(" yijun >> ", 10, pos);
   set_cursor(10);
}

/* clear screen */
void console_clear(){
    screen = 0;
    cursor = 0;
    set_screen(screen);
    set_cursor(cursor);
    u16* ptr = (u16*)MEM_BASE;
    while (ptr < (u16*)MEM_END){
        *ptr = 0x720;
        ptr++;
    }
    write_cmd_prompt(cursor);
}

/* initial screen */
void console_init(){
    console_clear();
}
