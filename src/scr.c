#include "scr.h"


VOID ScreenUpdate() {
  UINT8* video = (UINT8*)0xb8000;

  if (Cursor[1] >= 24) {
    CopyByte(ScreenBuffer, ScreenBuffer, 0, 160, 4000);
    ClearLine(Cursor[1]);
    Cursor[1]--;
  }
  
  CopyByte(video, ScreenBuffer, 0, 0, BUFFER_SIZE_MAX);

  return;
}

VOID ClearLine(UINT32 line) {
  UINT8 zero[BUFFER_SIZE_MAX] = {0, };

  CopyByte(ScreenBuffer, zero, line * 160, 0, 160);

  return;
}

VOID SetCursor(UINT8 x, UINT8 y) {
  Cursor[0] = x;
  Cursor[1] = y;
  return;
}

VOID PushCursor(UINT8 x, UINT8 y) {
  Cursor[0] += x;
  Cursor[1] += y;
  return;
}

VOID PullCursor(UINT8 x, UINT8 y) {
  Cursor[0] -= x;
  Cursor[1] -= y;
  return;
}

VOID Print(UINT8* str, UINT32 x, UINT32 y, UINT8 col) {
  UINT8* ptr = (UINT8*)(ScreenBuffer + x * 2 + y * 160);

  for (UINT32 i = 0; str[i] != 0; i++) {
    *ptr++ = str[i];
    *ptr++ = col;
  }

  return;
}

VOID PrintLn(UINT8* str, UINT8 col) {
  UINT8* ptr = (UINT8*)(ScreenBuffer + Cursor[0] * 2 + Cursor[1] * 160);

  for (UINT32 i = 0; str[i] != 0; i++) {
    *ptr++ = str[i];
    *ptr++ = col;
  }

  Cursor[1]++;

  return;
}

VOID Printf(UINT8* str, UINT32 x, UINT32 y, UINT8 col, ...) {
  va_list va;
  va_start(va, col);

  UINT8* ptr = (UINT8*)(ScreenBuffer + x * 2 + y * 160);

  for (UINT32 i = 0; str[i] != 0; i++) {
    if (str[i] == '%') {
      i++;
      if (str[i] == 'd')
        Printf_d((UINT32)va_arg(va, UINT32), &ptr, col);
      else if (str[i] == 's')
        Printf_s((UINT8*)va_arg(va, UINT8*), &ptr, col);
      else if (str[i] == 'c')
        Printf_c((UINT8)va_arg(va, UINT32), &ptr, col);
    }
    else {
      *ptr++ = str[i];
      *ptr++ = col;
    }
  }

  va_end(va);

  return;
}

VOID PrintfLn(UINT8* str, UINT8 col, ...) {
  va_list va;
  va_start(va, col);

  UINT8* ptr = (UINT8*)(ScreenBuffer + Cursor[0] * 2 + Cursor[1] * 160);

  for (UINT32 i = 0; str[i] != 0; i++) {
    if (str[i] == '%') {
      i++;
      if (str[i] == 'd')
        Printf_d((UINT32)va_arg(va, UINT32), &ptr, col);
      else if (str[i] == 's')
        Printf_s((UINT8*)va_arg(va, UINT8*), &ptr, col);
      else if (str[i] == 'c')
        Printf_c((UINT8)va_arg(va, UINT32), &ptr, col);
    }
    else {
      *ptr++ = str[i];
      *ptr++ = col;
    }
  }

  va_end(va);

  Cursor[1]++;

  return;
}

VOID Printf_d(UINT32 origin, UINT8** pptr, UINT8 col) {
  UINT8* buf = itoa(origin);

  for (UINT32 i = 0; buf[i] != 0; i++) {
    **pptr = buf[i];
    (*pptr)++;
    **pptr = col;
    (*pptr)++;
  }

  return;
}

VOID Printf_s(UINT8* origin, UINT8** pptr, UINT8 col) {
  for (UINT32 i = 0; origin[i] != 0; i++) {
    **pptr = origin[i];
    (*pptr)++;
    **pptr = col;
    (*pptr)++;
  }

  return;
}

VOID Printf_c(UINT8 origin, UINT8** pptr, UINT8 col) {
  **pptr = origin;
  (*pptr)++;
  **pptr = col;
  (*pptr)++;

  return;
}