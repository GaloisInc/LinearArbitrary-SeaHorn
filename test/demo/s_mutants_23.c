#include "seahorn/seahorn.h"

int main () {
	int x1 = 0;
	int x3 = 0;
	int x5 = 0;

	while (unknown()) {
		if (unknown()) {
			x1 ++;
			x3 --;
		} else {
			x1 --;
			x3 ++;
		}
		x5 += x1 + x3;
	}
	sassert (x5 >= 0);
}