#include "seahorn/seahorn.h"

void main () {

int x,y;

x=0;
y=0;

while (unknown()) {
x++;
y++;
}

while (x > 0 || x < 0) {
x--;
y--;
}

sassert (y >= 0 && y <= 0);

} 