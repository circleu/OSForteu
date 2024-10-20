#include "fs.h"

UINT8 ATA_STATUS() {
  UINT8 value;

  ASM_INB(value, ATA_PRIMARY_BUS7);

  return value;
}

VOID ATA_IDENTIFY(UINT16* buffer) {
  UINT8 status;

  ASM_OUTB(ATA_PRIMARY_BUS6, 0xa0);
  ASM_OUTB(ATA_PRIMARY_BUS2, 0x00);
  ASM_OUTB(ATA_PRIMARY_BUS3, 0x00);
  ASM_OUTB(ATA_PRIMARY_BUS4, 0x00);
  ASM_OUTB(ATA_PRIMARY_BUS5, 0x00);
  ASM_OUTB(ATA_PRIMARY_BUS7, 0xec);
  ASM_INB(status, ATA_PRIMARY_BUS7);

  if (status == FALSE) {
    PrintLn("Drive doesn't exist.", 0x07);
    return;
  }

  while (ATA_IsBSY() == TRUE);

  ASM_REP_INSW(buffer, ATA_PRIMARY_BUS0, 256);

  return;
}

UINT64 ATA_READ_NATIVE_MAX_ADDRESS_EXT() {
  UINT8 buffer[6] = {0, };
  UINT64 ret = 0;

  ASM_OUTB(ATA_PRIMARY_BUS7, 0x27);

  while (ATA_IsBSY() == TRUE);
  
  ASM_INB(buffer[0], ATA_PRIMARY_BUS3);
  ASM_INB(buffer[1], ATA_PRIMARY_BUS4);
  ASM_INB(buffer[2], ATA_PRIMARY_BUS5);
  ASM_OUTB(ATA_PRIMARY_CONTROL0, 0x80);
  ASM_INB(buffer[3], ATA_PRIMARY_BUS3);
  ASM_INB(buffer[4], ATA_PRIMARY_BUS4);
  ASM_INB(buffer[5], ATA_PRIMARY_BUS5);

  for (UINT32 i = 0; i < 6; i++) {
    ret += (UINT64)(buffer[i] << 8 * i);
  }

  return ret;
}

BOOL ATA_IsBSY() {
  UINT8 status = ATA_STATUS();

  if ((status & 0x80) == 0x80)
    return TRUE;
  else
    return FALSE;
}

BOOL ATA_IsRDY() {
  UINT8 status = ATA_STATUS();

  if ((status & 0x40) == 0x40)
    return TRUE;
  else
    return FALSE;
}

BOOL ATA_IsDRQ() {
  UINT8 status = ATA_STATUS();

  if ((status & 0x08) == 0x08)
    return TRUE;
  else
    return FALSE;
}

BOOL ATA_IsERR() {
  UINT8 status = ATA_STATUS();

  if ((status & 0x01) == 0x01)
    return TRUE;
  else
    return FALSE;
}

BOOL ATA_IsLBA48() {
  if ((ATA_IDENTIFY_Data[83] & 0x400) == 0x400)
    return TRUE;
  else
    return FALSE;
}

VOID ATA_READ_SECTORS_EXT(UINT64 sector, UINT8* buffer) {
  UINT8 LBA0 = sector & 0xff;
  UINT8 LBA1 = (sector >> 8) & 0xff;
  UINT8 LBA2 = (sector >> 16) & 0xff;
  UINT8 LBA3 = (sector >> 24) & 0xff;
  UINT8 LBA4 = (sector >> 32) & 0xff;
  UINT8 LBA5 = (sector >> 40) & 0xff;

  ASM_OUTB(ATA_PRIMARY_CONTROL0, 0x00);

  while (ATA_IsBSY() == TRUE);

  ASM_OUTB(ATA_PRIMARY_BUS6, 0x40);

  ASM_OUTB(ATA_PRIMARY_BUS2, 0x00);
  ASM_OUTB(ATA_PRIMARY_BUS3, LBA3);
  ASM_OUTB(ATA_PRIMARY_BUS4, LBA4);
  ASM_OUTB(ATA_PRIMARY_BUS5, LBA5);
  ASM_OUTB(ATA_PRIMARY_BUS2, 0x01);
  ASM_OUTB(ATA_PRIMARY_BUS3, LBA0);
  ASM_OUTB(ATA_PRIMARY_BUS4, LBA1);
  ASM_OUTB(ATA_PRIMARY_BUS5, LBA2);

  while ((ATA_IsBSY() == TRUE) || (ATA_IsRDY() == FALSE));

  ASM_OUTB(ATA_PRIMARY_BUS7, 0x24);

  if (ATA_IsERR() == TRUE) {
    PrintLn("Disk read error.", 0x03);
    return;
  }

  while (ATA_IsDRQ() == FALSE);

  ASM_REP_INSW(buffer, ATA_PRIMARY_BUS0, 256);

  return;
}

