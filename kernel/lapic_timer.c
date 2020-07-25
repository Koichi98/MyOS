#include "util.h"
#include "pm_timer.h"
#include "lapic_timer.h"

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
    return freq_khz;
}