#include "seahorn/seahorn.h"
#define a 2

int main () {
	unsigned int i ,j, n, sn = 0;
	j = i = unknown();
	n = unknown();
	while (i < n) {
		sn = sn + a ;
		i++;
	}
	sassert ((sn == (n-j) * a) || sn == 0);
	return 0;
}