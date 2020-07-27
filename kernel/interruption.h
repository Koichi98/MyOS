struct InterrputDescripter{
    unsigned short offset_lo;
    unsigned short segment;
    unsigned short attribute;
    unsigned short offset_mid;
    unsigned int offset_hi;
    unsigned int reserved;
}__attribute__((packed));

static void load_Idt_to_Idtr();

static void register_intr_handler(unsigned char index, unsigned long long offset, unsigned short segment, unsigned short attribute);

void init_intr();