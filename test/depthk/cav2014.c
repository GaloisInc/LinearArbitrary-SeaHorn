#include "seahorn/seahorn.h"

int main () { 
	int p = unknown();
	int i = 0;
	int j = 0;
	__VERIFIER_assume(p == 50);

	while (i < 100) {
		if (i == p) {
			i++;
			j=1;
		} else {
			i++;
		}
	}

	sassert (j == 1);
}