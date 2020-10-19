#include <assert.h>

extern double RandomNumber(void*);

struct Zorth {
  Zorth(void* rp) : sp(1), rand_ptr(rp) { CheckEmpty(); }
  ~Zorth() { CheckEmpty(); }

  double Pop() { return stack[--sp]; }
  void Push(double x) { stack[sp++] = x; }

  double Peek() { return stack[sp-1]; }
  void Poke(double x) { stack[sp-1] = x; }

  double PeekN(int n) { return stack[sp-n]; }
  void PokeN(int n, double x) { stack[sp-n] = x; }

  void Check() {
#if 0
    assert(1 <= sp);
    assert(sp <= SIZE-2);
#endif
  }
  void CheckEmpty() {
#if 0
    assert(1 == sp);
#endif
  }

  inline static constexpr int SIZE = 16;
  double stack[SIZE];
  int sp;
  //double rstack[SIZE];
  //int rsp;
  void* rand_ptr;
};

