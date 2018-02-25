#include "seahorn/seahorn.h"

extern int unknown ();

int sum (int n) {
  if (n <= 0)
    return 0;
  else 
    return n + sum (n-1);
}

void main () {
  int n = unknown ();
  sassert (n <= sum (n));
}
