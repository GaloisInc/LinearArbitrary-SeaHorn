#include "seahorn/seahorn.h"

int main () {
	int x = 0;
	int y = 0;
	int z = 0;

	while (x < 100) {
		x = x + y;
		y = z + 1;
		z = y - 1;
		sassert (x >= 0);
	}
}
