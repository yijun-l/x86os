#ifndef _LINUX_TASK_H
#define _LINUX_TASK_H

#include "./types.h"

#define NR_TASKS 64     /* the maximum number of tasks */

/* process state */
typedef enum task_state_t{
    TASK_RUNNING,
    TASK_INTERRUPTIBLE,
    TASK_UNINTERRUPTIBLE,
    TASK_ZOMBIE,
    TASK_STOPPED,
    TASK_SWAPPING,
} task_state_t;

/* process executable code */
typedef void* (*task_func_t)(void*);

/* tss */
typedef struct tss_t {
    /* Previous Task Link Field. Contains the Segment Selector for the TSS of the previous task. */
    /* item 0 - 4 */
    u32 link;   /* u16 Reserved | u16 Link */
    u32 esp0;
    u32 ss0;    /* u16 Reserved | u16 SS0 */
    u32 esp1;
    u32 ss1;    /* u16 Reserved | u16 SS1 */
    /* item 5 - 9 */
    u32 esp2;
    u32 ss2;    /* u16 Reserved | u16 SS2 */
    u32 cr3;
    u32 eip;
    u32 flags;
    /* item 10 - 14 */
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 ebx;
    u32 esp;
    /* item 15 - 19 */
    u32 ebp;
    u32 esi;
    u32 edi;
    u32 es;     /* u16 Reserved | u16 ES */
    u32 cs;     /* u16 Reserved | u16 CS */
    /* item 20 - 24 */
    u32 ss;     /* u16 Reserved | u16 SS */
    u32 ds;     /* u16 Reserved | u16 DS */
    u32 fs;     /* u16 Reserved | u16 FS */
    u32 gs;     /* u16 Reserved | u16 GS */
    u32 ldtr;   /* u16 Reserved | u16 LDTR */
    /* item 25 - 26 */
    u16 trace : 1;
    u16 reversed : 15;
    /* I/O Map Base Address Field. Contains a 16-bit offset from the base of the TSS to the I/O Permission Bit Map. */
    u16 iopb;
    u32 ssp;     /* shadow stack pointer */
} __attribute__((packed)) tss_t;

/* process control block (pcb) */
typedef struct task_t{
    tss_t context;
    int pid;
    int ppid;
    char name[16];
    task_state_t state;
    task_func_t function;
    int esp0;
} __attribute((packed)) task_t;

task_t* create_task(char*, task_func_t);
task_func_t* get_task_function(task_t*);
void task_init();

#endif
