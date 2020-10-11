#include "f.h"
#include <assert.h>

bool FlagVerbose;

Forth::Forth() {
  for (double& d : stack) d = 0;
  for (int i = 0; i < SIZE; i++ ) stack[i] = 0.0;
  sp = 1;
  words = map<string, Action>({
    {"0", [this]() { Push(0.0); }},
    {"1", [this]() { Push(1.0); }},
    {"dup", [this]() { Push(stack[sp-1]); }},
    {"+", [this]() { Push(Pop() + Pop()); }},
    {"*", [this]() { Push(Pop() * Pop()); }},
    {".", [this]() { printf("%20f ", Pop()); }},
    {"<", [this]() {
                    double b=Pop();
                    double a=Pop();
                    Push(a < b);
                  }},
  });
}

void Forth::Define(string name, Program prog) {
  words[name] = [this, prog]() {
    Run(prog);
  };
}
Program Forth::Parse(const char* s) {
  Program prog;
  while (true) {
    while (*s && *s <= 32) { // skip white space
      s++;
    }
    if (!*s) break;

    string buf;
    while (*s > 32) {
      buf.push_back(*s++);
    }
    Action act = words[buf];
    assert(act);
    prog.push_back(pair{buf, act});
    LOG(stderr, "compiled: `%s`\n", buf.c_str());
  }
  return prog;
}

void Forth::Check() {
  assert(sp >= 1);      // reserve [0]
  assert(sp < SIZE-1);  // reserver [SIZE-1]
}

void Forth::Run(const Program& program) {
  Check();
  for (auto step : program) {
    const auto [name, act] = step;
    LOG(stderr, "----------  %s\n", name.c_str());
    act();
    Check();
    Say();
  }
}

void Forth::Say() {
  LOG(stderr, "{ ");
  for (int i = 1; i < sp; i++) {
    LOG(stderr, "%g ", stack[i]);
  }
  LOG(stderr, "}\n");
}

void Forth::Push(double x) {
  stack[sp++] = x;
  Check();
}

double Forth::Pop() {
  double z = stack[--sp];
  Check();
  return z;
}
