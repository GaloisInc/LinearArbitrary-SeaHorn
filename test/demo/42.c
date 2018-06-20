#include "seahorn/seahorn.h"

int main () {
	int x1 = 0;
	int x3 = 100;

	while (unknown()) {
		x1 ++;
		if (x3 > 10)
			x3 = 0;
		else 
			x3 ++;
	}

	if (x1 > 10)
		sassert (x3 < x1);


	x1 = 0;
	x3 = 100;

	int c1 = unknown();
	int c2 = unknown();

	while (unknown()) {
		x1++;
		if (x1 > 0)
			c1 = 1;
		else 
			c1 = 0;

		if (x3 > 10)
			c2 = -1;
		else 
			c2 = x3;

		x3 = c1 + c2;
	}

	if (x1 > 10)
		sassert (x3 < x1);

	x1 = 0;
	x3 = 100;

	c1 = unknown();
	c2 = unknown();

	int x4 = unknown();

	while (c1 == c2) {
		x1 ++;

		if (x1 > 0)
			c1 = x4;
		else 
			c1 = x4 + 1;

		if (x3 > 10)
			c2 = 0;
		else
			c2 = x3 + 1;
	}

	if (x1 > 10)
		sassert (x3 < x1);
}