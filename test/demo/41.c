#include "seahorn/seahorn.h"

int main() {
	int i = 0;
	int m = unknown();
	//int j = 1000;
	int j = 5*m;
	int k = 0;

	while (unknown()) {
		i ++;
		j -= 3;
		k += 2;
	}

	if (i + k == 3*m)//600)
		sassert (j == 2*m);//400);
}