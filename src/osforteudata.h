#pragma once

#include "osforteutype.h"

#define BUFFER_SIZE_MAX 4096


extern UINT8 Timer;
extern UINT8 Keystroke;
extern UINT8 KeyboardBuffer[BUFFER_SIZE_MAX];
extern UINT16 KeyboardBufferIndex;

extern UINT8 ScreenBuffer[BUFFER_SIZE_MAX];
extern UINT32 Cursor[2];

extern UINT16 ATA_IDENTIFY[256];

VOID CopyByte(UINT8* dest, UINT8* origin, UINT32 d_start, UINT32 o_start, UINT32 count);
VOID FlushBuffer(UINT8* buffer);