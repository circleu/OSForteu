#include "fs.h"

UINT8 ATA_GET_STATUS() {
  UINT8 value;

  ASM_INB(0x00, ATA_PRIMARY_BUS7);
  ASM_GET_AL(value);

  return value;
}

BOOL ATA_IS_BSY() {
  UINT8 status = ATA_GET_STATUS();

  if ((status & 0x80) == 0x80)
    return TRUE;
  else
    return FALSE;
}

BOOL ATA_IS_RDY() {
  UINT8 status = ATA_GET_STATUS();

  if ((status & 0x40) == 0x40)
    return TRUE;
  else
    return FALSE;
}

BOOL ATA_IS_DRQ() {
  UINT8 status = ATA_GET_STATUS();

  if ((status & 0x08) == 0x08)
    return TRUE;
  else
    return FALSE;
}

BOOL ATA_IS_ERR() {
  UINT8 status = ATA_GET_STATUS();

  if ((status & 0x01) == 0x01)
    return TRUE;
  else
    return FALSE;
}

BOOL ATA_IS_LBA48() {
  UINT16 buffer[256];

  ASM_OUTB(ATA_PRIMARY_BUS6, 0xa0);
  ASM_OUTB(ATA_PRIMARY_BUS2, 0x00);
  ASM_OUTB(ATA_PRIMARY_BUS3, 0x00);
  ASM_OUTB(ATA_PRIMARY_BUS4, 0x00);
  ASM_OUTB(ATA_PRIMARY_BUS5, 0x00);
  ASM_OUTB(ATA_PRIMARY_BUS7, 0xec);
  ASM_INB(0x00, ATA_PRIMARY_BUS7);
  ASM_REP_INSW(buffer, 0x1f0, 256);

  if ((buffer[83] & 0x400) == 0x400)
    return TRUE;
  else
    return FALSE;
}