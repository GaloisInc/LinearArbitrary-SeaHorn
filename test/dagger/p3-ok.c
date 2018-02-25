#include "seahorn/seahorn.h"

void main()
{ 
  register int c;
  int delim = unknown ();
  int pvpbsize = unknown ();
  unsigned int bslashmode = unknown ();
  unsigned int route_syntax = unknown ();
  int cmntcnt = unknown ();
  int anglecnt = unknown ();
  int state = unknown ();
  int errno = unknown ();
  unsigned int tmp = unknown ();
  int id = 0;
  int unknownvar = unknown ();

  {
  errno = 0;
  bslashmode = 0;
  route_syntax = 0;
  cmntcnt = 0;
  anglecnt = 0;
  state = 1;
  c = -1;
  while (1) {
    while (1) {
      if (c != -1) {
        if (! bslashmode) {
          if (id >= pvpbsize - 5) {
            tmp = unknown();
            if (tmp > 40) {
            }
            returnnull:
            return ;
          }
          sassert (id >= 0);
	  sassert (id < pvpbsize);
          id++;
        }
      }
      c = unknown();
      if (c == 0) {
        if (state == 2) {
          c = '\"';
        } else {
          if (cmntcnt > 0) {
            c = ')';
          } else {
            if (anglecnt > 0) {
              c = '>';
            } else {
              break;
            }
          }
        }
      } else {
        if (c == delim) {
          if (cmntcnt <= 0) {
            if (state != 2) {
              if (anglecnt <= 0) {
                break;
              }
              if (delim == 44) {
                if (! route_syntax) {
                  c = '>';
                }
              }
            }
          }
        }
      }
      if (id >= pvpbsize - 5) {
        return ;
      } else {
        sassert (id >= 0);
	sassert (id < pvpbsize);
      }
      if (bslashmode) {
        bslashmode = 0;
        if (cmntcnt > 0) {
          c = -1;
          goto M__Cont;
        } else {
          if (c != 33) {
            goto M_L;
          } else {
            if (state == 2) {
              M_L:
              if (id >= pvpbsize - 5) {
                return ;
              } else {
                sassert (id >= 0); sassert (id < pvpbsize);
                id++;
              }
              goto M__Cont;
            }
          }
        }
      }
      if (c == 92) {
        bslashmode = 1;
      } else {
        if (! (state == 2)) {
          if (c == 40) {
	    unknownvar = unknown();
            if (unknownvar) {
              cmntcnt ++;
              c = -1;
            } else {
              goto M_L___1;
            }
          } else {
            M_L___1:
            if (c == 41) {
	    unknownvar = unknown();
              if (unknownvar) {
                if (cmntcnt <= 0) {
                  c = -1;
                } else {
                  cmntcnt --;
                }
              } else {
                goto M_L___0;
              }
            } else {
              M_L___0:
              if (cmntcnt > 0) {
                c = -1;
              } else {
                if (c == 60) {
                  anglecnt ++;
                  while (1) {
		    unknownvar = unknown();
                    if (unknownvar) {
		    unknownvar = unknown();
                      if (unknownvar) {
                        break;
                      }
                    } else {
                      break;
                    }
                  }
		    unknownvar = unknown();
                  if (unknownvar) {
                    route_syntax = 1;
                  }
                } else {
                  if (c == 62) {
                    if (anglecnt <= 0) {
                      c = -1;
                    } else {
                      anglecnt --;
                    }
                    route_syntax = 0;
                  } else {
                    if (delim == 32) {
			    unknownvar = unknown();
                      if (unknownvar) {
			    unknownvar = unknown();
                        if (unknownvar) {
                          c = ' ';
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
      if (c == -1) {
        goto M__Cont;
      }
      if (c == delim) {
        if (anglecnt <= 0) {
          if (state != 2) {
            break;
          }
        }
      }
      state = unknown();
      if (state == 5) {
	    unknownvar = unknown();
        if (unknownvar) {
          if (unknownvar) {
          } else {
          }
        } else {
        }
      }
	    unknownvar = unknown();
      if (unknownvar) {
        c = -1;
      }
	    unknownvar = unknown();
      if (unknownvar) {
        break;
      }
      M__Cont: ;
    }
	    unknownvar = unknown();
    if (unknownvar) {
      if (id >= pvpbsize - 5) {
        return ;
      } else {
        sassert (id >= 0);
	sassert (id < pvpbsize);
        id++;
      }
	    unknownvar = unknown();
      if (unknownvar) {
        goto returnnull;
      }
    }
    if (c != 0) {
      if (! (c != delim)) {
        if (! (anglecnt > 0)) {
          break;
        }
      }
    } else {
      break;
    }
  }
  return ;
}
}

