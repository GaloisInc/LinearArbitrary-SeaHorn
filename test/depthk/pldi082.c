#include "seahorn/seahorn.h"

int main () {
	int x = 0;
	int y = 0;
	int n = unknown();

	__VERIFIER_assume (n >= 0);
 
	do {
		if (x <= n)
			y++;
		else 
			y--;

		if (y < 0) {
			if (n >= 0) {
				sassert (y != -1 || x < 2*n + 3);
			}
			break;
		}
		x++;
	} while (1);
	return 0;
}