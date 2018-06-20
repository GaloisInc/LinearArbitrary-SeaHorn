#include "seahorn/seahorn.h"

int main () {
	int i = 1;
	int j = 6;
	int k = unknown();

	while (j >= i) {
		i += 2;
		j -= 1;
	}

	sassert (j == 4);

	k = j;
	i = k;
	j = 1;

	while (unknown()) {
		i+= k;
		j += 1;
	}

	sassert (i == k*j);
}