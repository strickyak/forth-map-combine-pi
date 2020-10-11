// Simple stand-alone C code for simple series.
// MIT License  2020  github!strickyak

#include <stdio.h>
#include <math.h>

// https://mathworld.wolfram.com/GregorySeries.html
//
// pi := 4 * ( 1 - 1/3 + 1/5 - 1/7 + 1/9 ... )

void GregorySeries() {
  double z = 1.0;
  double sign = -1.0;

  for (int i = 3; 1; i+=2) {
    z += sign * (1.0 / (double)i);
    printf("%3d: %.20g\n", i, 4.0 * z);
    sign = (-sign);
  }
}

// An improvement that I discovered as a teenager:
// print the average of each pair in the series.

void AverageOfPairsGregorySeries() {
  double prev = 1.0;
  double z = 1.0;
  double sign = -1.0;

  for (int i = 3; 1; i+=2) {
    z += sign * (1.0 / (double)i);
    printf("%3d: %.20g\n", i, 2.0 * (z + prev));
    prev = z;
    sign = (-sign);
  }
}

// Formula (10) at https://mathworld.wolfram.com/PiFormulas.html
//
// pi := (4.0 / sqrt(2.0)) * ( + 1/1 + 1/3 - 1/5 - 1/7 + 1/9 + 1/11 - 1/13 - 1/15 ...)

void formula10() {
  double z = 1.0;
  double sign = 1.0;
  double k = 4.0 / sqrt(2.0);

  for (int i = 3; 1; i+=4) {
    z += sign * (1.0 / (double)i);
    z -= sign * (1.0 / (double)(i+2));
    sign = (-sign);
    printf("%3d: %.20g\n", i, k * z);
  }
}

int main(int argc, char* argv[]) {
  if (argc==2) {
    switch (argv[1][0]) {
      case '1': GregorySeries(); break;
      case '2': AverageOfPairsGregorySeries(); break;
      case '3': formula10(); break;
    };
  };
  fprintf(stderr, "Usage:   %s [1|2|3]\n", argv[0]);
  return 13;
}
