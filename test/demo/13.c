#include "seahorn/seahorn.h"

extern int unknown ();

int mult (int n, int m) {
  if (m == 0)
    return 0;
  else if (m < 0)
    return - n + mult (n, (m+1));
  else
    return n + mult (n, (m-1));
}

void  main () {
  int n = unknown ();
  int m = unknown ();
  if (n < 0 && m > 0) {
    int t1 = mult (n, m);
    int t2 = mult (n, t1);
    sassert (t2 >= 0);
  }
}


