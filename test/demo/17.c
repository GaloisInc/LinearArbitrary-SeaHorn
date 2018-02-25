#include "seahorn/seahorn.h"

extern int unknown();

int mc91 (int x) {
    if (x > 100)
    return x - 10;
    else 
    return mc91 (mc91 (x + 11));
}

int main () {
  int n = unknown ();
  sassert (n > 101 || mc91 (n) == 91);
}
