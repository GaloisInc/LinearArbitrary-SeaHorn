#include "seahorn/seahorn.h"

int main() {
  int i=0, x=0, y=0, n;
  __VERIFIER_assume(n > 0);
  
  i = 0;
  while ( 1 ) {
    sassert(x == 0);
    i++;
  }
  sassert (x == 0);
  return 0;
}
