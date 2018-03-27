#include "seahorn/seahorn.h"

int main() {

  int x=unknown();
  __VERIFIER_assume(x>0);
  int y=unknonw();
  __VERIFIER_assume(y>=0);
  __VERIFIER_assume(y<x);
  int z=0;

  while(x>0) {
      x--;
      if(x==y) z=1; // assert(0);
  }
  sassert(z==1);
}