# 1 "/tmp/tmp.6E3V4HoYk8.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "/tmp/tmp.6E3V4HoYk8.c"
extern void __VERIFIER_error();
extern void __VERIFIER_assume(int);
void __VERIFIER_assert(int /*@  predicates{cond!=0} predicates_tpl{0==0} @*/ cond){
  if(!(cond)){
    ERROR: __VERIFIER_error();
  }
  return;
}
int __VERIFIER_nondet_int();
int __VERIFIER_nondet_bool();
int main()
{
    int /*@  predicates{(x>(-100)),x<100,x>=100} @*/ x=__VERIFIER_nondet_int();
    int y=__VERIFIER_nondet_int();
    int /*@  predicates{((x<100)&&(z>100)),(z>(-100)),x<=100,z<100,z<=100,z<=x,z>=x} terms_tpl{z+x,z-x} @*/ z=__VERIFIER_nondet_int();
__VERIFIER_assume(x<100);
__VERIFIER_assume(x>-100);
__VERIFIER_assume(z<100);
__VERIFIER_assume(z>-100);
    while(x<100 && 100<z)
    {
        int /*@  predicates{tmp!=0,tmp==0} @*/ tmp=__VERIFIER_nondet_bool();
        if(tmp){
            x++;
        } else {
            x--;
            z--;
        }
    }
    __VERIFIER_assert(x>=100 || z<=100);
    return 0;
}
