#include "shell.h"


UINT8 CurrentDirectory[13];
UINT8 DiskOutBuffer[DISK_BUFFER_SIZE_MAX] = {0, };
UINT8 DiskInBuffer[DISK_BUFFER_SIZE_MAX] = {0, };

VOID Shell() {
  struct File file = SeekFile("root");
  CopyByte(CurrentDirectory, file.Name, 0, 0, 13);

  while (1) {
    ASM_CLI;
    ASM_IRQ0_ONLY;
    ASM_STI;

    if (Keystroke == ENTER) {
      PushCursor(0, 1);
      
      TranslateCommand(KeyboardBuffer);
      
      FlushBuffer(KeyboardBuffer);
      KeyboardBufferIndex = 0;

      ClearLine(Cursor[1]);
    }
    else if (Keystroke == BACKSPACE)
      ClearLine(Cursor[1]);
    

    Printf("%s>>%s", 0, Cursor[1], 0x07, CurrentDirectory, KeyboardBuffer);
    Keystroke = 0;

    ASM_CLI;
    ASM_IRQ_ALL;
    ASM_STI;
  }

  return; 
}

VOID TranslateCommand(UINT8* str) {
  UINT8 parsed[ARG_SIZE_MAX][ARG_SIZE_MAX];

  for (UINT32 i = 0; i < ARG_SIZE_MAX; i++) {
    for (UINT32 j = 0; j < ARG_SIZE_MAX; j++) {
      parsed[i][j] = 0;
    }
  }
  
  parsestr(str, parsed);

  if (cmpstr(parsed[0], ""))
    return;

  else if (cmpstr(parsed[0], "seek")) {
    shcmd_seek(parsed[1]);
    return;
  }

  else if (cmpstr(parsed[0], "read")) {
    shcmd_read(parsed[1]);
    return;
  }

  else if (cmpstr(parsed[0], "write")) {
    shcmd_write(parsed[1], parsed[2]);
    return;
  }

  else if (cmpstr(parsed[0], "clr")) {
    FlushBuffer(ScreenBuffer);
    SetCursor(0, 0);
    return;
  }

  else {
    PrintfLn("Couldn't find command \'%s\'.", 0x03, KeyboardBuffer);
    return;
  }
}

VOID shcmd_seek(UINT8* arg0) {
  ASM_STI;
  Timer = 0;

  struct File file = SeekFile(arg0);

  ASM_CLI;
  PrintfLn("File Name: %s | Seek Time: %d", 0x03, file.Name, Timer);
  return;
}

VOID shcmd_read(UINT8* arg0) {
  ASM_STI;
  Timer = 0;

  initdiskbuffer(DiskOutBuffer);
  ReadFile(arg0, DiskOutBuffer);

  ASM_CLI;
  PrintLn(DiskOutBuffer, 0x03);
  PrintfLn("Read Time: %d", 0x03, Timer);
  return;
}

VOID shcmd_write(UINT8* arg0, UINT8* arg1) {
  ASM_STI;
  Timer = 0;

  initdiskbuffer(DiskInBuffer);
  CopyByte(DiskInBuffer, arg1, 0, 0, ARG_SIZE_MAX);
  WriteFile(arg0, DiskInBuffer);

  ASM_CLI;
  PrintfLn("Write Time: %d", 0x03, Timer);
  return;
}

VOID parsestr(UINT8* str, UINT8 dest[ARG_SIZE_MAX][ARG_SIZE_MAX]) {
  UINT32 row = 0;
  UINT32 col = 0;
  UINT32 i = 0;

  for (; str[i] == ' '; i++);

  for (; str[i] != 0; i++) {
    if (str[i] == '\'') {
      for (++i; str[i] != '\'' && str[i] != 0; i++) {
        dest[row][col++] = str[i];
      }
    }
    else if (str[i] == ' ') {
      for (; str[i + 1] == ' '; i++);

      row++;
      col = 0;
    }
    else {
      dest[row][col++] = str[i];
    }
  }
}

VOID initdiskbuffer(UINT8* buffer) {
  for (UINT32 i = 0; i < DISK_BUFFER_SIZE_MAX; i++) {
    buffer[i] = 0;
  }

  return;
}