#include "util.h"
#include "pm_timer.h"

const unsigned int freq_hz = 3579545;

unsigned short pm_timer_blk;

char pm_timer_is_32;

int strcmp(char* str1, char* str2){
    while(*str1 & *str2){
        if(*str1 != *str2){
            return 0;
        }
        str1++;
        str2++;
    }
    return 1;
}

void init_acpi_pm_timer(struct RSDP *rsdp){
    unsigned int *address = rsdp->xsdt_address;
    unsigned int length = *(address + 1);
    unsigned int count = (length-36)/8;
    unsigned int i = 0;
    struct FADT *fadt;
    while(i<length){
        fadt = *(address + 9 + 2*i);
        if(strcmp((fadt->sdth).signature,"FACP")){
            break;
        }
       i++;
    }

    pm_timer_blk = fadt->PM_TMR_BLK;
    if (fadt->flags&0b100000000){
        pm_timer_is_32 = 1;
    }else{
        pm_timer_is_32 = 0;
    }
}
    
void pm_timer_wait_milisec(unsigned int msec){
    signed long long incliment = (unsigned long long )freq_hz * msec / 1000;
    unsigned int value;
    unsigned int former_value;
    asm volatile ("in %[pm_timer_blk], %[former_value]" : [former_value]"=a"(former_value) : [pm_timer_blk]"d"(pm_timer_blk));
    if(!pm_timer_is_32){
        while(1){
            asm volatile ("in %[pm_timer_blk], %[value]" : [value]"=a"(value) : [pm_timer_blk]"d"(pm_timer_blk));
            if(former_value>value){
                incliment = incliment - (0x00ffffff + value - former_value);
            }else{
                incliment = incliment - (value - former_value);
            }
            if(incliment<=0){
                break;
            }
            former_value = value;
        }
    }else{
        while(1){
            asm volatile ("in %[pm_timer_blk], %[value]" : [value]"=a"(value) : [pm_timer_blk]"d"(pm_timer_blk));
            if(former_value>value){
                incliment = incliment - (0xffffffff + value - former_value);
            }else{
                incliment = incliment - (value - former_value);
            }
            if(incliment<=0){
                break;
            }
            former_value = value;
        }
    }
}