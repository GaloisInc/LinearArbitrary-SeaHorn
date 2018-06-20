#include "seahorn/seahorn.h"

int main() {
	int x = 0;
	int y = 0;
	int i = 0;
	int j = 0;

	while (unknown()) {
		i++;
		x += 1;
		y -= 1;
	}

	while (unknown()) {
		j++;
		x -= 1;
		y += 1;
	}

	if (i > j)
		sassert (x > y);
}