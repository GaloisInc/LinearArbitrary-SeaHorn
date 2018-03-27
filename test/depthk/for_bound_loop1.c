#include "seahorn/seahorn.h"

int main() {
  int i=0, x=0, y=0;
  int n=unknown();
  x = 0;
  //__VERIFIER_assume(n>0);
  while(i<n)
  {
    x = x-y;
    sassert(x==0);
    y = unknown();
    __VERIFIER_assume(y!=0);
    x = x+y;
    sassert(x!=0);
    i++;
  }
  //__VERIFIER_assume(i>=n);
  //sassert(x==0);
}