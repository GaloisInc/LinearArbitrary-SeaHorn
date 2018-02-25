#include "seahorn/seahorn.h"
extern int unknown ();

int main () {
	int cnt = unknown ();
	if (cnt > 0) {
		int f0 = 0;
		int f1 = 1;
		int tmp;
		while (cnt > 0) {
			tmp = f1;
			f1 = f1 + f0;
			f0 = tmp;
			cnt = cnt - 1;	
		}
	
		sassert (!(f0%2 == 0 && f1 % 2 == 0));
	}
}
