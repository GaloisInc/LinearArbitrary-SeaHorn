
#include "seahorn/seahorn.h"

int main ()  {
	int x = 0;
	int c = 1;

	while (1) {
		sassert (x >= 0);
		x = x + c;
		c = 2*c;
		//sassert (x >= 0);
	}
	return 0;
}