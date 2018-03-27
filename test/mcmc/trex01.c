#include "seahorn/seahorn.h"

void main() {
  int d = unknown();
  int x, y, k, z = 1;
  int c;
  L1:
  while (z < k) {
    z = 2 * z; 
  }
  sassert(z>=1);

  L2:
  while (x > 0 && y > 0) {
    c = unknown();
    if (c) {
      P1:
      x = x - d;
      y = unknown();
      z = z - 1;
    } else {
      y = y - d;
    }
  }
}
