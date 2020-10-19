#include <cstdio>
#include <math.h>

#include "compiler.h"

extern void Forth_program(Zorth*);

double RandomNumber(void* ptr) {
  return (double)rand() / (double)RAND_MAX;
}

int main() {
  Zorth forth(nullptr);

  Forth_program(&forth);
  printf("\n");
  return 0;
}
