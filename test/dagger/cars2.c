#include "seahorn/seahorn.h"

//This example is adapted from StInG 
void main()
{
	int x1 = unknown();
	int v1 = unknown();
	int x2 = unknown();
	int v2 = unknown();
	int x3 = unknown();
	int v3 = unknown();
	int t = unknown();

	x1=100;
	x2=75;
	x3=-50;
	if (! (v3 >= 0)) return;
	if (! (v1 <= 5)) return;
	if (! (v1 -v3 >= 0)) return;
	if (! (2* v2 - v1 - v3 == 0)) return;
	t=0;

	if (! (v2 +5 >=0)) return;
	if (! (v2 <= 5)) return;
	while (unknown())
	{
		if (! (v2 +5 >=0)) return;
		if (! (v2 <= 5)) return;
		if (unknown())
		{
			//if (! (2* x2 - x1 - x3>=0)) return;
			x1 = x1+v1;
			x3 = x3+v3;
			x2 = x2+v2;
			v2 = v2-1;
			t = t+1;
		}
		else
		{
			//if (! (2*x2 -x1-x3 <=0)) return;
			x1 = x1+v1;
			x3 = x3+v3;
			x2 = x2+v2;
			v2 = v2+1;
			t = t+1;
		}
	}
	sassert (5*t  + 75 >= x2);
}
