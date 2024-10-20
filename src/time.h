#pragma once

#include "type.h"
#include "asm.h"
#include "data.h"

#define CMOS_ADDRESS 0x70
#define CMOS_DATA 0x71

#define RTC_REGISTER_SECONDS 0x00
#define RTC_REGISTER_MINUTES 0x02
#define RTC_REGISTER_HOURS 0x04
#define RTC_REGISTER_WEEKDAY 0x06
#define RTC_REGISTER_DAY 0x07
#define RTC_REGISTER_MONTH 0x08
#define RTC_REGISTER_YEAR 0x09
#define RTC_REGISTER_A 0x0a
#define RTC_REGISTER_B 0x0b
#define RTC_REGISTER_C 0x0c


VOID EnableRTC();
BOOL RTC_IsUpdate();
UINT8 RTC_GetRegister(UINT8 reg);

VOID RTC_GetTime(struct Date* time);