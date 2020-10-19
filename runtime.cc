#include <cstdio>
#include <math.h>

#include "runtime.h"

// The immediate part of the .4th file.
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
