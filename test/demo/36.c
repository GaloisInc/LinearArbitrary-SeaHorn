#include "seahorn/seahorn.h"

int main () {
	int y = unknown();
	int z = unknown();
	int m = unknown();

	__VERIFIER_assume (y < z && m > 0);

	while (y < z) {
		y += 2*m;
	}

	int x = 0;

	while (y > z) {
		x ++;
		z += 2;
	}

	sassert (x <= m);
}