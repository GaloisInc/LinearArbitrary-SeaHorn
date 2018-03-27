#include "seahorn/seahorn.h"

int make_array (int n) {
  return n;
}

int arraysize (int src) {
  return src;
}

void update (int des, int i, int x) {
  sassert (0 <= i && i < des);
}

int sub (int src, int i) {
  sassert (0 <= i && i < src); 
  return 0;
}

int bs_aux (int key, int vec, int l, int u) {
  if (u < l)
    return -1;
  else {
    int m = l + (u-l) / 2;
    int x = sub (vec, m);
    if (x < key) 
      return bs_aux (key, vec, (m+1), u);
    else if (x > key)
      return bs_aux (key, vec, l, (m-1));
     else 
      return m;
  }
}

int bsearch (int key, int vec) {
  return bs_aux (key, vec, 0, (arraysize (vec) - 1));
}

void main () {
  int n = unknown();
  int m = unknown();
  int v1 = make_array(n);
  int v2 = make_array(m);
  __VERIFIER_assume (0<=n && n==m); 
  bsearch (v1, v2);
}