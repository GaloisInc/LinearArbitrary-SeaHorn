#include "seahorn/seahorn.h"

int main () { 
	int p = unknown();
	int i = 0;
	int j = 0;
	int lb, ub, limit;
	limit = unknown();
	lb = unknown();
	ub = unknown();
	__VERIFIER_assume(0 <= lb && lb < ub && ub < limit);
	__VERIFIER_assume(lb <= p && p <= ub);

	while (i < limit) {
		if (i == p) {
			i++;
			j=1;
		} else {
			i++;
		}
	}

	sassert (j == 1);
}