#include "../include/linux/task.h"
#include "../include/linux/mm.h"
#include "../include/linux/kernel.h"
#include "../include/linux/string.h"
#include "../include/asm/system.h"

extern task_t* current;

task_t* tasks[NR_TASKS] = {0};
int task_esp0 = 0;
int task_ebp0 = 0;
short task_ss0 = 0;

int find_empty_process(){
    for (int i = 0; i < NR_TASKS; i++){
        if (tasks[i] == 0){
            return i;
        }
    }
    printk("No valid PID\n");
    return -1;
}

task_t* create_task(char* name, task_func_t fun, int priority){
    /* put pcb and stack in the same page */
    task_t* task = kmalloc(4096);

    task->pid = find_empty_process();
    task->ppid = 0;
    task->esp = (int)task + 0x1000;
    task->context.esp = task->esp;
    task->context.ebp = task->esp;
    memcpy(task->name, name, strlen(name));
    task->state = TASK_INTERRUPTIBLE;
    task->function = fun;
    task->priority = priority;
    task->counter = priority;
    task->sched_times = 0;
    tasks[task->pid] = task;

    return task;
}

task_func_t get_task_function(task_t* task){
    return task->function;
}

int inc_sched_times(task_t* task){
    return task->sched_times++;
}

void task_exit(task_t* task){
    for (int i = 0; i < NR_TASKS; i++){
        task_t* tmp = tasks[i];
        if(task == tmp){
            printk("task exit: %s\n", tmp->name);

            tasks[i] = 0;
            current = 0;
            kfree_s(tmp, 4096);
            break;
        }
    }
}

/* test task function a */
void* func_a(void* args){
    printk("Task A starts running... \n");
    for( int i = 0; i < 10; i++){
        printk("Task A, Counter: %d\n",i);
        HLT;
        HLT;
        HLT;
        HLT;
        HLT;
    }
}

/* test task function b */
void* func_b(void* args){
    printk("Task B starts running... \n");
    for( int i = 0; i < 10; i++){
        printk("Task B, Counter: %d\n",i);
        HLT;
        HLT;
        HLT;
        HLT;
        HLT;
    }
}

void* task_c(void* args){
    printk("Task C starts running... \n");
    for(int i = 0; i < 10; i++){
        printk("Task C, Counter: %d\n", i);
        HLT;
        HLT;
        HLT;
        HLT;
        HLT;
    }
}

void task_init(){
    create_task("task_a", func_a, 5);
    create_task("task_b", func_b, 10);
    create_task("task_c", task_c,15);
}
