#include "seahorn/seahorn.h"

int unknown();
int main() { 

  int fail=0;
  int x=unknown();
  __VERIFIER_assume(x>0);
  while(x>0) {
//    __ESBMC_assume(fail!=1);
    if(fail==1) {
      x= -1;
      fail=2;
    } else {
      x--;
    }
  }
  sassert(x==0);
}