#pragma once

#include "type.h"
#include "data.h"

#define BACKSPACE 0x08
#define ENTER 0x13


UINT8 TranslateKey(UINT8 origin);
VOID KeyboardController();