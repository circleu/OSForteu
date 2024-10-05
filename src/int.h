#pragma once

#include "type.h"
#include "asm.h"
#include "data.h"
#include "kbd.h"
#include "scr.h"

#define MASTER_PIC_COMMAND 0x20
#define MASTER_PIC_DATA 0x21
#define SLAVE_PIC_COMMAND 0xa0
#define SLAVE_PIC_DATA 0xa1


struct interrupt_frame;

VOID SetIDT();
__attribute__((interrupt)) VOID ISR_Ignore(struct interrupt_frame*);
__attribute__((interrupt)) VOID ISR_Timer(struct interrupt_frame*);
__attribute__((interrupt)) VOID ISR_Keyboard(struct interrupt_frame*);

struct IDT {
  UINT16 OffsetLow;
  UINT16 Selector;
  UINT16 Type;
  UINT16 OffsetHigh;
}__attribute__((packed));

struct IDTR {
  UINT16 Size;
  UINT32 Address;
}__attribute__((packed));