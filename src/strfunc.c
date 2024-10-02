#include "strfunc.h"


UINT32 strlen(UINT8* str) {
  UINT32 len;

  for (len = 0; str[len] != 0; len++);

  return len;
}

UINT8* itoa(UINT32 num) {
  UINT32 digit = 1;
  UINT8 result[11] = {0, };

  for (; num >= (digit - 1); digit *= 10) {}
  digit /= 10;

  for (UINT32 i = 0; digit != 0; i++) {
    result[i] = num / digit + 48;

    num %= digit;
    digit /= 10;
  }

  UINT8* out = result;

  return out;
}

BOOL cmpstr(UINT8* str1, UINT8* str2) {
  for (UINT32 i = 0; str1[i] != 0 || str2[i] != 0; i++) {
    if (str1[i] != str2[i])
      return FALSE;
  }

  return TRUE;
}