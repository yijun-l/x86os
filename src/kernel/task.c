#include "../include/linux/task.h"
#include "../include/linux/mm.h"
#include "../include/linux/kernel.h"
#include "../include/linux/string.h"

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
    task->state = TASK_INIT;
    task->function = fun;

    tasks[task->pid] = task;
    task->state = TASK_READY;

    return task;
}

void* test(void* args){

}

void task_init(){
    create_task("test", test);
}