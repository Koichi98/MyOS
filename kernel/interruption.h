#pragma once

struct InterrputDescripter{
    unsigned short offset_lo;
    unsigned short segment;
    unsigned short attribute;
    unsigned short offset_mid;
    unsigned int offset_hi;
    unsigned int reserved;
}__attribute__((packed));

void init_intr();