VOID ATA_WRITE_SECTORS_EXT(UINT64 sector, UINT8* buffer) {
  UINT8 LBA0 = sector & 0xff;
  UINT8 LBA1 = (sector >> 8) & 0xff;
  UINT8 LBA2 = (sector >> 16) & 0xff;
  UINT8 LBA3 = (sector >> 24) & 0xff;
  UINT8 LBA4 = (sector >> 32) & 0xff;
  UINT8 LBA5 = (sector >> 40) & 0xff;

  ASM_OUTB(ATA_PRIMARY_CONTROL0, 0x00);

  while (ATA_IsBSY() == TRUE);

  ASM_OUTB(ATA_PRIMARY_BUS6, 0x40);

  ASM_OUTB(ATA_PRIMARY_BUS2, 0x00);
  ASM_OUTB(ATA_PRIMARY_BUS3, LBA3);
  ASM_OUTB(ATA_PRIMARY_BUS4, LBA4);
  ASM_OUTB(ATA_PRIMARY_BUS5, LBA5);
  ASM_OUTB(ATA_PRIMARY_BUS2, 0x01);
  ASM_OUTB(ATA_PRIMARY_BUS3, LBA0);
  ASM_OUTB(ATA_PRIMARY_BUS4, LBA1);
  ASM_OUTB(ATA_PRIMARY_BUS5, LBA2);

  while ((ATA_IsBSY() == TRUE) || (ATA_IsRDY() == FALSE));

  ASM_OUTB(ATA_PRIMARY_BUS7, 0x34);

  if (ATA_IsERR() == TRUE) {
    PrintLn("Disk read error.", 0x03);
    return;
  }

  while (ATA_IsDRQ() == FALSE);

  ASM_REP_OUTSW(ATA_PRIMARY_BUS0, buffer, 256);

  while (ATA_IsBSY() == TRUE);

  ASM_OUTB(ATA_PRIMARY_BUS7, 0xe7);

  while (ATA_IsBSY() == TRUE);

  return;
}

VOID ATA_Read_Cluster(UINT32 cluster, UINT8* buffer) {
  UINT8 temp[512] = {0, };
  UINT64 sector = (UINT64)(cluster * 8);

  for (UINT8 i = 0; i < 8; i++) {
    ATA_READ_SECTORS_EXT(sector + i, temp);
    CopyByte(buffer, temp, 512 * i, 0, 512);
  }

  return;
}

VOID ATA_Write_Cluster(UINT32 cluster, UINT8* buffer) {
  UINT8 temp[512] = {0, };
  UINT64 sector = (UINT64)(cluster * 8);

  for (UINT8 i = 0; i < 8; i++) {
    CopyByte(temp, buffer, 0, 512 * i, 512);
    ATA_WRITE_SECTORS_EXT(sector + i, temp);
  }

  return;
}

struct FSInfo GetFSInfo() {
  UINT8 buffer[512];
  struct FSInfo* ptr;

  ATA_READ_SECTORS_EXT(FS_START_SECTOR, buffer);
  ptr = (struct FSInfo*)buffer;

  return *ptr;
}

struct File SeekFile(UINT8* name, UINT32* dest_offset) {
  UINT8 temp[512] = {0, };
  struct File* ptr;

  for (UINT32 i = 0; i < FSInfo.FileTableSize; i++) {
    ATA_READ_SECTORS_EXT(FSInfo.FileTableStartSector + i, temp);

    for (UINT32 j = 0; j < 512 / FSInfo.FileSize; j++) {
      ptr = (struct File*)(temp + FSInfo.FileSize * j);
      
      if (cmpstr(ptr->Name, name)) {
        *dest_offset = (512 / FSInfo.FileSize) * i + j; 
        return *ptr;
      }
    }
  }
  
  CopyByte(ptr->Name, "~ERROR", 0, 0, 6);
  return *ptr;
}

struct Fptr SeekFptr(UINT32 offset) {
  UINT8 temp[512] = {0, };
  struct Fptr* ptr;

  ATA_READ_SECTORS_EXT(FSInfo.FptrTableStartSector + FSInfo.FptrSize * offset / 512, temp);
  ptr = (struct Fptr*)(temp + FSInfo.FptrSize * offset % 512);

  return *ptr;
}

