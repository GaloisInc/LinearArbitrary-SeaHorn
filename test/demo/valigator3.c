#include "seahorn/seahorn.h"
extern int unknown ();

int main () {
	int a = unknown();
	int x = a;
	int b = unknown ();
	int y = b;
    	while((x > y) || (y > x)) {
		x = x + 1;
		y = y-1; 
	};
    	sassert(x == y && 2*x == a + b);
}
