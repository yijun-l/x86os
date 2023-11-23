#include "../include/linux/task.h"
#include "../include/linux/types.h"
#include "../include/asm/system.h"

extern task_t* tasks[NR_TASKS];
extern void switch_task();
extern void switch_idle_task(task_t*);

task_t* current = 0;

static task_t* find_ready_task(){
    for (int i = 0; i < NR_TASKS; i++){
        task_t* task = tasks[i];
        if (task->state == TASK_INTERRUPTIBLE){
            return task;
        }
    }
    return 0;
}

void sched(){
    task_t* next = find_ready_task();
    if (next != 0){
        next->state = TASK_RUNNING;
        current->state = TASK_INTERRUPTIBLE;
        current = next;
    }
    switch_task();
}