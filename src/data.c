#include "data.h"


UINT8 Timer = 0;
UINT8 Keystroke = 0;
UINT8 KeyboardBuffer[BUFFER_SIZE_MAX] = {0, };
UINT16 KeyboardBufferIndex = 0;

UINT8 ScreenBuffer[BUFFER_SIZE_MAX] = {0, };
UINT32 Cursor[2] = {0, 0};

UINT16 ATA_IDENTIFY_Data[256] = {0, };
struct FSInfo FSInfo = {0, };

VOID CopyByte(UINT8* dest, UINT8* origin, UINT32 d_start, UINT32 o_start, UINT32 count) {
  for (UINT32 i = 0; i < count; i++) {
    dest[d_start + i] = origin[o_start + i];
  }

  return;
}

VOID FlushBuffer(UINT8* buffer) {
  UINT8 zero[BUFFER_SIZE_MAX] = {0, };
  CopyByte(buffer, zero, 0, 0, BUFFER_SIZE_MAX);

  return;
}