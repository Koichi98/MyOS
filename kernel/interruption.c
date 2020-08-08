#include "util.h"
#include "pm_timer.h"
#include "lapic_timer.h"
#include "interruption.h"

struct InterrputDescripter IDT[256];

unsigned char data[10];

static void load_Idt_to_Idtr(){
    unsigned long long IDT_address = IDT;
    //(Size of IDT) -1 : (16 * 256) -1 = 4096 = 0xfff
    data[0] = 0xff;
    data[1] = 0x0f;

    data[9] = IDT_address >> 56;
    data[8] = IDT_address >> 48;
    data[7] = IDT_address >> 40;
    data[6] = IDT_address >> 32;
    data[5] = IDT_address >> 24;
    data[4] = IDT_address >> 16;
    data[3] = IDT_address >> 8 ;
    data[2] = IDT_address;
    asm volatile ("lidt %0" :: "m"(data));
}

static void register_intr_handler(unsigned char index, unsigned long long offset, unsigned short segment, unsigned short attribute){
    unsigned short slice_low_16bit = 0b1111111111111111;
    IDT[index].offset_lo = offset & slice_low_16bit;
    IDT[index].segment = segment;
    IDT[index].attribute = attribute;
    IDT[index].offset_mid = offset >> 16 & slice_low_16bit;
    IDT[index].offset_hi = offset >> 32;
    //IDT[index].reserved = 0x00000000;
}

void init_intr(){

    load_Idt_to_Idtr();

    unsigned long long handler;
    asm volatile ("lea lapic_intr_handler(%%rip), %[handler]" : [handler]"=r"(handler));
    unsigned long long handler2;
    asm volatile ("lea syscall_handler(%%rip), %[handler2]" : [handler2]"=r"(handler2));
    unsigned short reg16;
    asm volatile ("mov %%cs, %0" : "=r"(reg16)); 
    unsigned short attribute = 0x8e00;

    //IDT zero clearance
    struct InterrputDescripter empty;
    empty.offset_lo = 0x0000;
    empty.segment = 0x0000;
    empty.attribute = 0x0000;
    empty.offset_mid = 0x0000;
    empty.offset_hi = 0x0000;
    empty.reserved = 0x0000;

    for(int i=0;i<256;i++){
        IDT[i] = empty;
    }
    
    register_intr_handler(32,handler,reg16,attribute);
    register_intr_handler(128,handler2,reg16,attribute);
    asm volatile("sti");
}
