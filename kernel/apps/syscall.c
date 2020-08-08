#include "syscall.h"

typedef enum SYSCALL {
    SYSCALL_PUTS,
}SYSCALL;


unsigned long long syscall_puts(char *str){
  unsigned long long ret;
  asm volatile (
  "mov %[id], %%rdi\n"
  "mov %[str], %%rsi\n"
  "int $0x80\n"
  "mov %%rax, %[ret]\n"
  : [ret]"=r"(ret)
  : [id]"r"((unsigned long long)SYSCALL_PUTS),
  [str]"m"((unsigned long long)str)
  );
    return ret;
}