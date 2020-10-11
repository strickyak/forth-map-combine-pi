#include <cstdio>
#include "f.h"

void Test(const map<string, string>& defs, const char* prog, const char* want) {
  LOG(stderr, "################### TEST: %s\n", prog);

  string output;

  NumPrinter npr = [&](double x) {char buf[1000]; sprintf(buf, "%.20g ", x); output += buf; };
  StrPrinter spr = [&](const string& s) {char buf[1000]; sprintf(buf, "%s ", s.c_str()); output += buf; };
  Forth f(npr, spr);
  for (const auto& [name, prog] : defs) {
    f.Define(name, f.Parse(prog.c_str()));
  }
  f.Run(f.Parse(prog));

  LOG(stderr, "################### GOT: {%s}\n", output.c_str());
  LOG(stderr, "################### WANT: {%s}\n", want);
  if (output != want) {
    fprintf(stderr, "###### FAILS ######\n");
    exit(3);
  }
  f.CheckEmpty();
}

int main(int argc, const char* argv[]) {
  FlagVerbose = 1;

  map<string, string> none;
  Test(none, "1 2 3 4 + + + dup * .", "100 ");

  // do loop
  Test(none, "0 101 1 do i + loop .", "5050 ");
  Test(none, "3 0 do 8 5 do j 10 * i + . loop loop `ok", "5 6 7 15 16 17 25 26 27 ok ");
  Test(none, "0 10 0 do 10 0 do j 10 * i + + loop loop .", "4950 ");

  // cond: if-true if-false
  Test(none, "123 . 4 5 < cond: 88 33 . 12345 .", "123 88 12345 ");
  Test(none, "123 . 4 5 >= cond: 88 33 . 12345 .", "123 33 12345 ");
  Test(none, "`begin 4 5 >= cond: `yes `no `end", "begin no end ");

  // while: pred body
  {
    map<string, string> defs = { {"pred1", "1 - dup 0 <"}, {"body1", "dup ."}, };
    Test(defs, "`begin 10 while: pred1 body1 drop `end", "begin 9 8 7 6 5 4 3 2 1 0 end ");
  }

  // recursion
  {
    map<string, string> defs = {
      {"factorial", "dup 0 <= cond: one recurse"},
      {"one", "drop 1"},
      {"recurse", "dup 1 - factorial *"},
    };
    Test(defs, "10 factorial .", "3628800 ");
  }

  // tail recursion (not optimized).
  {
    map<string, string> defs = {
      {"countdown", "dup . 1 - dup 0 > cond: countdown drop"},
    };
    Test(defs, "10 countdown", "10 9 8 7 6 5 4 3 2 1 ");
  }
}
