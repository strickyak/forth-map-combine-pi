#include <cstdio>
#include "f.h"

int main(int argc, const char* argv[]) {
  Forth f;
  auto prog = f.Parse(argv[1]);
  f.Run(prog);
  printf("\n");
}
