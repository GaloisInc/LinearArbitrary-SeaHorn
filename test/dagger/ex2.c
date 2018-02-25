#include "seahorn/seahorn.h"
int main () {
int x=0;

if (unknown()) x = x+1;
else x = x+22; 

if (unknown()) x = x+1;
else x = x+20; 

if (unknown()) x = x+1;
else x = x+18; 

if (unknown()) x = x+1;
else x = x+16; 

if (unknown()) x = x+1;
else x = x+14; 

if (unknown()) x = x+1;
else x = x+12; 

if (unknown()) x = x+1;
else x = x+10; 

if (unknown()) x = x+1;
else x = x+8; 

if (unknown()) x = x+1;
else x = x+6; 

if (unknown()) x = x+1;
else x = x+4; 

if (unknown()) x = x+1;
else x = x+2; 

sassert (x <= 132);

return 0;
}