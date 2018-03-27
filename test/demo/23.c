void main()
{
  int n = unknown1();
  int m = unknown2();
  int x=0; 
  int y;
  y = m;
  if(n < 0)
    return;
  if(m < 0)
    return;
  if(m > n-1)
    return;
  while(x<=n-1) {
    x++;
    if(x>=m+1) y++;
    else if(x > m) return;
    x = x;
  }
  if(x < n)
    return;
  if(y >= n+1)
  {goto ERROR; ERROR: __VERIFIER_error();}
}