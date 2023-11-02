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

#define WIDTH 80
#define HEIGHT 25
#define ROW_SIZE (WIDTH * 2)
#define SCR_SIZE (ROW_SIZE * HEIGHT)

#define ASCII_LF 0xa

static u32 screen;          /* screen start location memory address     */
static u32 cursor;          /* current cursor location memory address   */
static u32 x, y;            /* cursor coordinate on the current screen  */

/* set screen start location */
static void set_screen(){
    outb(CRTC_ADDR_REG, START_ADDR_H);
    outb(CRTC_DATA_REG, (screen >> 8) & 0xff);
    outb(CRTC_ADDR_REG, START_ADDR_L);
    outb(CRTC_DATA_REG, screen & 0xff);
}

/* set cursor location */
static void set_cursor(){
    outb(CRTC_ADDR_REG, CURSOR_LOC_H);
    outb(CRTC_DATA_REG, (cursor >> 8) & 0xff);
    outb(CRTC_ADDR_REG, CURSOR_LOC_L);
    outb(CRTC_DATA_REG, cursor & 0xff);
}

/* clear screen */
static void console_clear(){
    u16* ptr = (u16*)(MEM_BASE + (screen * 2));
    while (ptr < (u16*)(MEM_BASE + SCREEN_SIZE + screen * 2)){
        *ptr++ = 0x0720;     /* 0x20 (space) 0x07 (black, light gray) */
    }
}

/* initial screen */
void console_init(){
    screen = 0;
    cursor = 0;
    x = 0;
    y = 0;
    set_screen();
    set_cursor();
    console_clear();
}

/* verify if screen or cursor moving needed */
static void console_validation(){
    if (x == WIDTH){
        x = 0;
        y++;
    }
    if (y == HEIGHT){
        screen += WIDTH;
        set_screen();
        y--;
    }
}

/* new line operation */
static void command_lf(){
    cursor += WIDTH - x;
    set_cursor();
    x = 0;
    y++;
    console_validation();
}

/* print string buf to specific location */
void console_write(u8 *buf){
    u8* ptr = buf;
    while(*ptr){
        switch (*ptr) {
            case ASCII_LF:
                command_lf();
                break;
            default:
                *(char*)(MEM_BASE+cursor*2) = *ptr;
                x++;
                cursor++;
                set_cursor();
                console_validation();
                break;
        }
        ptr++;

    }

}