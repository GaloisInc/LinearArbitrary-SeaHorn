#include "seahorn/seahorn.h"

int main () {
	int x = unknown();
	int y = unknown();
	int xa = unknown();
	int ya = unknown();

	__VERIFIER_assume(xa + 2 * ya >= 0);
	while (unknown()) {
		x = xa + 2 * ya;
		y = -2 * xa + ya;

		x++;
		if (unknown()) y = y + x;
		else y = y - x;

		xa = x - 2 * y;
		ya = 2 * x + y;
	}
	sassert(xa + 2 * ya >= 0);
	return 0;
}