#include "seahorn/seahorn.h"

void mime7to8()
{ 
  int c1 ;
  int c2 ;
  int c3 ;
  int c4 ;
  int tmp___6 = unknown();
  int id = unknown();

  {
  if (tmp___6 == 0) {

    id = 0;

    while (1) {
      c1 = unknown();
      if (! (c1 != -1)) {
        break;
      }
      if (unknown()) {
        if (unknown()) {
          continue;
        }
      }
      c2 = unknown();
      while (1) {
        c2 = unknown();
        if (unknown()) {
          if (unknown()) {
            break;
          }
        } else {
          break;
        }
      }
      if (c2 == -1) {
        break;
      }
      c3 = unknown();
      while (1) {
        c3 = unknown();
        if (unknown()) {
          if (unknown()) {
            break;
          }
        } else {
          break;
        }
      }
      if (c3 == -1) {
        break;
      }
      c4 = unknown();
      while (1) {
        c4 = unknown();
        if (unknown()) {
          if (unknown()) {
            break;
          }
        } else {
          break;
        }
      }
      if (c4 >= -1) {
	if (c4 <= -1) {
        break;
	}
      }
      if (c1 >= 61) {
	if (c1 <= 61) 
        continue;
      } else {
        if (c2 >= 61) {
	if (c2 <= 61)
          continue;
        }
      }
      if (c1 < 0) {
        c1 = -1;
      } else {
        if (c1 > 127) {
          c1 = -1;
        } else {
          c1 = unknown();
        }
      }
      if (c2 < 0) {
        c2 = -1;
      } else {
        if (c2 > 127) {
          c2 = -1;
        } else {
          c2 = unknown();
        }
      }
      if (id >= 100) {
	sassert (id >= 0); sassert (id <= 100);
        id = 0;
      } else {
	sassert (id >= 0); sassert (id <= 100);
      }
      id++;
      if (unknown()) {
        goto MY_L;
      } else {
        if (id >= 100) { 
          MY_L:
          if (id > 1 || id < 1) {
            id--;
		sassert (id >= 0); sassert (id <= 100);
            if (unknown() ) {
              id++;
            } else {
              id--;
		sassert (id >= 0); sassert (id <= 100);
              if (unknown()) {
                id++;
              }
            }
          }
          //printf("resetting fbufp\n");
          id = 0;
        }
      }
      if (c3 >= 61) {
	if (c3 <= 61)
        continue;
      }

      if (c3 < 0) {
        c3 = -1;
      } else {
        if (c3 > 127) {
          c3 = -1;
        } else {
          c3 = unknown();
        }
      }
      if (id >= 100) { 
		sassert (id >= 0); sassert (id <= 100);
        id = 0;
      } else {
		sassert (id >= 0); sassert (id <= 100);
      }
      id++;
      if (unknown()) {
        goto MY_L___0;
      } else {
        if (id >= 100) { 
          MY_L___0:
          if (id > 1 || id < 1) {
            id--;
		sassert (id >= 0); sassert (id <= 100);
            if (unknown()) {
              id++;
            } else {
              id--;
		sassert (id >= 0); sassert (id <= 100);
              if (unknown()) {
                id++;
              }
            }
          }
		sassert (id >= 0); sassert (id <= 100);
          //printf("resetting fbufp\n");
          id = 0;
        }
      }
      if (c4 >= 61) {
	if (c4 <= 61)
        continue;
      }

      if (c4 < 0) {
        c4 = -1;
      } else {
        if (c4 > 127) {
          c4 = -1;
        } else {
          c4 = unknown();
        }
      }
      if (id >= 100) {
		sassert (id >= 0); sassert (id <= 100);
        id = 0;
      } else {
		sassert (id >= 0); sassert (id <= 100);
      }
      id++;
      if (unknown()) {
        goto MY_L___1;
      } else {
        if (id >= 100) {
          MY_L___1:
          if (id > 1 || id < 1) {
            id--;
		sassert (id >= 0); sassert (id <= 100);
            if (unknown()) {
              id++;
            } else {
              id--;
		sassert (id >= 0); sassert (id <= 100);
              if (unknown() ) {
                id++;
              }
            }
          }
          //printf("resetting fbufp\n");
          id = 0;
        }
      }
    }
    if (id >= 0) {
      sassert (id >= 0); sassert (id <= 100);
    }
  } else {
    //printf("We only support base64 encoding...\n");
  }
  return;
}
}

int main() {
mime7to8();
return 0;
}