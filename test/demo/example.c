#include "seahorn/seahorn.h"

int main () {
	int p, c, s = unknown();
	int x = 0, y = 0;
	p = s ? 1 : 2;
	while (unknown()) {
		x++;
		c = 100;
		while (unknown()) {
			if (p != 1 && p != 2) {
				c = 0;
			}
			y++;
		}
		sassert (c == 100);
	}
	sassert ((s && p == 1) || (!s && p == 2));
	return 0;
}