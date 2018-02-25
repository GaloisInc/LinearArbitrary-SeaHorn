#include "seahorn/seahorn.h"
extern int unknown ();

int main ()  {
	int x = unknown();
	int ox = x;
	int a = 0;

	while (x < 5) {
		a = a + x;
		x = x + 1;
	}

	if (ox == 0)
		sassert (a == 10);
}
