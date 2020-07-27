#include "util.h"
#include "hardware.h"
#define FONT_HEIGHT 8

extern unsigned char font[128][FONT_HEIGHT];

struct FrameBuffer *FB;
unsigned int fb_y;
unsigned fb_x;

void init_frame_buffer(struct FrameBuffer *fb){
  for (unsigned int i = 0; i < fb->height; i++) {
    for (unsigned int j = 0; j < fb->width; j++) {
      struct Pixel *pixel = fb->base + fb->width * i + j;
      pixel->r = 0;
      pixel->g = 0;
      pixel->b = 0;
    }
  }
}


void putc(char c){
    //case c = '\n'
    if(c==10){
        if(fb_y + 8 > FB->height){
            init_frame_buffer(FB);
            fb_x = 0;
            fb_y = 0;
        }
       fb_x = 0;
       fb_y += 8; 
    }
    //case c = '\r'
    else if(c==13){
        if(fb_y + 8 > FB->height){
            init_frame_buffer(FB);
            fb_x = 0;
            fb_y = 0;
        }
        fb_y += 8;
    }else{
        for(int j=0;j<8;j++){
            char font_c = font[c][j];
            for (int i=0;i<8;i++){
                if (font_c&0b10000000){
                    (FB->base + fb_x + i + FB->width * (fb_y+j)) -> r = 255;
                    (FB->base + fb_x + i + FB->width * (fb_y+j)) -> g = 255;
                    (FB->base + fb_x + i + FB->width * (fb_y+j)) -> b = 255;
                }
                font_c = font_c <<  1;
            }
        }
        if(fb_x + 7 > FB->width && fb_y + 7 > FB->height){
            init_frame_buffer(FB);
            fb_x=0;
            fb_y=0;
        }else if(fb_x + 7 > FB->width){
            fb_x = 0;
            fb_y += 8;
        }else{
            fb_x += 8;
        }
    }
}

void puts(char *str){
    while(*str){
        putc(*str);
        str++;
    }
}

void puth(unsigned long long value,unsigned char digits_len){
    unsigned long long q,r;
    q = value;
    unsigned char count = digits_len;
    char array[digits_len];
    while(count>0){
        r = q % 16;
        q = q / 16;
        array[count-1] = r;
        count--;
    }
    
    for(int i=0;i<digits_len;i++){
        if(array[i]<10){
            putc(48+array[i]);
        }else{
            putc(65+array[i]-10);
        }
    }

}


void print(){
    puts("ok\n");
}

void (*reserved_callback2)();

void func(){
reserved_callback2 = print;
reserved_callback2();
}