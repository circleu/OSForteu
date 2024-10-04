#include "fs.h"

UINT8 ATA_Get_STATUS() {
  UINT8 value;

  ASM_INB(0x00, ATA_PRIMARY_BUS7);
  ASM_GET_AL(value);

  return value;
}

VOID ATA_Get_IDENTIFY(UINT16* buffer) {
  ASM_OUTB(ATA_PRIMARY_BUS6, 0xa0);
  ASM_OUTB(ATA_PRIMARY_BUS2, 0x00);
  ASM_OUTB(ATA_PRIMARY_BUS3, 0x00);
  ASM_OUTB(ATA_PRIMARY_BUS4, 0x00);
  ASM_OUTB(ATA_PRIMARY_BUS5, 0x00);
  ASM_OUTB(ATA_PRIMARY_BUS7, 0xec);
  ASM_INB(0x00, ATA_PRIMARY_BUS7);
  ASM_REP_INSW(buffer, ATA_PRIMARY_BUS0, 256);

  return;
}

BOOL ATA_Is_BSY() {
  UINT8 status = ATA_Get_STATUS();

  if ((status & 0x80) == 0x80)
    return TRUE;
  else
    return FALSE;
}

BOOL ATA_Is_RDY() {
  UINT8 status = ATA_Get_STATUS();

  if ((status & 0x40) == 0x40)
    return TRUE;
  else
    return FALSE;
}

BOOL ATA_Is_DRQ() {
  UINT8 status = ATA_Get_STATUS();

  if ((status & 0x08) == 0x08)
    return TRUE;
  else
    return FALSE;
}

BOOL ATA_Is_ERR() {
  UINT8 status = ATA_Get_STATUS();

  if ((status & 0x01) == 0x01)
    return TRUE;
  else
    return FALSE;
}

BOOL ATA_Is_LBA48() {
  if ((ATA_IDENTIFY[83] & 0x400) == 0x400)
    return TRUE;
  else
    return FALSE;
}

VOID ATA_Read_Sector(UINT64 sector, UINT8* buffer) {
  UINT8 LBA0 = sector & 0xff;
  UINT8 LBA1 = (sector >> 8) & 0xff;
  UINT8 LBA2 = (sector >> 16) & 0xff;
  UINT8 LBA3 = (sector >> 24) & 0xff;
  UINT8 LBA4 = (sector >> 32) & 0xff;
  UINT8 LBA5 = (sector >> 40) & 0xff;

  ASM_OUTB(ATA_PRIMARY_CONTROL0, 0x00);

  while (ATA_Is_BSY() == TRUE);

  ASM_OUTB(ATA_PRIMARY_BUS6, 0x40);

  ASM_OUTB(ATA_PRIMARY_BUS2, 0x00);
  ASM_OUTB(ATA_PRIMARY_BUS3, LBA3);
  ASM_OUTB(ATA_PRIMARY_BUS4, LBA4);
  ASM_OUTB(ATA_PRIMARY_BUS5, LBA5);
  ASM_OUTB(ATA_PRIMARY_BUS2, 0x01);
  ASM_OUTB(ATA_PRIMARY_BUS3, LBA0);
  ASM_OUTB(ATA_PRIMARY_BUS4, LBA1);
  ASM_OUTB(ATA_PRIMARY_BUS5, LBA2);

  while ((ATA_Is_BSY() == TRUE) || (ATA_Is_RDY() == FALSE));

  ASM_OUTB(ATA_PRIMARY_BUS7, 0x24);

  if (ATA_Is_ERR() == TRUE) {
    PrintLn("Disk read error.", 0x03);
    return;
  }

  while (ATA_Is_DRQ() == FALSE);

  ASM_REP_INSW(buffer, ATA_PRIMARY_BUS0, 256);

  return;
}

VOID ATA_Read_Cluster(UINT64 cluster, UINT8* buffer) {
  UINT8 temp[512] = {0, };
  cluster *= 8;

  for (UINT8 i = 0; i < 8; i++) {
    ATA_Read_Sector(cluster + i, temp);
    CopyByte(buffer, temp, 512 * i, 0, 512);
  }

  return;
}