int main()
{
  int k=1;
  int w = 1;
  while(unknown1()) {
    int z = unknown2();
    if(z>5) w++;
    k+=w;
  }
  if(k <= 0)
    {
      goto ERROR;
       ERROR: __VERIFIER_error();
    }
}