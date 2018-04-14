#include "seahorn/seahorn.h"
int f(int x) {
	int r = 0;
	while (x > 0) {
		r = r + x; x--; }
	return r;
}

int g(int x) {
	if (x > 0)
		return x + g(x-1);
	else return x;
}

int main() {
	int x = unknown();
	__VERIFIER_assume (x >= 0 && x < 9);
	sassert (f(x) == g(x));
}