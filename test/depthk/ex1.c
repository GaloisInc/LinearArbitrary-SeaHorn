#include "seahorn/seahorn.h"

int main() {
	int x = 0;
	int y = 0;
	while (1) {
		if (x <= 50)  {
			y++;
		} else y--;
		if (y < 0) break;
		x++;
	}
	sassert(x+y<=101);
	sassert(x <= 102);
}