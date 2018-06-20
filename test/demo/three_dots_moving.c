#include "seahorn/seahorn.h"

int main () {
	int x = unknown();
	int y = unknown();
	int K = unknown();
	int g = unknown();

	__VERIFIER_assume (K >= 0 && x < y && g >= K - x + y - x && g >= y - x && g >= y - K);

	while (y != K) {
		sassert (g > 0);
		g --;
		if (x == y) {
			if (x > K)
				x --;
			else
				x ++;
			y = x;
		} else {
			y--;
		}
	}
}