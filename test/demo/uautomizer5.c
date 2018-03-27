#include "seahorn/seahorn.h"

int main() {
    int copied = 0;
    int x,y, oldx, oldy;
    x = unknown();
    y = unknown();
    oldx = unknown();
    oldy = unknown();
    while (x>0 && y>0) {
        if (copied == 1) {
            sassert ( (x<oldx && 0<=oldx) || (y<oldy && 0<=oldy));
        } else {
            if (nondet()) {
                copied = 1;
                oldx = x;
                oldy = y;
            }
        }
        
        if (nondet()) {
            x = x -1;
        } else {
            x = nondet();
            y = y -1;
        }
    }
}