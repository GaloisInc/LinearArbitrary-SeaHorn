#include "seahorn/seahorn.h"

void main () {
int i, j;
int from;
int to;
int k;
int Bound;

i = unknown();
j = unknown();
from = unknown();
to = unknown();
k = unknown();
Bound = unknown();

if (!(k >= 0)) return;
if (!(k <= Bound)) return;

if (!(from >= 0)) return;
if (!(from <= k)) return;

i = from;
j = 0;

while (i < k) {
i++;
j++;
}

sassert (j <= Bound);

}
