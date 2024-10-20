#pragma once

#include "type.h"
#include "strfunc.h"
#include "asm.h"
#include "data.h"
#include "kbd.h"
#include "scr.h"

#define ARG_SIZE_MAX 0xff
#define DISK_BUFFER_SIZE_MAX 4096*16

VOID Shell();
VOID TranslateCommand(UINT8* str);

VOID shcmd_seek(UINT8* arg0);
VOID shcmd_read(UINT8* arg0);
VOID shcmd_write(UINT8* arg0, UINT8* arg1);
VOID shcmd_create(UINT8* arg0, UINT8* arg1, UINT8* arg2);

VOID parsestr(UINT8* str, UINT8 dest[ARG_SIZE_MAX][ARG_SIZE_MAX]);
VOID FlushDiskBuffer(UINT8* buffer);