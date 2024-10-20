[bits 16]
[org 0x7c00]

Start:
  xor ax, ax
  mov ds, ax
  mov [BOOTDISK], dl

SetVideoMode:
  mov ax, 0x0003
  int 0x10
  jnc EnableA20Line

EnableA20Line:
  mov ax, 0x2401
  int 0x15
  jnc LoadKernel

LoadKernel:
  mov ax, 0x1000
  mov es, ax
  xor bx, bx

  mov ah, 0x02
  mov al, 127
  mov ch, 0
  mov cl, 2
  mov dh, 0
  mov dl, [BOOTDISK]
  int 0x13

  jnc PrepareProtectedMode

PrepareProtectedMode:
  cli

  mov al, 0x11
  out 0x20, al
  out 0xa0, al

  mov al, 0x20
  out 0x21, al
  mov al, 0x28
  out 0xa1, al

  mov al, 0x04
  out 0x21, al
  mov al, 0x02
  out 0xa1, al

  mov al, 0x01
  out 0x21, al
  out 0xa1, al

  mov al, 0xff
  out 0x21, al
  out 0xa1, al

  lgdt[GDTR]

  mov eax, cr0
  or eax, 1
  mov cr0, eax

  jmp dword 0x08:EnterProtectedMode

[bits 32]
EnterProtectedMode:
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  mov ebp, 0xffffff
  mov esp, ebp

  jmp dword 0x08:0x10000

GDTR:
  dw GDT_end - GDT - 1
  dd GDT

GDT:
; See image.png to further information.
  dw 0, 0
  dw 0, 0

  dw 0b1111111111111111, 0b0000000000000000
  dw 0b1001101000000000, 0b0000000011001111

  dw 0b1111111111111111, 0b0000000000000000
  dw 0b1001001000000000, 0b0000000011001111

GDT_end:

BOOTDISK db 0

times 510 - ($ - $$) db 0
dw 0xaa55