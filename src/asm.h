#pragma once

#include "type.h"

#define ASM_OUTB(dest, origin) asm volatile("mov al, %0; mov dx, %1; out dx, al;"::"r"((UINT8)origin), "r"((UINT16)dest))
#define ASM_INB(dest, origin) asm volatile("mov dx, %1; in al, dx; mov %0, al":"=r"((UINT8)dest): "r"((UINT16)origin))
#define ASM_LIDT(idtr) asm volatile("mov eax, %0; lidt [eax]"::"r"(idtr))
#define ASM_CLI asm volatile("cli")
#define ASM_STI asm volatile("sti")
#define ASM_REP_OUTSW(dest, origin, count) asm volatile("mov dx, %0; mov esi, %1; mov ecx, %2; rep outsw;"::"r"((UINT16)dest), "r"(origin), "r"(count))
#define ASM_REP_INSW(dest, origin, count) asm volatile("mov dx, %0; mov edi, %1; mov ecx, %2; rep insw;"::"r"((UINT16)origin), "r"(dest), "r"(count))

