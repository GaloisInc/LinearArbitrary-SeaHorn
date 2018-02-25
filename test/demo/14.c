#include "seahorn/seahorn.h"

extern int unknown();

int add (int x, int y) {
  if (y <= 0)
    return x;
  else 
    return 1 + add (x, (y-1));
}

int sum (int x) {
  if (x <= 0) 
    return 0; 
  else 
    return add (x, (sum (x-1)));
}

void main () {
  int n = unknown();
  sassert (0 <= sum (n));
}
