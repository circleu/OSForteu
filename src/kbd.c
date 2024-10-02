#include "kbd.h"


UINT8 TranslateKey(UINT8 origin) {
  UINT8 result;
  static UINT8 shift_flag;

  if (shift_flag == TRUE) {
    if (origin == 0x1e) result = 'A';
    else if (origin == 0x30) result = 'B';
    else if (origin == 0x2e) result = 'C';
    else if (origin == 0x20) result = 'D';
    else if (origin == 0x12) result = 'E';
    else if (origin == 0x21) result = 'F';
    else if (origin == 0x22) result = 'G';
    else if (origin == 0x23) result = 'H';
    else if (origin == 0x17) result = 'I';
    else if (origin == 0x24) result = 'J';
    else if (origin == 0x25) result = 'K';
    else if (origin == 0x26) result = 'L';
    else if (origin == 0x32) result = 'M';
    else if (origin == 0x31) result = 'N';
    else if (origin == 0x18) result = 'O';
    else if (origin == 0x19) result = 'P';
    else if (origin == 0x10) result = 'Q';
    else if (origin == 0x13) result = 'R';
    else if (origin == 0x1f) result = 'S';
    else if (origin == 0x14) result = 'T';
    else if (origin == 0x16) result = 'U';
    else if (origin == 0x2f) result = 'V';
    else if (origin == 0x11) result = 'W';
    else if (origin == 0x2d) result = 'X';
    else if (origin == 0x15) result = 'Y';
    else if (origin == 0x2c) result = 'Z';
    else if (origin == 0x02) result = '!';
    else if (origin == 0x03) result = '@';
    else if (origin == 0x04) result = '#';
    else if (origin == 0x05) result = '$';
    else if (origin == 0x06) result = '%';
    else if (origin == 0x07) result = '^';
    else if (origin == 0x08) result = '&';
    else if (origin == 0x09) result = '*';
    else if (origin == 0x0a) result = '(';
    else if (origin == 0x0b) result = ')';
    else if (origin == 0x0c) result = '_';
    else if (origin == 0x39) result = ' ';

    else if (origin == 0x0e) result = BACKSPACE;
    else if (origin == 0x1c) result = ENTER;
    else if (origin == 0x2a) {
      shift_flag = TRUE;
      result = 0xff;
    }
    else if (origin == 0xaa) {
      shift_flag = FALSE;
      result = 0xff;
    }
    else result = 0xff;
  }
  else if (shift_flag == FALSE) {
    if (origin == 0x1e) result = 'a';
    else if (origin == 0x30) result = 'b';
    else if (origin == 0x2e) result = 'c';
    else if (origin == 0x20) result = 'd';
    else if (origin == 0x12) result = 'e';
    else if (origin == 0x21) result = 'f';
    else if (origin == 0x22) result = 'g';
    else if (origin == 0x23) result = 'h';
    else if (origin == 0x17) result = 'i';
    else if (origin == 0x24) result = 'j';
    else if (origin == 0x25) result = 'k';
    else if (origin == 0x26) result = 'l';
    else if (origin == 0x32) result = 'm';
    else if (origin == 0x31) result = 'n';
    else if (origin == 0x18) result = 'o';
    else if (origin == 0x19) result = 'p';
    else if (origin == 0x10) result = 'q';
    else if (origin == 0x13) result = 'r';
    else if (origin == 0x1f) result = 's';
    else if (origin == 0x14) result = 't';
    else if (origin == 0x16) result = 'u';
    else if (origin == 0x2f) result = 'v';
    else if (origin == 0x11) result = 'w';
    else if (origin == 0x2d) result = 'x';
    else if (origin == 0x15) result = 'y';
    else if (origin == 0x2c) result = 'z';
    else if (origin == 0x02) result = '1';
    else if (origin == 0x03) result = '2';
    else if (origin == 0x04) result = '3';
    else if (origin == 0x05) result = '4';
    else if (origin == 0x06) result = '5';
    else if (origin == 0x07) result = '6';
    else if (origin == 0x08) result = '7';
    else if (origin == 0x09) result = '8';
    else if (origin == 0x0a) result = '9';
    else if (origin == 0x0b) result = '0';
    else if (origin == 0x0c) result = '-';
    else if (origin == 0x39) result = ' ';

    else if (origin == 0x0e) result = BACKSPACE;
    else if (origin == 0x1c) result = ENTER;
    else if (origin == 0x2a) {
      shift_flag = TRUE;
      result = 0xff;
    }
    else if (origin == 0xaa) {
      shift_flag = FALSE;
      result = 0xff;
    }
    else result = 0xff;
  }

  return result;
}

VOID KeyboardController() {
  if (Keystroke == BACKSPACE && KeyboardBufferIndex != 0)
    KeyboardBuffer[--KeyboardBufferIndex] = 0;
  else if (Keystroke != 0xff && Keystroke != BACKSPACE && Keystroke != ENTER)
    KeyboardBuffer[KeyboardBufferIndex++] = Keystroke;
    
  return;
}