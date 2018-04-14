#include "seahorn/seahorn.h"
int main () {
	int x = unknown();
	int a = 0 , s = 1 , t = 1;
	while (s <= x) {
		a += 1;
		t += 2;
		s += t;
	}
	sassert (t < 10 || s >= 30);
	return 0;
}