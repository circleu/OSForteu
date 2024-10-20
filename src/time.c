#include "time.h"


VOID EnableRTC() {
  UINT8 temp;

  ASM_CLI;
  ASM_OUTB(CMOS_ADDRESS, (0x80 + RTC_REGISTER_B));
  ASM_INB(temp, CMOS_DATA);
  ASM_OUTB(CMOS_ADDRESS, (0x80 + RTC_REGISTER_B));
  ASM_OUTB(CMOS_DATA, (temp | 0x40));
  ASM_STI;

  ASM_OUTB(CMOS_ADDRESS, RTC_REGISTER_C);
  ASM_INB(temp, CMOS_DATA);

  return;
}

BOOL RTC_IsUpdate() {
  UINT8 status;
  
  ASM_OUTB(CMOS_ADDRESS, RTC_REGISTER_A);
  ASM_INB(status, CMOS_DATA);

  if ((status & 0x80) == 0x80)
    return TRUE;
  else
    return FALSE;
}

UINT8 RTC_GetRegister(UINT8 reg) {
  UINT8 value;

  ASM_OUTB(CMOS_ADDRESS, reg);
  ASM_INB(value, CMOS_DATA);

  return value;
}

VOID RTC_GetTime(struct Date* time) {
  UINT8 reg_b;

  while (RTC_IsUpdate());

  ASM_OUTB(CMOS_ADDRESS, (0x80 + RTC_REGISTER_B));
  ASM_INB(reg_b, CMOS_DATA);

  time->Seconds = RTC_GetRegister(RTC_REGISTER_SECONDS);
  time->Minutes = RTC_GetRegister(RTC_REGISTER_MINUTES);
  time->Hours = RTC_GetRegister(RTC_REGISTER_HOURS);
  time->Day = RTC_GetRegister(RTC_REGISTER_DAY);
  time->Month = RTC_GetRegister(RTC_REGISTER_MONTH);
  time->Year = RTC_GetRegister(RTC_REGISTER_YEAR);

  if (!(reg_b & 0x04)) {
    time->Seconds = (time->Seconds & 0x0F) + ((time->Seconds / 16) * 10);
    time->Minutes = (time->Minutes & 0x0F) + ((time->Minutes / 16) * 10);
    time->Hours = ((time->Hours & 0x0F) + (((time->Hours & 0x70) / 16) * 10)) | (time->Hours & 0x80);
    time->Day = (time->Day & 0x0F) + ((time->Day / 16) * 10);
    time->Month = (time->Month & 0x0F) + ((time->Month / 16) * 10);
    time->Year = (time->Year & 0x0F) + ((time->Year / 16) * 10);
  }

  if (!(reg_b & 0x02) && (time->Hours & 0x80)) {
    time->Hours = ((time->Hours & 0x7F) + 12) % 24;
  }

  return;
}