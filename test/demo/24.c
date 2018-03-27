void main(int argc, char* argv[]) {

  int n;
  int i, k, j;

  k = unknown1();
  n = unknown1();
  if(n < 1)
    return;
  if(k < n)
    return;
  j = 0;
  while( j <= n-1 ) {
    j++;
    k--;
  } 
  if(j < n)
    return;
  if(k <= -1)
  {goto ERROR;ERROR: __VERIFIER_error();}
}