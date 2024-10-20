CC = gcc
CFLAGS = -c -m32 -masm=intel -mno-80387 -ffreestanding -fno-pie -O0 -nostdlib
OBJ = tmp/main.o tmp/int.o tmp/data.o tmp/kbd.o tmp/scr.o tmp/strfunc.o tmp/fs.o tmp/math64.o tmp/shell.o tmp/time.o

kernel.img : tmp/boot.bin tmp/kernel.bin
	cat $^ > $@

tmp/kernel.bin : $(OBJ)
	ld -melf_i386 -Ttext 0x10000 -nostdlib $^ -o $@ --oformat binary

tmp/time.o : src/time.c
	$(CC) $(CFLAGS) $< -o $@	

tmp/shell.o : src/shell.c
	$(CC) $(CFLAGS) $< -o $@

tmp/math64.o : src/math64.c
	$(CC) $(CFLAGS) $< -o $@

tmp/fs.o : src/fs.c
	$(CC) $(CFLAGS) $< -o $@

tmp/strfunc.o : src/strfunc.c
	$(CC) $(CFLAGS) $< -o $@

tmp/scr.o : src/scr.c
	$(CC) $(CFLAGS) $< -o $@

tmp/kbd.o : src/kbd.c
	$(CC) $(CFLAGS) $< -o $@

tmp/data.o : src/data.c
	$(CC) $(CFLAGS) $< -o $@

tmp/int.o : src/int.c
	$(CC) $(CFLAGS) $< -o $@

tmp/main.o : src/main.c
	$(CC) $(CFLAGS) $< -o $@

tmp/boot.bin : src/boot.asm
	nasm -f bin -o $@ $<

clean :
	rm tmp/*.*