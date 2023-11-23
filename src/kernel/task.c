#include "../include/linux/task.h"
#include "../include/linux/mm.h"
#include "../include/linux/kernel.h"
#include "../include/linux/string.h"
#include "../include/asm/system.h"

task_t* tasks[NR_TASKS] = {0};

int find_empty_process(){
    for (int i = 0; i < NR_TASKS; i++){
        if (tasks[i] == 0){
            return i;
        }
    }
    printk("No valid PID\n");
    return -1;
}

task_t* create_task(char* name, task_func_t fun){
    /* put pcb and stack in the same page */
    task_t* task = kmalloc(4096);

    task->pid = find_empty_process();
    task->ppid = 0;
    task->esp0 = (int)task + 0x1000;
    memcpy(task->name, name, strlen(name));
    task->state = TASK_INTERRUPTIBLE;
    task->function = fun;

    tasks[task->pid] = task;

    return task;
}

task_func_t*  get_task_function(task_t* task){
    return &task->function;
}

/* test task function a */
void* func_a(void* args){
    printk("Task A starting running... \n");
    static int counter_a = 0;
    while(1){
        printk("Task A, Counter: %d\n",counter_a);
        counter_a++;
        HLT;
        HLT;
        HLT;
        HLT;
        HLT;
    }
}

/* test task function b */
void* func_b(void* args){
    printk("Task B starting running... \n");
    static int counter_b = 0;
    while(1){
        printk("Task B, Counter: %d\n",counter_b);
        counter_b++;
        HLT;
        HLT;
        HLT;
        HLT;
        HLT;
    }
}

void task_init(){
    create_task("task_a", func_a);
    create_task("task_b", func_b);
}
