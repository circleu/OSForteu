#include "int.h"


VOID SetIDT() {
  struct IDT IDT[3];
  struct IDTR IDTR = (struct IDTR){256 * 8 - 1, 0};
  UINT32 isr;
  UINT16* idt_ptr;

  isr = (UINT32)ISR_Ignore;
  IDT[0].OffsetLow = (UINT16)(isr & 0xffff);
  IDT[0].Selector = (UINT16)0x08;
  IDT[0].Type = (UINT16)0x8e00;
  IDT[0].OffsetHigh = (UINT16)(isr >> 16);

  isr = (UINT32)ISR_Timer;
  IDT[1].OffsetLow = (UINT16)(isr & 0xffff);
  IDT[1].Selector = (UINT16)0x08;
  IDT[1].Type = (UINT16)0x8e00;
  IDT[1].OffsetHigh = (UINT16)(isr >> 16);

  isr = (UINT32)ISR_Keyboard;
  IDT[2].OffsetLow = (UINT16)(isr & 0xffff);
  IDT[2].Selector = (UINT16)0x08;
  IDT[2].Type = (UINT16)0x8e00;
  IDT[2].OffsetHigh = (UINT16)(isr >> 16);

  for (UINT32 i = 0; i < 0x100; i++) {
    idt_ptr = (UINT16*)(i * 8);
    *idt_ptr++ = IDT[0].OffsetLow;
    *idt_ptr++ = IDT[0].Selector;
    *idt_ptr++ = IDT[0].Type;
    *idt_ptr++ = IDT[0].OffsetHigh;
  }

  idt_ptr = (UINT16*)(0x20 * 8);
  *idt_ptr++ = IDT[1].OffsetLow;
  *idt_ptr++ = IDT[1].Selector;
  *idt_ptr++ = IDT[1].Type;
  *idt_ptr++ = IDT[1].OffsetHigh;

  idt_ptr = (UINT16*)(0x21 * 8);
  *idt_ptr++ = IDT[2].OffsetLow;
  *idt_ptr++ = IDT[2].Selector;
  *idt_ptr++ = IDT[2].Type;
  *idt_ptr++ = IDT[2].OffsetHigh;

  ASM_OUTB(0x64, 0xae);
  ASM_LIDT(&IDTR);
  ASM_OUTB(MASTER_PIC_DATA, 0x00);
  ASM_OUTB(SLAVE_PIC_DATA, 0x00);

  return;
}

__attribute__((interrupt)) VOID ISR_Ignore(struct interrupt_frame* frame) {
  ASM_OUTB(MASTER_PIC_COMMAND, 0x20);

  return;
}

__attribute__((interrupt)) VOID ISR_Timer(struct interrupt_frame* frame) {
  ASM_OUTB(MASTER_PIC_COMMAND, 0x20);

  Timer++;

  ScreenUpdate();

  return;
}

__attribute__((interrupt)) VOID ISR_Keyboard(struct interrupt_frame* frame) {
  UINT8 key = 0;
  
  ASM_INB(key, 0x60);

  Keystroke = TranslateKey(key);
  KeyboardController();

  ASM_OUTB(MASTER_PIC_COMMAND, 0x20);

  return;
}


