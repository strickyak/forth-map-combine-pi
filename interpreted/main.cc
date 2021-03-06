#include <cstdio>
#include <math.h>
#include "f.h"

int main(int argc, const char* argv[]) {
  const char* argv0 = argv[0];

  while (argc>1 && argv[1][0]=='-') {
    if (argv[1][1]=='v') {
      FlagVerbose = 1;
    }
    argc--, argv++;
  }

  if (argc % 2 == 1) {
    fprintf(stderr, "Usage: %s name1 'def1...' name2 'def2...' ... 'runme...'\n", argv0);
    return 13;
  }

  RandomNum rn = []() { return (double)rand() / (double)RAND_MAX; };
  NumPrinter npr = [](double x) {printf("%.20g ", x); };
  StrPrinter spr = [](const string& s) {printf("%s ", s.c_str()); };
  Forth f(rn, npr, spr);

  for (int i = 1; i < argc-1; i+=2 ) {
    auto prog = f.Parse(argv[i+1]);
    f.Define(argv[i], prog);
    LOG(stderr, "==========  DEFINED: %s == %s\n", argv[i], argv[i+1]);
  }

  LOG(stderr, "==========  RUN: %s\n", argv[argc-1]);
  auto prog = f.Parse(argv[argc-1]);
  f.Run(prog);
  printf("\n");
  f.CheckEmpty();  // Error if the program did not clean up its stack.
}
