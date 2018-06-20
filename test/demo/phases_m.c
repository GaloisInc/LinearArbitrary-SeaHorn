#include "seahorn/seahorn.h"

int main () {
	int x = 0;
	int LRG1 = unknown();
	int LRG2 = unknown();
	int tmp1 = unknown();
	int tmp2 = unknown();

	__VERIFIER_assume (LRG1 > 0 && LRG1 < LRG2 && LRG1 == 2*tmp1 && LRG2 == 2*tmp2 + 1);

	while (x < LRG1) {
		if (x < LRG1)
			x++;
		else
			x+=2;
	}

	while (x < LRG2) {
		if (x < LRG1)
			x++;
		else
			x+=2;
	}

	sassert (x % 2 == 0);
}