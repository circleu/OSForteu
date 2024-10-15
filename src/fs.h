#pragma once

#include "type.h"
#include "asm.h"
#include "scr.h"
#include "data.h"

#define ATA_PRIMARY_BUS0 0x1f0 // data[R/W] | 16 bit
#define ATA_PRIMARY_BUS1 0x1f1 // error[R], features[W] | 8/16 bit
// error
// 0: AMNF - address mark not found
// 1: TKZNF - track zero not found
// 2: ABRT - aborted command
// 3: MCR - media change request
// 4: IDNF - id not found
// 5: MC - media changed
// 6: UNC - uncorrectable data error
// 7: BBK - bad block detected
#define ATA_PRIMARY_BUS2 0x1f2 // sector count[R/W] | 8/16 bit
#define ATA_PRIMARY_BUS3 0x1f3 // LBA low[R/W] | 8/16 bit
#define ATA_PRIMARY_BUS4 0x1f4 // LBA mid[R/W] | 8/16 bit
#define ATA_PRIMARY_BUS5 0x1f5 // LBA high[R/W] | 8/16 bit
#define ATA_PRIMARY_BUS6 0x1f6 // drive select[R/W] | 8 bit
// 0-3: LBA bit 24-17
// 4: DRV - selects the drive number
// 5: (always 1)
// 6: LBA (always 1. we're gonna use LBA)
// 7: (always 1)
#define ATA_PRIMARY_BUS7 0x1f7 // status port[R], command port[W] | 8 bit
// status
// 0: ERR - indicates an error occured. send new command to clear it
// 1: IDX - index (always 0)
// 2: CORR - corrected data (always 0)
// 3: DRQ - set when the drive has PIO data to transfer, or is ready to accept PIO data
// 4: SRV - overlapped mode service request
// 5: DF - drive fault error
// 6: RDY - bit is clear when drive is spun down, or after an error. set otherwise
// 7: BSY - indicates the drive is preparing to send/receive data (wait for it to clear)
#define ATA_PRIMARY_CONTROL0 0x3f6 // alternate status[R], device control[W] | 8 bit
// device control
// 0: (always 0)
// 1: nIEN - set this to stop the current device from sending interrupts
// 2: SRST - set, then clear, this to do a software reset on all ata drive on a bus, if one is misbehaving
// 3: (reserved)
// 4: (reserved)
// 5: (reserved)
// 6: (reserved)
// 7: HOB - set this to read back the high order byte of the lat LBA48 value sent to an io port
#define ATA_PRIMARY_CONTROL1 0x3f7 // drive address[R] | 8 bit

#define ATA_SECONDARY_BUS0 0x170
#define ATA_SECONDARY_BUS1 0x171
#define ATA_SECONDARY_BUS2 0x172
#define ATA_SECONDARY_BUS3 0x173
#define ATA_SECONDARY_BUS4 0x174
#define ATA_SECONDARY_BUS5 0x175
#define ATA_SECONDARY_BUS6 0x176
#define ATA_SECONDARY_BUS7 0x177
#define ATA_SECONDARY_CONTROL0 0x376
#define ATA_SECONDARY_CONTROL1 0x377

#define FS_START_SECTOR 128


struct FSInfo {
  UINT32 FileTableStartSector;
  UINT32 FileTableSize;
  UINT8 FileSize;

  UINT32 DptrTableStartSector;
  UINT32 DptrTableSize;
  UINT8 DptrSize;

  UINT32 DataAreaStartSector;
}__attribute__((packed));

struct Date {
  UINT16 Year;
  UINT8 Month;
  UINT8 Date;
  UINT8 Hour;
  UINT8 Minute;
  UINT8 Second;
}__attribute__((packed));

struct File {
  UINT8 Name[13];
  UINT8 Type;
  UINT32 PtrTableOffset;
  struct Date CreateDate;
  struct Date EditDate;
}__attribute__((packed));

struct FilePointer {
  UINT8 Status0;
  UINT8 Status1;
  UINT32 DataAreaOffset[7];
  UINT8 Reserved[2];
}__attribute__((packed));


UINT8 ATA_STATUS();
VOID ATA_IDENTIFY(UINT16* buffer);
UINT64 ATA_READ_NATIVE_MAX_ADDRESS_EXT();
BOOL ATA_Is_BSY();
BOOL ATA_Is_RDY();
BOOL ATA_Is_DRQ();
BOOL ATA_Is_ERR();
BOOL ATA_Is_LBA48();
VOID ATA_READ_SECTORS_EXT(UINT64 sector, UINT8* buffer);
VOID ATA_Read_Cluster(UINT32 cluster, UINT8* buffer);
struct FSInfo GetFSInfo();
struct File SeekFile(UINT8* name);