#include "seahorn/seahorn.h"

int main (int argc, char** argv) {
	int xa = unknown();
	int ya = unknown();

	int x;
	int y;

	int loopnum1 = rand() % 20;
	__VERIFIER_assume (xa + 2 * ya >= 0);
	while (loopnum1--) {
	int loopnum2 = rand() % 10;
		x = xa + 2*ya;
		y = -2*xa + ya;

		x++;
		if (loopnum2--) y = y+x;
		else y = y-x;

		xa = x - 2*y;
		ya = 2*x + y;
		printf("%d %d\n", xa, ya);
	}


	sassert (xa + 2*ya >= 0);
	return 0;
}