int nondet();
void assume (_Bool v) { while (!v); }
void assert (_Bool v) { if (!v) ERROR: __VERIFIER_error(); }


int main (void)
{
  int x,y;
  int u, v;
  
  x = 0;
  y = 0;
  
  u = nondet ();
  v = nondet ();
  assume (u > 0);
  assume (v > 0);
  assume (u < v);
  
  while (x < v + 1)
    {
      if (x < v ) y++;
      x++;
    }
  
  assert (x != y);
  return 1;
}