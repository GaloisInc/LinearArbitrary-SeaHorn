#include "seahorn/seahorn.h"

int main () {
	int c = 0;
	int i = 0;
	int N = unknown();
	__VERIFIER_assume (N > 0);

	while (c < N) {
		c++;
		i+=2;
	}

	c = 0;
	int N1 = unknown();
	__VERIFIER_assume (N1 < N);

	while (c < N1) {
		c++;
		i++;
	}

	sassert (i >= 2*N);
}