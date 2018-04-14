int main(){
  int x = 10;
  int y = 10;
  
  while (nondet()){
    x++;
    y++;
  }

  while (x  > 0){
    x--;
    y--;
  }
  

  if (y != 0)
    goto ERROR;

      return 1;

ERROR:__VERIFIER_error();
}