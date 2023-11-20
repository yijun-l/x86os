#ifndef _LINUX_TASK_H
#define _LINUX_TASK_H

#define NR_TASKS 64     /* maximum number of processes */

/* process state */
typedef enum task_state_t{
    TASK_INIT,
    TASK_RUNNING,
    TASK_READY,
    TASK_BLOCKED,
    TASK_SLEEPING,
    TASK_WAITING,
    TASK_DIED,
} task_state_t;

/* process code */
typedef void* (*task_func_t)(void*);

/* pcb */
typedef struct task_t{
    int pid;
    int ppid;
    int esp0;
    char name[16];
    task_state_t state;
    task_func_t function;
} __attribute((packed)) task_t;

task_t* create_task(char* name, task_func_t fun);
void task_init();

#endif