#include "seahorn/seahorn.h"

extern int unkown ();

int main () {
	int a = unknown ();
	int b = unknown ();
	int result = a;
	int counter = b;
	while (counter > 0) {
		counter --;
		result ++;
	}
	sassert (b < 0 || result == a + b);
}
