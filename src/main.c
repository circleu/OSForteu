#include "main.h"

VOID main() {
  SetIDT();
  ASM_STI;
  PrintfLn("%d", 0x07, ATA_IS_LBA48());

  while (1);

  return;
}