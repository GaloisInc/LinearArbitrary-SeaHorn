#include "seahorn/seahorn.h"

int main () {
	int m = 0;
	int i = 1;

	while (unknown()) {
		i = -i;
		m ++;
	}

	if (m == 100)
		sassert (i > 0);
}