VOID ReadFile(UINT8* name, UINT8* buffer) {
  UINT8 temp[4096] = {0, };
  UINT32 offset;
  struct File file = SeekFile(name, &offset);
  struct Fptr fptr = SeekFptr(file.FptrTableOffset);

  if (cmpstr(name, "root") != 1 && fptr.DataAreaOffset[0] == 0) {
    CopyByte(buffer, "~ERROR", 0, 0, 6);
    return;
  }

  for (UINT32 i = 0; i < fptr.Status0; i++) {
    ATA_Read_Cluster(FSInfo.DataAreaStartSector / 8 + fptr.DataAreaOffset[i], temp);
    CopyByte(buffer, temp, 4096 * i, 0, 4096);
  }

  return;
}

VOID WriteFile(UINT8* name, UINT8* buffer) {
  UINT8 temp[4096] = {0, };
  UINT32 offset;
  struct File file = SeekFile(name, &offset);
  struct Fptr fptr = SeekFptr(file.FptrTableOffset);

  if (cmpstr(name, "root") != 1 && fptr.DataAreaOffset[0] == 0) {
    PrintLn("~ERROR", 0x03);
    return;
  }

  if (CheckFileAccessibility(file) == 0x01 || CheckFileAccessibility(file) == 0x02)
    return;

  for (UINT32 i = 0; i < fptr.Status0; i++) {
    CopyByte(temp, buffer, 0, 4096 * i, 4096);
    ATA_Write_Cluster(FSInfo.DataAreaStartSector / 8 + fptr.DataAreaOffset[i], temp);
  }
}

UINT8 CheckFileAccessibility(struct File file) {
  if (file.Type < 0x10) {
    return 0x00;
  }
  else if (file.Type < 0x20 && (file.Type & 0x10) == 0x10) {
    PrintfLn("File \'%s\' is read-only.", 0x03, file.Name);
    return 0x01;
  }
  else if (file.Type < 0x30 && (file.Type & 0x20) == 0x20) {
    PrintfLn("File \'%s\' is inaccessible.", 0x03, file.Name);
    return 0x02;
  }

  PrintfLn("Couldn't check the accessibility of file \'%s\'.", 0x03, file.Name);
  return 0x03;
}

VOID CreateFile(UINT8* name, UINT8* type, UINT8* ro) {
  UINT32 offset;
  UINT8 temp[4096] = {0, };
  struct Fptr* ptr;
  struct File file = SeekFile("", &offset);
  struct Fptr fptr = {0, };

  file.CreateDate = CurrentTime;
  file.EditDate = CurrentTime;

  CopyByte(file.Name, name, 0, 0, 13);

  file.Type = 0;
  if (cmpstr(type, "file"))
    file.Type = 0;
  else if (cmpstr(type, "dir"))
    file.Type = 1;
  if (cmpstr(ro, "true"))
    file.Type += 0x10;
  else if (cmpstr(type, "false"))
    file.Type += 0x00;

  for (UINT32 i = 0; i < FSInfo.FptrTableSize; i++) {
    ATA_READ_SECTORS_EXT(FSInfo.FptrTableStartSector + i, temp);
    for (UINT32 j = 0; j < 512 / FSInfo.FptrSize; j++) {
      ptr = (struct Fptr*)(temp + FSInfo.FptrSize * j);
      if (ptr->Status0 == 0x00 && ptr->Status1 == 0x00)
        file.FptrTableOffset = 512 / FSInfo.FptrSize * i + j;
    }
  }

  ATA_READ_SECTORS_EXT(FSInfo.FileTableStartSector + (offset / (512 / FSInfo.FileSize)), temp);
  CopyByte(temp, (UINT8*)(&file), (offset % (512 / FSInfo.FileSize)) * FSInfo.FileSize, 0, FSInfo.FileSize);
  ATA_WRITE_SECTORS_EXT(FSInfo.FileTableStartSector + (offset / (512 / FSInfo.FileSize)), temp);

  fptr.Status0 = 0x01;
  
  for (UINT32 i = 0; i < math64_div_64_by_32(ATA_READ_NATIVE_MAX_ADDRESS_EXT(), 8); i++) {
    ATA_Read_Cluster((FSInfo.DataAreaStartSector / 8) + i, temp);
    if (temp[0] == 0) {
      fptr.DataAreaOffset[0] = i;
      break;
    }
  }

  ATA_READ_SECTORS_EXT(FSInfo.FptrTableStartSector + (file.FptrTableOffset / (512 / FSInfo.FptrSize)), temp);
  CopyByte(temp, (UINT8*)(&fptr), (file.FptrTableOffset % (512 / FSInfo.FptrSize)) * FSInfo.FptrSize, 0, FSInfo.FptrSize);
  ATA_WRITE_SECTORS_EXT(FSInfo.FptrTableStartSector + (file.FptrTableOffset / (512 / FSInfo.FptrSize)), temp);

  return;
}