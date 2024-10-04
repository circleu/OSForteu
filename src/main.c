#include "main.h"

VOID main() {
  // Setup...
  SetIDT();
  PrintLn("IDT is set.", 0x07);
  
  ATA_Get_IDENTIFY(ATA_IDENTIFY);
  PrintLn("Sent IDENTIFY command.", 0x07);

  if (ATA_Is_LBA48() != 1) {
    PrintLn("This device doesn't support LBA48.", 0x03);
    PrintLn("Please update your BIOS.", 0x03);
    goto LOOP;
  }
  else
    PrintLn("Checked LBA48 support.", 0x07);

  ASM_STI;
  
  // You can now use OSForteu!


  LOOP:
  while (1);

  return;
}