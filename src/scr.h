#pragma once

#include <stdarg.h>
#include "data.h"
#include "strfunc.h"


VOID ScreenUpdate();
VOID ClearLine(UINT32 line);
VOID SetCursor(UINT8 x, UINT8 y);
VOID PushCursor(UINT8 x, UINT8 y);
VOID PullCursor(UINT8 x, UINT8 y);
VOID Print(UINT8* str, UINT32 x, UINT32 y, UINT8 col);
VOID PrintLn(UINT8* str, UINT8 col);
VOID Printf(UINT8* str, UINT32 x, UINT32 y, UINT8 col, ...);
VOID PrintfLn(UINT8* str, UINT8 col, ...);
VOID Printf_d(UINT32 origin, UINT8** pptr, UINT8 col);
VOID Printf_s(UINT8* origin, UINT8** pptr, UINT8 col);
VOID Printf_c(UINT8 origin, UINT8** pptr, UINT8 col);