#include "seahorn/seahorn.h"

int main () {
	int i = unknown();
	int LRG = unknown();
	__VERIFIER_assume (i == 0 && LRG == 100);

	while (i < LRG) {
		i += 2;
	}

	LRG *= 2;

	while (i < LRG) {
		i += 2;
	}

	sassert (i == LRG);
}