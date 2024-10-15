#include "shell.h"

VOID Shell() {
  struct File file = SeekFile("root");
  UINT8 CurrentDirectory[13];
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
    ASM_STI;

    struct File file = SeekFile(parsed[1]);

    ASM_CLI;
    return;
  }

  else if (cmpstr(parsed[0], "read")) {
    ASM_STI;

    struct File file = SeekFile(parsed[1]);
    UINT8 buffer[4096] = {0, };


    ASM_CLI;
  }

  else if (cmpstr(parsed[0], "print")) {
    PrintLn(parsed[1], 0x03);
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