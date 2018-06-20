#include "seahorn/seahorn.h"

int main () {
	int x1 = -100;
	int x3 = 2*x1;

	while (unknown()) {
		x1 ++;
		x3 += 2;
	}

	sassert (x1 <= 0 || x3 > 0);
}