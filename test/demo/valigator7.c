#include "seahorn/seahorn.h"

extern int unknown ();

int main ()  {
	int a = unknown ();
	if (a >= 0) {
		int x = a;
		int cnt = 1;
		while (cnt > 0) {
			cnt = cnt - 1;
			x = x + 1;
		}
		sassert(x == a + 1);
	}
	return 0;
}
