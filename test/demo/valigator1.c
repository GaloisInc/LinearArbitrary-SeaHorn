#include "seahorn/seahorn.h"

int main () {
	int x = unknown ();
	__VERIFIER_assume(x < 10);
	while(x < 10) {
		x = x + 1;
	};
	x = x + 1;
	sassert(x == 11);
	return 0;
}