#include "seahorn/seahorn.h"

int main()
{
	int i;
	for(i=0;i<1000;i++);
	sassert(i<=10000);
	return 0;
}
