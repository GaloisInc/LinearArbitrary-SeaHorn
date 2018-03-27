#include "seahorn/seahorn.h"

int f (int x) {
  if (x < -1)
    return f (-2);
  else if (x <= 1)
    return 2 * x - 1;
  else
    return x;
}

void main () {
  int n = unknown();
  __VERIFIER_assume (n >= 2);
  sassert(f(n) >= 0);
}