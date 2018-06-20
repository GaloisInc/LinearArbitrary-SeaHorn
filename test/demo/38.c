#include "seahorn/seahorn.h"

int main () {
	int i = 0;
	int j = 0;
	int z = unknown();

	while (unknown()) {
		if (j < 0) {
			j -= z;
			i += z;
		} else {
			j += z;
			i -= z;
		}
	}

	if (i < j)
		z = -z;

	while (i != j) {
		if (j < 0) {
			j = i + z;
			i = j - z;
		} else {
			j = i - z;
			i = j + z;
		}
	}

	sassert (i == 0);
}