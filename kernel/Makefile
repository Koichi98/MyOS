CFLAGS = -Wall -Wextra -nostdinc -nostdlib -fno-builtin -fno-common
LDFLAGS = -s -x

all: kernel apps/app1 apps/app2 apps/app3
	make -C ../
	make copy

kernel: kernel.o memory.o syscall.o sched.o interruption.o intr_handler.o lapic_timer.o pm_timer.o util.o segmentation.o hardware.o load_gdt.o font.o
	ld $(LDFLAGS) -T kernel.ld -o $@ $+

apps/app1: apps/app1.o apps/syscall.o
	ld $(LDFLAGS) -T apps/app1.ld -o $@ $+

apps/app2: apps/app2.o apps/syscall.o
	ld $(LDFLAGS) -T apps/app2.ld -o $@ $+

apps/app3: apps/app3.o apps/syscall.o
	ld $(LDFLAGS) -T apps/app3.ld -o $@ $+

%.o: %.c
	gcc-7 $(CFLAGS) -c -o $@ $<

apps/%.o: apps/%.c
	gcc-7 $(CFLAGS) -c -o $@ $<

%.o:%.s
	gcc-7 $(CFLAGS) -c -o $@ $<

qemu: kernel apps/app1 apps/app2 apps/app3
	make all
	qemu-system-x86_64 -m 4G -bios ../build/OVMF.fd -hda fat:rw:../fs

copy:
	cp kernel ../fs/
	cp apps/app1 ../fs/
	cp apps/app2 ../fs/
	cp apps/app3 ../fs/

clean:
	rm -f *.o *apps/*.o apps/app1 apps/app2 apps/app3  kernel

.PHONY: all qemu copy clean
