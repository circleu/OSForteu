#pragma once

#include "type.h"


// This thing is so fking slow!!! We need to think about a solution for optimization, or re-make the codes.
// It takes about 5 seconds to perform the div_64_by_64 operation!!!
// Do I have to migrate to 64-bit???
UINT64 math64_div_64_by_32(UINT64 a, UINT32 b);
UINT64 math64_rem_64_by_32(UINT64 a, UINT32 b);
UINT64 math64_mul_64_by_32(UINT64 a, UINT32 b);

UINT64 math64_div_64_by_64(UINT64 a, UINT64 b);
UINT64 math64_rem_64_by_64(UINT64 a, UINT64 b);