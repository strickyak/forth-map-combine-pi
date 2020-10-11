#include "f.h"
#include <assert.h>
#include <math.h>

bool FlagVerbose;

Forth::Forth(NumPrinter npr, StrPrinter spr)
  : num_print(npr), str_print(spr)
{
  rsp = sp = 1, pc = 0;
  for (int i = 0; i < SIZE; i++ ) rstack[i] = stack[i] = 0.0;
  words = map<string, Action>({
    {"dup", [this]() { Push(stack[sp-1]); }},
    {".", [this]() { num_print(Pop()); }},
    {"+", [this]() { Push(Pop() + Pop()); }},
    {"*", [this]() { Push(Pop() * Pop()); }},
    {"-", [this]() { double b=Pop(); double a=Pop(); Push(a - b); }},
    {"/", [this]() { double b=Pop(); double a=Pop(); Push(a / b); }},
    {"%", [this]() { double b=Pop(); double a=Pop(); Push(fmod(a, b)); }},
    {"<", [this]() { double b=Pop(); double a=Pop(); Push(a < b); }},
    {"<=", [this]() { double b=Pop(); double a=Pop(); Push(a <= b); }},
    {"=", [this]() { double b=Pop(); double a=Pop(); Push(a == b); }},
    {"<>", [this]() { double b=Pop(); double a=Pop(); Push(a != b); }},
    {">", [this]() { double b=Pop(); double a=Pop(); Push(a > b); }},
    {">=", [this]() { double b=Pop(); double a=Pop(); Push(a >= b); }},
    {"swap", [this]() { double b=Pop(); double a=Pop(); Push(b); Push(a); }},
    {"rot", [this]() { double c=Pop(); double b=Pop(); double a=Pop(); Push(b); Push(c); Push(a); }},
    {"drop", [this]() { (void) Pop(); }},
    {"i", [this]() { double i = rstack[rsp-1]; Push(i); }},
    {"j", [this]() { double j = rstack[rsp-4]; Push(j); }},
    {"do", [this]() { double start=Pop(); double limit=Pop(); RPush(pc); RPush(limit); RPush(start); }},
    {"loop", [this]() {
        double i= 1.0 + RPop(); double limit=RPop(); double jump=RPop();
        if (i >= limit) return;
        RPush(jump); RPush(limit); RPush(i);
        pc = jump;
                      }},
    {"cond:", [this]() {
                         // USAGE: ...pred cond: y n
        auto chosen = Program({(*prog)[pc+1+(Pop()==0.0)]});
        LOG(stderr, "chosen=<%s>\n", chosen[0].first.c_str());
        Run(chosen);
        pc += 2;
                      }},
    {"while:", [this]() {
                         // USAGE: while: pred body
        auto pred = Program({(*prog)[pc+1]});
        auto body = Program({(*prog)[pc+2]});
        while (1) {
          Run(pred);
          if (Pop()) break;
          Run(body);
        }
        pc += 2;
                      }},
  });
  CheckEmpty();
}

void Forth::Define(string name, Program prog) {
  words[name] = [this, prog]() {
    Run(prog);
  };
}

static bool parseNum(const char* s, double*p) {
  bool has_digit = false;
  for (const char* t=s; *t; t++) {
    if (!(*t=='-' || *t=='.' || '0'<=*t && *t<='9')) return false;
    if ('0'<=*t && *t<='9') has_digit = true;
  }
  if (!has_digit) return false;
  *p = atof(s);
  return true;
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
    LOG(stderr, "compiling: {%s}\n", buf.c_str());

    Action act;
    double x;
    if (parseNum(buf.c_str(), &x)) {
      act = [this, x](){ Push(x); };
    } else if (buf.length() > 1 && buf[0]=='`') {
      act = [this, buf](){ str_print(buf.substr(1)); };
    } else {
      act = words[buf];
      if (!act) {
        // A lambda to runtime-bind the action.
        act = [this, buf](){
          Action a = words[buf];
          assert(a);
          Check();
          a();
          Check();
        };
      }
    }
    assert(act);
    prog.push_back(pair{buf, act});
  }
  return prog;
}

void Forth::Check() {
  assert(sp >= 1);      // reserve [0]
  assert(sp < SIZE-1);  // reserve [SIZE-1]
  assert(rsp >= 1);      // reserve [0]
  assert(rsp < SIZE-1);  // reserve [SIZE-1]
}

void Forth::CheckEmpty() {
  assert(sp == 1);
  assert(rsp == 1);
}

void Forth::Run(const Program& program) {
  Check();
  double save_pc = pc;
  const Program* save_prog = prog;
  prog = &program;
  for (pc=0; pc<program.size(); pc++) {
    const auto& [name, act] = program[pc];
    LOG(stderr, "----------  %s\n", name.c_str());
    act();
    Check();
    DebugShow();
  }
  prog = save_prog;
  pc = save_pc;
}

void Forth::DebugShow() {
  LOG(stderr, "r={ ");
  for (int i = 1; i < rsp; i++) {
    LOG(stderr, "%.18g ", rstack[i]);
  }
  LOG(stderr, "}   s={ ");
  for (int i = 1; i < sp; i++) {
    LOG(stderr, "%.18g ", stack[i]);
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

void Forth::RPush(double x) {
  rstack[rsp++] = x;
  Check();
}

double Forth::RPop() {
  double z = rstack[--rsp];
  Check();
  return z;
}
