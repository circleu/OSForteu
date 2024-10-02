#include "main.h"

VOID main() {
  SetIDT();
  ASM_STI;

  while (1);

  return;
}