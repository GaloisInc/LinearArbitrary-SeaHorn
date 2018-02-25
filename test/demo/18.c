#include "seahorn/seahorn.h"

extern int unknown();

void loop () { 
  loop ();
}

int init = 0;
int opened = 1;
int closed = 2;
int ignore = 3;

int readit (int st) {
  if (st == opened)
	return opened;
  else if (st == ignore)
	return st;
  else sassert (0==1);
}

int read (int x, int st) {
  if (x)
    return readit (st); 
  else 
    return st;
}

int closeit (int st) {
  if (st == opened)
    return closed; 
  else if (st == ignore)
    return st; 
  else {
    loop (); 
    return 0;
  }
}

int close (int x, int st) {
  if (x) 
    closeit (st); 
  else 
    return st;
}

void f (int x, int y, int st) {
  close (y, (close (x, st))); 
  f (x, y, (read (y, (read (x, st)))));
}

int next (int st) {
  if (st == init)
    return opened; 
  else 
    return ignore;
}

void g (int b3, int x, int st) { 
  if (b3 > 0) 
    f (x, 1, (next (st)));
  else 
    f (x, 0, st);
}

void main () {
  int b2 = unknown ();
  int b3 = unknown ();
  if (b2 > 0)
    g (b3, 1, opened);
  else 
    g (b3, 0, init);
}
