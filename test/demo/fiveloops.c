int main(){

  int x = 10;
  int y = 10;
  int z = 10;
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
          while (y > 0){
            x++;
            y++;
          }
        }
      }
    }
  }

  if (x != y){
    goto ERROR;
  }

  return 1;

ERROR: __VERIFIER_error();
}