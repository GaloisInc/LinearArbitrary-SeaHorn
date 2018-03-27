#include "seahorn/seahorn.h"

int main () {
	int x = 0;
	int y = 0;
	while (unknown()) {
		x++;
		y++;
	}

	while (x > 0) {
		x--;
		y--;
	}

	sassert (y > -1);
	return 0;
}