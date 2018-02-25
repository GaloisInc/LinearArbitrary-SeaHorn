#include "seahorn/seahorn.h"

extern int unknown();

int  mult (int n, int m) {
    if (n <= 0 || m <= 0)
    	return 0;
    else
	return  n + mult (n, (m-1));
}

int main () {
  int n = unknown ();
  sassert (n <= mult (n, n));
}
