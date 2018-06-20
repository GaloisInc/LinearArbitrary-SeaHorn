#include "seahorn/seahorn.h"

int main () {
	int x1 = 0;
	int x3 = unknown();
	int y = unknown();

	__VERIFIER_assume (y > 0 && y < 5 && x3 == 3*y);

	while (x1 < 200) {
		x1 ++;
		x3 ++;
		sassert (x3 >= 3 && x3 <= 212);
	}
}