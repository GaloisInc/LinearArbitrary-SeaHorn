#include "seahorn/seahorn.h"
int main () {
	int x1 = unknown();
	int x2 = unknown();
	int x3 = unknown();
	int c1 = unknown();
	int c2 = unknown();
	__VERIFIER_assume (x1 > 0 && x2 >0 && x3 > 0);
	while ( x1 > 0 && x2 > 0 && x3 > 0 ) {
		if ( c1 ) x1=x1-1;
		else if ( c2 ) x2=x2-1;
		else x3=x3-1;
		c1=nondet (); c2=nondet ();
	}
	sassert (x1 == 0 || x2 == 0 || x3 == 0);
}