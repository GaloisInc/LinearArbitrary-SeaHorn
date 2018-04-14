int main(){
  int x = nondet() , y = nondet() ;
  while (x > 0) {
    while(y < 0){
    y++;
    }
    x++;
  }
  
  while ( y > 0){
    y--;
  }
  if (y > 0) goto ERROR;
  if (x > 0) goto ERROR;

  return 0 ;

ERROR:__VERIFIER_error();
}