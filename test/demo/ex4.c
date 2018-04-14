#include "seahorn/seahorn.h"
int main () {
	int b = unknown();
	int x = 0;
	int y = 0;

	while (unknown()) {
		if (b >= 0) {
			x++;
			y++;
		} else {
			x++;
			y--;
		}
	}
	sassert ((y >= 0 && b >= 0) || (b <= (-1) && y <= 0));

}