#include "seahorn/seahorn.h"

int main () {
	int i = 0;
	while (i < 256) {
		int j = 0;
		while (j < 16) {
			j += 2;
		}
		i += j;
	}
	sassert (i == 256);
}