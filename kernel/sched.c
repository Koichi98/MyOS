#include "util.h"
#include "pm_timer.h"
#include "lapic_timer.h"
#include "interruption.h"
#include "sched.h"
#include "hardware.h"

//#define STACK_SIZE 1024
#define TASK_NUM 3

//char stack1 [STACK_SIZE];
//char stack2 [STACK_SIZE];
//char stack3 [STACK_SIZE];

//static void task1(){
    //while (1) {
        //puts("hello from task1\r\n");
        //volatile int i = 100000000;
        //while (i--);
    //}
//}

//static void task2(){
    //while (1) {
        //puts("hello from task2\r\n");
        //volatile int i = 100000000;
        //while (i--);
    //}
//}

//static void task3(){
    //while (1) {
        //puts("hello from task3\r\n");
        //volatile int i = 100000000;
        //while (i--);
    //}
//}

struct Task{
    unsigned long long sp;
};

struct Task tasks[TASK_NUM];

unsigned int current_task;

static void init_task(int idx,unsigned char *stack_bottom,unsigned long long rip){
    unsigned long long *sp = stack_bottom;

    unsigned long long ss;
    asm volatile("mov %%ss, %0":"=r"(ss));
    *(sp-1) = ss;

    *(sp-2) = stack_bottom;

    unsigned long long cur_sp = sp - 2;
    unsigned long long rflags;
    asm volatile ("mov %%rsp, %0\n" "mov %1, %%rsp\n" "pushfq\n" : "=r"(rflags) : "m"(cur_sp));
    asm volatile ("mov %0, %%rsp"::"m"(rflags));

    unsigned long long cs;
    asm volatile("mov %%cs, %0":"=r"(cs));
    *(sp-4) = cs;

    *(sp-5) = rip;
    tasks[idx].sp =  (unsigned long long)stack_bottom - 8*20;
}


void init_tasks(){

    unsigned char* stack_bottom2 = 0x106000000;
    unsigned char* stack_bottom3 = 0x107000000;
    unsigned long long APP1_START = 0x104000000;
    unsigned long long APP2_START = 0x105000000;
    unsigned long long APP3_START = 0x106000000;

    
    init_task(1,stack_bottom2,APP2_START);
    init_task(2,stack_bottom3,APP3_START);

    //asm volatile ("mov %0,%%rsp" ::"r"(stack1+STACK_SIZE));
    //current_task = 0;
    //task1();

    asm volatile ("jmp *%0" :: "m"(APP1_START));
    
}

void schedule(unsigned long long sp){

    tasks[current_task].sp = sp;
    current_task = (current_task+1)%3;

    lapic_set_eoi();

    unsigned long long sp_new;
    sp_new = tasks[current_task].sp; 
    asm volatile ("mov %0, %%rsp" :: "r"(sp_new));
    asm volatile ("pop %r15");
    asm volatile ("pop %r14");
    asm volatile ("pop %r13");
    asm volatile ("pop %r12");
    asm volatile ("pop %r11");
    asm volatile ("pop %r10");
    asm volatile ("pop %r9");
    asm volatile ("pop %r8");
    asm volatile ("pop %rdi");
    asm volatile ("pop %rsi");
    asm volatile ("pop %rbp");
    asm volatile ("pop %rbx");
    asm volatile ("pop %rdx");
    asm volatile ("pop %rcx");
    asm volatile ("pop %rax");
    asm volatile ("iretq");

}
