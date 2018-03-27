#include "seahorn/seahorn.h"

int main () {
	int x = 0;
	int y = 0;
	int z = unknown();
	while (unknown()) {
		x+=10;
		y++;
	}
	sassert (x > z || y < z+1);
	return 0;
}