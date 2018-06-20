void main(int n)
{
 int k=1;
 int i=1;
 int j=0;
 while(i<=n-1) {
   if(k <= i-1)
   ERROR: __VERIFIER_error();
  j=0;
  while(j<=i-1) {
      k += (i-j);
      j++;
  }
  if(j < i)
    return;
  i++;
 }
 
}