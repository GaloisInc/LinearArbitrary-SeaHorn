#include "seahorn/seahorn.h"

int f (int x) {
  if (x < -2)
    return f (-3);
  else if (x < 2)
    return 2 * x - 1;
  else if (x <= 2)
    return f (2 * x - 1);
  else
    return x;
}

void main () {
  sassert(f (3) >= 0);
}
