#include "seahorn/seahorn.h"

//This example is adapted from StIng 
void main()
{

	int invalid = unknown();
	int unowned = unknown();
	int nonexclusive = unknown();
	int exclusive = unknown();

	if (! (exclusive==0)) return;
	if (! (nonexclusive==0)) return;
	if (! (unowned==0)) return;
	if (! (invalid>= 1)) return;

	while (unknown())
	{
		if (unknown())
		{
			if (! (invalid >= 1)) return;
			nonexclusive=nonexclusive+exclusive;
			exclusive=0;
			invalid=invalid-1;
			unowned=unowned+1;
		}
		else
		{
			if (unknown())
			{
				if (! (nonexclusive + unowned >=1)) return;
				invalid=invalid + unowned + nonexclusive-1;
				exclusive=exclusive+1;
				unowned=0;
				nonexclusive=0;
			}
			else
			{
				if (! (invalid >= 1)) return;
				unowned=0;
				nonexclusive=0;
				exclusive=1;
				invalid=invalid+unowned+exclusive+nonexclusive-1;
			}
		}
	}

	sassert (exclusive >= 0);
	sassert (unowned >= 0);
	sassert (invalid + unowned + exclusive + nonexclusive >= 1);
}