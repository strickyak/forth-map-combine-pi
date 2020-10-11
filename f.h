#include <cstdio>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

using std::function;
using std::map;
using std::pair;
using std::string;
using std::vector;

using Action = function<void()>;
using Program = vector<pair<string, Action>>;
using NumPrinter = function<void(double)>;
using StrPrinter = function<void(const string&)>;

class Forth {
 public:
  Forth(NumPrinter npr, StrPrinter spr);
  Program Parse(const char* s);
  void Run(const Program& program);
  void Push(double x);
  double Pop();
  void RPush(double x);
  double RPop();
  void Check();
  void CheckEmpty();
  void DebugShow();
  void Define(string name, Program prog);

 private:
  static constexpr int SIZE = 64;
  double rstack[SIZE];
  double stack[SIZE];
  int sp, rsp, pc;
  const Program* prog;
  map<string, Action> words;
  NumPrinter num_print;
  StrPrinter str_print;
};

extern bool FlagVerbose;

#define LOG if(FlagVerbose)fprintf
