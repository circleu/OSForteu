#include "math64.h"


UINT64 math64_div_64_by_32(UINT64 a, UINT32 b) {
  UINT64 result = 0;

  if ((UINT64)b * b <= 0xffffffff) {
    while (a >= b * b) {
      a -= b * b;
      result += b;
    }
  }

  while (a >= b) {
    a -= b;
    result++;
  }

  return result;
}

UINT64 math64_rem_64_by_32(UINT64 a, UINT32 b) {
  if ((UINT64)b * b <= 0xffffffff) {
    while (a >= b * b) {
      a -= b * b;
    }
  }
  
  while (a >= b) {
    a -= b;
  }

  return a;
}

UINT64 math64_mul_64_by_32(UINT64 a, UINT32 b) {
  UINT64 result = 0;

  if (b / 2 == 0) {
    while (b / 2 > 0) {
      result += a + a;
      b -= 2;
    }
  }

  while (b > 0) {
    result += a;
    b--;
  }

  return result;
}

UINT64 math64_div_64_by_64(UINT64 a, UINT64 b) {
  UINT64 result = 0;

  while (a >= b) {
    a -= b;
    result++;
  }

  return result;
}

UINT64 math64_rem_64_by_64(UINT64 a, UINT64 b) {
  while (a >= b) {
    a -= b;
  }

  return a;
}