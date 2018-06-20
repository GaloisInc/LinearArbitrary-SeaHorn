#include "seahorn/seahorn.h"

int main () {
	int A = unknown();
	int B = unknown();
	int a = A;
	int b = B;

	__VERIFIER_assume (A > 0 && B > 0);

	while (a != b) {
		if (a > b)
			a -= b;
		else
			b -= a;
	}

	sassert (a >= 1);
	sassert (a <= A);

	sassert (b >= 1);
	sassert (b <= B);
}