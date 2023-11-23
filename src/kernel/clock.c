#include "../include/asm/io.h"
#include "../include/linux/kernel.h"
#include "../include/linux/sched.h"

#define PIT_CH0_DATA 0x40   /* Channel 0 data port (read/write) */
#define PIT_CH1_DATA 0x41   /* Channel 1 data port (read/write) */
#define PIT_CH2_DATA 0x42   /* Channel 2 data port (read/write) */
#define PIT_CMD      0x43   /* Mode/Command register (write only) */

#define OSCILLATOR 1193182  /* PIT chip runs at 1.193182 MHz    */
#define FREQUENCY 20        /* expected frequency, measured in Hz */
#define COUNTER (OSCILLATOR / FREQUENCY)

void clock_init(){
    /*
     *  bit 6-7, 0b00,  channel 0
     *  bit 4-5, 0b11,  access mode: low byte / high byte
     *  bit 1-3, 0b010, rate generator
     *  bit 0,   0b0,   16-bit binary
     */
    outb(PIT_CMD, 0b00110100);
    outb(PIT_CH0_DATA, COUNTER & 0xff);  /* low byte  */
    outb(PIT_CH0_DATA, (COUNTER >> 8) & 0xff);  /* high byte */
}

void clock_handler(){
    sched();
}