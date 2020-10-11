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

// typedef void (*Action)();
using Action = function<void()>;
using Program = vector<pair<string, Action>>;

class Forth {
 public:
  Forth();
  Program Parse(const char* s);
  void Run(const Program& program);
  void Push(double x);
  double Pop();
  void Check();
  void Say();
  void Define(string name, Program prog);

 private:
  static constexpr int SIZE = 10;
  double stack[SIZE];
  int sp;
  map<string, Action> words;
};

extern bool FlagVerbose;

#define LOG if(FlagVerbose)fprintf
