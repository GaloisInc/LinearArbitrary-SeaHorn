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
  int SIZE;
  unsigned int j,k;
  int *array;
  int menor, v1, v2, v3;
  
  SIZE = __VERIFIER_nondet_int();//=5;
  menor = __VERIFIER_nondet_int();

  for(j=0;j<SIZE;j++) {
       // assert {1;1} b0(menor, j, array[0]);
       array[j] = __VERIFIER_nondet_int();
       
       if(array[j]<=menor)
          menor = array[j];                          
       v1 = __VERIFIER_nondet_int();
       v2 = __VERIFIER_nondet_int();
       v3 = __VERIFIER_nondet_int();
    }                       
    
    __VERIFIER_assert(SIZE <= 0 || array[0]>=menor);    
}

