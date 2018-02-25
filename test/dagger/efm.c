#include "seahorn/seahorn.h"

//This example is adapted from StInG 
void main()
{
	int X1 = unknown();
	int X2 = unknown();
	int X3 = unknown();
	int X4 = unknown();
	int X5 = unknown();
	int X6 = unknown();

	if (! (X1>=1)) return;
	if (! (X2==0)) return;
	if (! (X3==0)) return;
	if (! (X4==1)) return;
	if (! (X5==0)) return;
	if (! (X6==0)) return;

	while(unknown())
	{
		if (unknown())
		{
			if (! (X1 >= 1)) return;
			if (! (X4 >= 1)) return;
			X1=X1-1;
			X4=X4-1;
			X2=X2+1;
			X5=X5+1;
		}
		else
		{
			if (unknown())
			{
				if (! (X2 >= 1)) return;
				if (! (X6 >= 1)) return;
				X2=X2-1;
				X3=X3+1;
			}
			else
			{
				if (unknown())
				{
					if (! (X4 >= 1)) return;
					if (! (X3 >= 1)) return;
					X3=X3-1;
					X2=X2+1;
				}
				else
				{
					if (unknown())
					{
						if (! (X3>=1)) return;
						X3=X3-1;
						X1=X1+1;
						X6=X6+X5;
						X5=0;
					}
					else
					{
						if (! (X2 >= 1)) return;
						X2 = X2 - 1;
						X1 = X1 + 1;
						X4 = X4 + X6;
						X6 = 0;
					}
				}
			}
		}
	}

	sassert (X4 + X5 + X6 -1 >= 0); 
	sassert (X4 + X5 + X6 -1 <= 0); 
	sassert (X4 + X5 <= 1);
	sassert (X5  >= 0);
	sassert (X4  >= 0);
	sassert (X3  >= 0);
	sassert (X2  >= 0);
	sassert (X1 + X5 >= 1);
	sassert (X1 + X2 >= X4 + X5);
	sassert (X1 + X2 + X3 >= 1);
}
