void main()
{
  int x; int y; int k; int j;int i; int n;
    x = unknown1();
    y = unknown1();
    k = unknown1();
    i = unknown1();
    j = unknown1();
    n = unknown1();
    int m = 0;
    if((x+y) != k)
      return;
    j = 0;
    while(j<=n-1) {
      if(j==i)
      {
         x++;
         y--;
      }else
      {
         y++;
         x--;
      }
	if(unknown1())
  		m = j;
      j++;
    }
    if(j < n)
      return;
    if(x + y <= k - 1 || x + y >= k + 1 || (n >= 1 && ((m <= -1) || (m >= n))))
    {goto ERROR; ERROR: __VERIFIER_error();}
}