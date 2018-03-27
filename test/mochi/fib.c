#include "seahorn/seahorn.h"

int fib (int n) { 
  if (n<2) 
  	return 1;
  else
    return fib (n-1) + fib (n-2);
}

void main () {
	int n = unknown();
  	sassert (n <= fib (n));
}
