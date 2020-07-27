#include "hardware.h"

void init_frame_buffer(struct FrameBuffer *fb);

void putc(char c);

void puts(char *str);

void puth(unsigned long long value,unsigned char digits_len);

void print();