#include "seahorn/seahorn.h"

int main() {
	int x = 0;
	int y = 0;
	int z = 0;

	while (unknown()) {
		x++;
		y++;
		z-=2;
	}

	while (x >= 1) {
		z++;
		z++;
		x--;
		y--;
	}

	sassert (x > 0 || z < 1);
	return 0;
}