#include "seahorn/seahorn.h"

extern int unknown ();

int make_array (int n, int i) { 
	return n - i;
}

int array_max (int n, int i, int m) {
	if (i >= n)
		return m;
  	else {
		int x = make_array (n, i);
		int z = 0;
		if (x > m)
			z = x;
		else
			z = m;
		return array_max (n, i+1, z);
	}
}

int main () {
	int n = unknown ();
	int i = unknown ();  
	if (n>0 && i>=0 && i<=0) {
		int m = array_max (n, i, -1);
    		sassert (m >= n);
	}
	return 0;
}
