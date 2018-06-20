void main()
{
 int n = unknown1();
 int k=1;
 int i=1;
 int j=0;
 while(i<=n-1) {
  j=0;
  while(j<=i-1) {
      k += (i-j);
      j++;
  }
  if(j<i)
    return;
  i++;
 }
 if(i < n)
   return;
 if(k <= n-1){
   goto ERROR; ERROR: __VERIFIER_error();
 }
 
}