#include "shell.h"


UINT8 CurrentDirectory[13];
UINT8 DiskOutBuffer[DISK_BUFFER_SIZE_MAX] = {0, };
UINT8 DiskInBuffer[DISK_BUFFER_SIZE_MAX] = {0, };

VOID Shell() {
  UINT32 offset;
  struct File file = SeekFile("root", &offset);
  CopyByte(CurrentDirectory, file.Name, 0, 0, 13);

  while (1) {
    ASM_CLI;

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
    if (cmpstr(parsed[1], "")) {
      PrintLn("Usage: seek <file name>", 0x03);
      return;
    }

    shcmd_seek(parsed[1]);
    return;
  }

  else if (cmpstr(parsed[0], "read")) {
    if (cmpstr(parsed[1], "")) {
      PrintLn("Usage: read <file name>", 0x03);
      return;
    }

    shcmd_read(parsed[1]);
    return;
  }

  else if (cmpstr(parsed[0], "write")) {
    if (cmpstr(parsed[1], "") || cmpstr(parsed[2], "")) {
      PrintLn("Usage: write <file name> <text>", 0x03);
      return;
    }

    shcmd_write(parsed[1], parsed[2]);
    return;
  }

  else if (cmpstr(parsed[0], "create")) {
    if (cmpstr(parsed[1], "") || cmpstr(parsed[2], "") || cmpstr(parsed[3], "")) {
      PrintLn("Usage: create <file name> <type>[file / dir] <read-only>[true / false]", 0x03);
      return;
    }

    shcmd_create(parsed[1], parsed[2], parsed[3]);
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

  UINT32 offset;
  struct File file = SeekFile(arg0, &offset);

  ASM_CLI;
  PrintfLn("File Name: %s | Seek Time: %d", 0x03, file.Name, Timer);
  return;
}

VOID shcmd_read(UINT8* arg0) {
  ASM_STI;
  Timer = 0;

  FlushDiskBuffer(DiskOutBuffer);
  ReadFile(arg0, DiskOutBuffer);

  ASM_CLI;
  PrintLn(DiskOutBuffer, 0x03);
  PrintfLn("Read Time: %d", 0x03, Timer);
  return;
}

VOID shcmd_write(UINT8* arg0, UINT8* arg1) {
  ASM_STI;
  Timer = 0;

  FlushDiskBuffer(DiskInBuffer);
  CopyByte(DiskInBuffer, arg1, 0, 0, ARG_SIZE_MAX);
  WriteFile(arg0, DiskInBuffer);

  ASM_CLI;
  PrintfLn("Write Time: %d", 0x03, Timer);
  return;
}

VOID shcmd_create(UINT8* arg0, UINT8* arg1, UINT8* arg2) {
  ASM_STI;
  Timer = 0;

  CreateFile(arg0, arg1, arg2);

  ASM_CLI;
  PrintfLn("Create Time: %d", 0x03, Timer);
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

VOID FlushDiskBuffer(UINT8* buffer) {
  for (UINT32 i = 0; i < DISK_BUFFER_SIZE_MAX; i++) {
    buffer[i] = 0;
  }

  return;
}