#include "../include/linux/task.h"
#include "../include/linux/types.h"
#include "../include/linux/kernel.h"
#include "../include/asm/system.h"

extern task_t* tasks[NR_TASKS];
extern void switch_task();
extern void switch_idle_task(task_t*);

task_t* current = 0;

task_t* find_ready_task() {
    task_t* next = 0;

    int is_all_zero = 1;    /* 1 for all time slice exhausted  */
    int is_null = 1;        /* 1 for no ready tasks            */

    /* iterate tasks array to fill is_all_zero and is_null     */
    for (int i = 0; i < NR_TASKS; i++) {
        task_t* task = tasks[i];
        if (task == 0) continue;
        is_null = 0;
        if (task->counter != 0) {
            is_all_zero = 0;
            break;
        }
    }

    /* return when no tasks*/
    if (is_null){
        return 0;
    }
    /* jump when all time slice exhausted*/
    if (is_all_zero){
        for (int i = 0; i < NR_TASKS; i++) {
            task_t *tmp = tasks[i];
            if (tmp == 0) continue;
            tmp->counter = tmp->priority;
        }

        return find_ready_task();
    }

    /* find pending scheduled tasks */
    for (int i = 0; i < NR_TASKS; i++) {
        task_t* task = tasks[i];
        if (task == 0) continue;
        if ((current == task) && (task->state == TASK_RUNNING)) {
            task->state = TASK_INTERRUPTIBLE;
        }

        if (task->state != TASK_INTERRUPTIBLE) continue;

        if (next == 0) {
            next = task;
        } else if ((task->priority > next->priority) && (task->counter != 0)) {
            next = task;
        }
    }
    return next;
}
void sched() {
    task_t* next = find_ready_task();
    if (current != 0) {
        current->state = TASK_INTERRUPTIBLE;
    }
    next->state = TASK_RUNNING;
    current = next;
    current->counter--;
    switch_task(next);
}


