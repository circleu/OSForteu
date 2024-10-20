#pragma once

#include <stdint.h>

#define TRUE 1
#define FALSE 0


typedef void VOID;
typedef uint8_t BOOL;
typedef uint8_t UINT8;
typedef uint16_t UINT16;
typedef uint32_t UINT32;
typedef uint64_t UINT64;

struct Date {
  UINT16 Year;
  UINT8 Month;
  UINT8 Day;
  UINT8 Hours;
  UINT8 Minutes;
  UINT8 Seconds;
}__attribute__((packed));