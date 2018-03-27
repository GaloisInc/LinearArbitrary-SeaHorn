#include "seahorn/seahorn.h"

int succ (int x) {
	return x+1;
}

int repeat (int n, int s) {
	if (n == 0)
		return s;
	else 
		return succ (repeat (n-1, s));
}

int main () {
	int n = unknown();
	int m = repeat (n, 0);
	sassert (m == n);
}