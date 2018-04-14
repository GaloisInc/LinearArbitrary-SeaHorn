#include "seahorn/seahorn.h"

int main() {
	int w = unknown();
	int b = unknown();
	__VERIFIER_assume (w >= 0); 
	__VERIFIER_assume (b >= 0); 
	__VERIFIER_assume (w + b >= 2); 
	int z = w;
	while (w + b >= 2) 
	{
		if (unknown() && w >= 2) {
			// Move 1
			w -= 2;
			b += 1; 
		}
		else if (b >= 1) {
			// Move 2 or 3
			b -= 1;
		}
	}
	sassert (w == 0 || b == 0);
	sassert (w == 1 || b == 1);
	return 0;
}
