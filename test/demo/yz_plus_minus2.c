#include "seahorn/seahorn.h"

int main () {
	int x = 0;
	int y = 0;
	int z = 0;
	int w = 0;

	while (x < 10000) {
		x = x + w;
		y = z + 1;
		z = w - 1;
		w = y + 1;
		sassert (x >= 0);
	}
}
