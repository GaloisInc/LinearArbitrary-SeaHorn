int main(){

  int x = 10;
  int y = 10;
  int z = 10;
  int k = 0;
  while (nondet()){
    x++;
    y++;
    while (nondet()){
      x += 2;
      y += 2;
      while (x > 0){
        x--;
        y--;
        while (z > 10){
          z--;
          y++;
          x++;
          while (nondet2()){
            x++;
            y++;
            while (nondet()){
              k--;
              z--;
            }
          }
        }
      }
    }
  }

  if (x != y){
    goto ERROR;
  }
  
  if (k > 0)
    goto ERROR;

  if (z == 11)
    goto ERROR;

  return 1;

ERROR:__VERIFIER_error();
}