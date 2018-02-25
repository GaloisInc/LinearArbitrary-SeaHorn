#include "seahorn/seahorn.h"

void main () {
int phase, success, give_up, cutoff, count, entered_phase, maxcost, linkages, search, words, canonical, valid;
phase = success = give_up = 0;
cutoff = 0;
count = 0;

entered_phase = unknown ();
maxcost = unknown ();
linkages = unknown ();
search = unknown ();
words = unknown ();
canonical = unknown ();
valid = unknown ();

while(!(success || give_up)) {
     entered_phase = 0;
     if (!phase) {
         if (cutoff == 0) cutoff = 1;
         else {
             if (cutoff == 1 && maxcost > 1) cutoff = maxcost;
             else {
                 phase = 1;
                 cutoff = 1000;  entered_phase = 1;
             }
         }
        if (cutoff == maxcost && search <= 0) give_up = 1;
     } else {
         count++;
         if (count > words) give_up = 1;
     }
     if (entered_phase) count = 1;  
     linkages = unknown ();
     if (linkages > 5000) linkages = 5000;
     canonical = valid = 0;
     if (linkages != 0) {
         valid = unknown ();
         if (!(0 <= valid && valid <= linkages)) {
            return;
         } 
         canonical = linkages;
     }
     //sassert(! ((canonical == 0) && (linkages > 0) && (linkages < 5000)));
     if (valid > 0) success = 1;
}

 sassert ( (valid > 0 || count > words || (cutoff == maxcost && search <= 0))
  && valid <= linkages
  && canonical == linkages
  && !(linkages > 5000) );
 return;
}
