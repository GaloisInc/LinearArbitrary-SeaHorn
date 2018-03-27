#include "seahorn/seahorn.h"

int main () {
	int k = unknown();
	int from = unknown();

	int i, j;

	__VERIFIER_assume (k >= 0 && k <= 100 && from >= 0 && from <= k);

	i = from;
	j = 0;

	while (i < k) {
		i++;
		j++;
	}

	sassert (j < 101);
	return 0;
}