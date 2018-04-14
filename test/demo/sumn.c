#include "seahorn/seahorn.h"
int main () {
	int i = 0;
	int s = 0;
	int n = unknown();
	while (i < n) {
		s = s + i;
		i++;
	}
	sassert (n < 10 || s >= 45);
}