#include "seahorn/seahorn.h"

void main() {
	int i = unknown();
	int j = unknown();
	int x = i;
	int y = j;

 	while (x != 0) {
		x--;
		y--;
	}

	if (i == j)
		sassert (y == 0);
}
