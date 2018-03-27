#include "seahorn/seahorn.h"

int main () {
	int x = 0;
	int y = 0;

	do {
		if (x <= 50)
			y++;
		else 
			y--;
		x++;
	} while (!(y == 0));

	sassert (x == 102);
	return 0;
}