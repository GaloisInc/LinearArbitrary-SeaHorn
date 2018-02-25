#include "seahorn/seahorn.h"

extern int unknown();

void main() {
  int x = 0;
  int y = unknown ();
   
  while (y != 0) {
    if (y < 0) {
      x = x - 1;
      y = y + 1;
    } else {
      x = x + 1;
      y = y - 1;
    }
    sassert (x != 0);
  }
}
