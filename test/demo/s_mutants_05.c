#include "seahorn/seahorn.h"

int main () {
	int x1 = 0;
	int x3 = unknown();
	__VERIFIER_assume(x3 > x1);

	while (unknown()) {
		x1 ++;
		x3 += 2;
	}
	if (x1 > 500)
		sassert (x3 > 1000);
}