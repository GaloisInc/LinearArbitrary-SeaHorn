#include "seahorn/seahorn.h"

int main () {
	int x = 0;
	int y = 0;
	while (unknown()) {
		x+=4;
		y++;
	}

	while (x > 0) {
		x-=4;
		y--;
	}

	sassert (y > -1);
	return 0;
}