#pragma once

#include "type.h"
#include "strfunc.h"
#include "asm.h"
#include "data.h"
#include "kbd.h"
#include "scr.h"

#define ARG_SIZE_MAX 0xff


VOID Shell();
VOID TranslateCommand(UINT8* str);

VOID parsestr(UINT8* str, UINT8 dest[ARG_SIZE_MAX][ARG_SIZE_MAX]);