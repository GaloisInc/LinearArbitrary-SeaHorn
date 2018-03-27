void main()
{
	int flag = unknown1();
	int x = 0;
	int y = 0;

	int j = 0;
	int i = 0;


	while(unknown1())
	{
	  x++;
	  y++;
	  i+=x;
	  j+=y;
	  if(flag)  j+=1;
          j = j;
	} 
	if(j <= i - 1)
	{
	  goto ERROR;
          ERROR: __VERIFIER_error();
	}
	
}