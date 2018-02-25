#include "seahorn/seahorn.h"

extern int unknown();

int main () {
	int cnt = unknown();
	int ocnt = cnt;
	int f0 = 0;
	int f1 = 1;
	int tmp;
	while (cnt > 0) {
		tmp = f1;
		f1 = f1 + f0;
		f0 = tmp;
		cnt = cnt - 1;	
	}
	sassert (ocnt != 7 || f1 == 21);
	return 0;
}
