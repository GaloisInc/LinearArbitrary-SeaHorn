#include "seahorn/seahorn.h"

int main()
{
	int x = unknown();
	int y = unknown();
	int z = unknown();
	int i = unknown();

	__VERIFIER_assume(y > 1 && z == 1 && x > 0 && i >= x);
	while (x > 0) {
		x = x - y;
		y = y - z;
		z = -z;
		i--;
	}

  	sassert(i >= 0);
}
