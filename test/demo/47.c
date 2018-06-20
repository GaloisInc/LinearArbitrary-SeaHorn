#include "seahorn/seahorn.h"

int main () {
	int i = 1;
	int j = 1;
	int k = unknown();

	__VERIFIER_assume (k >= 0 && k <= 1);

	int LRG1 = unknown();
	int LRG2 = unknown();

	__VERIFIER_assume (LRG1 < LRG2);

	while (i < LRG2) {
		i ++;
		j += k;
		k -= 1;

		sassert (i + k >= 1 && i + k <= 2);
	}
}