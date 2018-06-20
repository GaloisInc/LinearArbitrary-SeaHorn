#include "seahorn/seahorn.h"

int main () {
	int x = unknown();
	int y = unknown();
	int z = unknown();
	__VERIFIER_assume(x + y + z >= 0);
	while (z> 0) {
		x++;
		y++;
		z -= 2;
	}

	sassert(x + y >= 0);
	return 0;
}