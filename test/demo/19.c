void main() {
  int x= 0;
  int m=0;
  int n = unknown1();
  while(x<=n-1) {
     if(unknown1()) {
	m = x;
     }
     x= x+1;
  }
  if(x < n)
    return;
  if(n>=1 && (m <= -1 || m >= n))
  {goto ERROR; ERROR:__VERIFIER_error();}

}