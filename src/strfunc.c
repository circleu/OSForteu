#include "strfunc.h"


UINT32 strlen(UINT8* str) {
  UINT32 len;

  for (len = 0; str[len] != 0; len++);

  return len;
}

UINT8* itoa(UINT64 num) {
  UINT64 digit = 1;
  static UINT8 result[21] = {0, };

  for (UINT32 i = 0; i < 21; i++) {
    result[i] = 0;
  }

  for (; num >= (digit - 1);) {
    digit = math64_mul_64_by_32(digit, 10);
  }
  digit = math64_div_64_by_32(digit, 10);

  for (UINT32 i = 0; digit != 0; i++) {
    result[i] = math64_div_64_by_64(num, digit) + 48;

    num = math64_rem_64_by_64(num, digit);
    digit = math64_div_64_by_32(digit, 10);
  }

  return result;
}

BOOL cmpstr(UINT8* str1, UINT8* str2) {
  for (UINT32 i = 0; str1[i] != 0 || str2[i] != 0; i++) {
    if (str1[i] != str2[i])
      return FALSE;
  }

  return TRUE;
}