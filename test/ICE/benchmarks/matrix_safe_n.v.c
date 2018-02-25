extern void __VERIFIER_error() __attribute__ ((__noreturn__));
extern int __VERIFIER_nondet_int();
void __VERIFIER_assert(int cond) {
  if (!(cond)) {
    ERROR: __VERIFIER_error();;
  }
  return;
}

main()
{
  unsigned int N_LIN;
  unsigned int N_COL;
  unsigned int j,k;
  int **matriz;
  int maior,v1,v2,v3;
  N_LIN = __VERIFIER_nondet_int();
  N_COL = __VERIFIER_nondet_int();

  maior = __VERIFIER_nondet_int();
  for(j=0;j<N_COL;j++)
  {
    for(k=0;k<N_LIN;k++)
    {
       __VERIFIER_assert(matriz[0][0]<=maior || (j == 0 && k == 0));
       matriz[j][k] = __VERIFIER_nondet_int();

       if(matriz[j][k]>=maior)
          maior = matriz[j][k];
    }
    v1 = __VERIFIER_nondet_int();
    v2 = __VERIFIER_nondet_int();
    v3 = __VERIFIER_nondet_int();
  }

  if(!(matriz[0][0]<=maior))
  {
    ERROR:
    goto ERROR;;
  }
}
