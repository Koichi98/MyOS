#include "syscall.h"
#include "util.h"

//typedef enum SYSCALL {
    //SYSCALL_PUTS,
//}SYSCALL;


unsigned long long syscall_puts(char *str){
    puts(str);
    return 0;
}

unsigned long long syscall_handler_internal(SYSCALL syscall_id,unsigned long long arg1,unsigned long long arg2,unsigned long long arg3){
    puts(arg1);
    if (syscall_id == SYSCALL_PUTS){
        return syscall_puts((char*)arg1);
    }
    return 1;
}