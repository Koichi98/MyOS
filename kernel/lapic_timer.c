#include "util.h"
#include "pm_timer.h"
#include "lapic_timer.h"
#include "interruption.h"

volatile unsigned int *lvt_timer = 0xfee00320;

volatile unsigned int *initial_count = 0xfee00380;

volatile unsigned int *current_count = 0xfee00390;

volatile unsigned int *divide_config = 0xfee003e0;

unsigned int lapic_timer_freq_khz;

unsigned int measure_lapic_freq_khz(){
    *lvt_timer = 0x00000000;
    *initial_count = 0x10000000;
    pm_timer_wait_milisec(0);
    unsigned int over_head = *initial_count - *current_count;
    *initial_count = 0x10000000;
    pm_timer_wait_milisec(100);
    unsigned int incliment = *initial_count - *current_count;
    unsigned int freq_khz = (incliment - over_head) / 1000 * 10;
    lapic_timer_freq_khz = freq_khz;
    return freq_khz;
}

volatile unsigned int *lapic_eoi = 0xfee000b0;

void (*reserved_callback)();

void lapic_periodic_exec(unsigned int msec,void *callback){
    init_intr();
    reserved_callback = callback;
    //unsigned int temp = *lvt_timer;
    //vector:32,delivery status:0,mask:0,timer mode:01
    *lvt_timer = 0b0100000000000100000;// & temp; 0x00010020; 
    //*divide_config = *divide_config | 0b00000000000000000000000000001011;

    *initial_count = lapic_timer_freq_khz * msec;
}

void lapic_intr_handler_internal(){
    reserved_callback();
    *lapic_eoi = 0x00000001;
}