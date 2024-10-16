#include "main.h"

VOID main() {
  // Setup...
  SetIDT();
  PrintLn("IDT is set.", 0x07);

  ASM_STI;
  PrintLn("Enabled all interrupts.", 0x07);
  
  ATA_IDENTIFY(ATA_IDENTIFY_Data);
  PrintLn("Sent IDENTIFY command.", 0x07);

  if (ATA_Is_LBA48() != 1) {
    PrintLn("This device doesn't support LBA48.", 0x03);
    goto LOOP;
  }
  else
    PrintLn("Checked LBA48 support.", 0x07);
  
  FSInfo = GetFSInfo();
  PrintLn("Got FS Info.", 0x07);
  PrintfLn("This disk has %d sector(s).", 0x07, ATA_READ_NATIVE_MAX_ADDRESS_EXT());
  PrintfLn("File table starts at cluster %d.", 0x07, FSInfo.FileTableStartSector / 8);
  PrintfLn("File table size is %d byte(s).", 0x07, FSInfo.FileTableSize * 512);
  PrintfLn("File size is %d byte(s).", 0x07, FSInfo.FileSize);
  PrintfLn("Fptr table starts at cluster %d.", 0x07, FSInfo.FptrTableStartSector / 8);
  PrintfLn("Fptr table size is %d byte(s).", 0x07, FSInfo.FptrTableSize * 512);
  PrintfLn("Fptr size is %d byte(s).", 0x07, FSInfo.FptrSize);
  PrintfLn("Data area starts at cluster %d.", 0x07, FSInfo.DataAreaStartSector / 8);
  PushCursor(0, 2);
  
  // You can now use OSForteu!

  Shell();

  LOOP:
  while (1);

  return;
}