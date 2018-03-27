#include "seahorn/seahorn.h"

int loop (int x, int i) {
  if (i < 0)
    return x;
  else if (x < 1)
    return loop (x - 1, i - 1);
  else if (x > 2) 
    return loop (x, i - 1);
  else
    return loop (3 - x, i - 1);
}

int main () {
  int n = unknown();
  sassert (loop (3, n) >= 3);
  sassert (loop (1, n) >= 0);
}