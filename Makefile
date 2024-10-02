CC = gcc
CFLAGS = -c -m32 -masm=intel -mno-80387 -ffreestanding -fno-pie -O0 -nostdlib
OBJ = tmp/main.o tmp/int.o tmp/osforteudata.o tmp/kbd.o tmp/scr.o tmp/strfunc.o

kernel.img : tmp/boot.bin tmp/kernel.bin
	cat $^ > $@

tmp/kernel.bin : $(OBJ)
	ld -melf_i386 -Ttext 0x10000 -nostdlib $^ -o $@ --oformat binary

tmp/strfunc.o : src/strfunc.c
	$(CC) $(CFLAGS) $< -o $@

tmp/scr.o : src/scr.c
	$(CC) $(CFLAGS) $< -o $@

tmp/kbd.o : src/kbd.c
	$(CC) $(CFLAGS) $< -o $@

tmp/osforteudata.o : src/osforteudata.c
	$(CC) $(CFLAGS) $< -o $@

tmp/int.o : src/int.c
	$(CC) $(CFLAGS) $< -o $@

tmp/main.o : src/main.c
	$(CC) $(CFLAGS) $< -o $@

tmp/boot.bin : src/boot.asm
	nasm -f bin -o $@ $<

clean :
	rm tmp/*.*