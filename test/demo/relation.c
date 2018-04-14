#include "seahorn/seahorn.h"

int f(int n1){
	int r1;
	if (n1 <= 0) {
		r1 = 0;
	} else {
		r1 = f(n1 - 1) + n1;
	}
	return r1;
}

int g(int n2, int m2){
	int r2;
	r2=0;
	while (n2 > 0) {
	r2 += m2;
	n2--;
	}
	return r2;
}

int main () {
	int x1 = unknown ();
	int x2 = unknown ();
	int y2 = unknown ();
	__VERIFIER_assume (0 <= x1 && x1 <= 6);
	__VERIFIER_assume (0 <= y2 && y2 <= 6);

	if (x1 == x2 && x2 <= y2) {
		int r1 = f(x1);
		int r2 = g(x2, y2);
		sassert (r1 <= r2);
	}
	return 0;
}