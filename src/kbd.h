#pragma once

#include "osforteutype.h"
#include "osforteudata.h"

#define BACKSPACE 0x08
#define ENTER 0x13


UINT8 TranslateKey(UINT8 origin);
VOID KeyboardController();