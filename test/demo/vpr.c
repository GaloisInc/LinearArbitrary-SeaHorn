#include "seahorn/seahorn.h"

void main ()
{
  int prev_success, prev2_success, current, final, success;
  final = unknown ();
  success = unknown ();

  prev_success = 1; /* Actually final - 1 failed, but this makes router */
                      /* try final-2 and final-3 even if both fail: safer */
  prev2_success = 1;
  current = final - 2;

  while ( (prev2_success  || prev_success ) && current >= 1) {
  
     success = unknown ();
  
     if (success)  {
        final = current;
     } 
 
     prev2_success = prev_success;
     prev_success = success;
     current--;
  }

  sassert( !(!prev2_success  && !prev_success) ||  current + 2 <= final );

}