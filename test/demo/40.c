#include "seahorn/seahorn.h"

int main() {
	int x = 0;
	int y = 0;

	while (unknown()) {
		x += x;
		y += 1;
	}


	x += y;

	int z = -x;

	while (unknown()) {
		z--;
		sassert (z <= 0);
	}

}