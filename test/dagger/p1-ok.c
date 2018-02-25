#include "seahorn/seahorn.h"

typedef unsigned long size_t;

size_t ColonOkInAddr ;
char *MustQuoteChars ;
static char buf[401] ;
char *crackaddr()
{
  char c ;
  int cmtlev ;
  int realcmtlev ;
  int anglelev ;
  int copylev ;
  int bracklev ;
  int id ;
  int ctr ;
  int bufheadctr ;
  int tmp ;
  int tmp___5 ;
  int tmp___10 ;
  char *tmp___20 ;
  int tmp___22 ;
  int tmp___31 ;
  int tmp___38 ;

  ColonOkInAddr = unknown();

  int LEN = unknown();

  __VERIFIER_assume (LEN >= 0);

  c = unknown();
  while (c != 0) {
    tmp = unknown();
    if (! tmp) {
      break;
    }
  }
  ctr = LEN;
  id = 0;
  bufheadctr = 0;
  realcmtlev = 0;
  cmtlev = realcmtlev;
  anglelev = cmtlev;
  copylev = anglelev;
  bracklev = 0;
  while (1) {
    c = unknown();
    if (! ((int )c != 0)) {
      break;
    }
    if (copylev > 0) {
      if (id < ctr) {
        if (ctr <= LEN) {
          if (!(id >= 0)) goto ERROR;
          id ++;
        } else {
          return ((char *)0);
        }
      } else {
        return ((char *)0);
      }
    }
    if ((int )c == 92) {
      if (cmtlev <= 0) {
        if (! unknown()) {
        }
      }
      c = unknown();
      if ((int )c == 0) {
      }
      if (copylev > 0) {
        if (id < ctr) {
          if (ctr <= LEN) {
            if (!(id >= 0)) goto ERROR;
            id ++;
          } else {
            return ((char *)0);
          }
        } else {
          return ((char *)0);
        }
      }
    }
    if ((int )c == 34) {
      if (cmtlev <= 0) {
        if (copylev > 0) {
          if (id < ctr) {
            if (ctr <= LEN) {
              if (unknown()) {
                ctr --;
              } else {
                ctr ++;
              }
            }
          }
        }
        continue;
      }
    }
    if (unknown()) {
      goto putg;
    }
    if ((int )c == 40) {
      cmtlev ++;
      if (id < ctr) {
        if (ctr <= LEN) {
          ctr --;
          realcmtlev ++;
          tmp___5 = copylev;
          copylev ++;
          if (tmp___5 <= 0) {
            if (id > bufheadctr) {
              goto M_L;
            } else {
              if (id < bufheadctr) {
                M_L:
                if (id < ctr) {
                  if (ctr <= LEN) {
                    if (!(id >= 0)) goto ERROR;
                    id ++;
                  } else {
                    return ((char *)0);
                  }
                } else {
                  return ((char *)0);
                }
              }
            }
            if (id < ctr) {
              if (ctr <= LEN) {
                if (!(id >= 0)) goto ERROR;
                id ++;
              } else {
                return ((char *)0);
              }
            } else {
              return ((char *)0);
            }
          }
        }
      }
    }
    if (cmtlev > 0) {
      if ((int )c == 41) {
        cmtlev --;
        copylev --;
        if (id < ctr) {
          if (ctr <= LEN) {
            realcmtlev --;
            ctr ++;
          }
        }
      }
      continue;
    } else {
      if ((int )c == 41) {
        if (copylev > 0) {
          if (id < ctr) {
            if (ctr <= LEN) {
              id --;
            }
          }
        }
      }
    }
    if ((int )c == 91) {
      bracklev ++;
    } else {
      if ((int )c == 93) {
        bracklev --;
      }
    }
    if ((int )c == 58) {
      if (anglelev <= 0) {
        if (bracklev <= 0) {
          if (! unknown()) {
            if (! ColonOkInAddr) {
              if (unknown()){
                goto M_L___0;
              } else {
                if (unknown()){
                  M_L___0:
                  if (cmtlev <= 0) {
                    if (! unknown()) {
                    }
                  }
                  if (copylev > 0) {
                    if (id < ctr) {
                      if (ctr <= LEN) {
                        if (!(id >= 0)) goto ERROR;
                        id ++;
                      } else {
                        return ((char *)0);
                      }
                    } else {
                      return ((char *)0);
                    }
                    if (id < ctr) {
                      if (ctr <= LEN) {
                        if (!(id >= 0)) goto ERROR;
                        id ++;
                      } else {
                        return ((char *)0);
                      }
                    } else {
                      return ((char *)0);
                    }
                  }
                  goto putg;
                }
              }
              id = bufheadctr;
              if (unknown()) {
                if (id < ctr) {
                  if (ctr <= LEN) {
                    if (!(id >= 0)) goto ERROR;
                    id ++;
                  } else {
                    return ((char *)0);
                  }
                } else {
                  return ((char *)0);
                }
                while (1) {
                  if (unknown()) {
                    if (unknown()) {
                      if (unknown()) {
                        break;
                      }
                    } else {
                      break;
                    }
                  } else {
                    break;
                  }
                  continue;
                }
              }
              while (unknown()) {
                tmp___10 = unknown();
                c = (char )tmp___10;
                if (unknown()) {
                  if ((int )c == 34) {
                    if (id < ctr) {
                      if (ctr <= LEN) {
                        if (!(id >= 0)) goto ERROR;
                        id ++;
                      } else {
                        return ((char *)0);
                      }
                    } else {
                      return ((char *)0);
                    }
                    if (id < ctr) {
                      if (ctr <= LEN) {
                        if (!(id >= 0)) goto ERROR;
                        id ++;
                      } else {
                        return ((char *)0);
                      }
                    } else {
                      return ((char *)0);
                    }
                  } else {
                    goto M_L___1;
                  }
                } else {
                  M_L___1:
                  if (id < ctr) {
                    if (ctr <= LEN) {
                      if (!(id >= 0)) goto ERROR;
                      id ++;
                    } else {
                      return ((char *)0);
                    }
                  } else {
                    return ((char *)0);
                  }
                }
              }
              if (unknown()) {
                if (id == bufheadctr + 1) {
                  id --;
                } else {
                  if (id < ctr) {
                    if (ctr <= LEN) {
                      if (!(id >= 0)) goto ERROR;
                      id ++;
                    } else {
                      return ((char *)0);
                    }
                  } else {
                    return ((char *)0);
                  }
                }
                while (1) {
                  c = unknown();
                  if (! ((int )c != 58)) {
                    break;
                  }
                  if (id < ctr) {
                    if (ctr <= LEN) {
                      if (!(id >= 0)) goto ERROR;
                      id ++;
                    } else {
                      return ((char *)0);
                    }
                  } else {
                    return ((char *)0);
                  }
                }
                if (id < ctr) {
                  if (ctr <= LEN) {
                    if (!(id >= 0)) goto ERROR;
                    id ++;
                  } else {
                    return ((char *)0);
                  }
                } else {
                  return ((char *)0);
                }
              }
              while (unknown()) {
                if (id < ctr) {
                  if (ctr <= LEN) {
                    if (!(id >= 0)) goto ERROR;
                    id ++;
                  } else {
                    return ((char *)0);
                  }
                } else {
                  return ((char *)0);
                }
              }
              copylev = 0;
              bufheadctr = id;
              continue;
            }
          }
        }
      }
    }
    if ((int )c == 59) {
      if (copylev <= 0) {
        if (! ColonOkInAddr) {
          if (id < ctr) {
            if (ctr <= LEN) {
              if (!(id >= 0)) goto ERROR;
              id ++;
            } else {
              return ((char *)0);
            }
          } else {
            return ((char *)0);
          }
        }
      }
    }
    tmp___20 = unknown();
    if ((unsigned long )tmp___20 != (unsigned long )((void *)0)) {
      if (cmtlev <= 0) {
        if (! unknown()) {
        }
      }
    }
    if ((int )c == 60) {
      if (unknown()) {
      }
      anglelev = 1;
      if (id < ctr) {
        if (ctr <= LEN) {
          if (! unknown()) {
            ctr --;
          }
        }
      }
      id = bufheadctr;
      if (unknown()) {
        if (id < ctr) {
          if (ctr <= LEN) {
            if (!(id >= 0)) goto ERROR;
            id ++;
          } else {
            return ((char *)0);
          }
        } else {
          return ((char *)0);
        }
        while (unknown()) {
          continue;
        }
      }
      while (unknown()) {
        tmp___22 = unknown();
        c = (char )tmp___22;
        if (unknown()) {
          if ((int )c == 34) {
            if (id < ctr) {
              if (ctr <= LEN) {
                if (!(id >= 0)) goto ERROR;
                id ++;
              } else {
                return ((char *)0);
              }
            } else {
              return ((char *)0);
            }
            if (id < ctr) {
              if (ctr <= LEN) {
                
                id ++;
              } else {
                return ((char *)0);
              }
            } else {
              return ((char *)0);
            }
          } else {
            goto M_L___2;
          }
        } else {
          M_L___2:
          if (id < ctr) {
            if (ctr <= LEN) {
              if (!(id >= 0)) goto ERROR;
              id ++;
            } else {
              return ((char *)0);
            }
          } else {
            return ((char *)0);
          }
        }
      }
      if (unknown()) {
        if (id == 1) {
          id --;
        } else {
          if (id < ctr) {
            if (ctr <= LEN) {
              if (!(id >= 0)) goto ERROR;
              id ++;
            } else {
              return ((char *)0);
            }
          } else {
            return ((char *)0);
          }
        }
        while (1) {
          c = unknown();
          if (! ((int )c != 60)) {
            break;
          }
          if (id < ctr) {
            if (ctr <= LEN) {
              if (!(id >= 0)) goto ERROR;
              id ++;
            } else {
              return ((char *)0);
            }
          } else {
            return ((char *)0);
          }
        }
        if (id < ctr) {
          if (ctr <= LEN) {
            if (!(id >= 0)) goto ERROR;
            id ++;
          } else {
            return ((char *)0);
          }
        } else {
          return ((char *)0);
        }
      }
      copylev = 0;
      continue;
    }
    if ((int )c == 62) {
      if (anglelev > 0) {
        anglelev --;
        if (id < ctr) {
          if (ctr <= LEN) {
            if (unknown()) {
              ctr ++;
            }
          }
        }
      } else {
        if (id < ctr) {
          if (ctr <= LEN) {
            if (copylev > 0) {
              id --;
            }
            continue;
          }
        }
      }
      tmp___31 = copylev;
      copylev ++;
      if (tmp___31 <= 0) {
        if (id < ctr) {
          if (ctr <= LEN) {
            if (!(id >= 0)) goto ERROR;
            id ++;
          } else {
            return ((char *)0);
          }
        } else {
          return ((char *)0);
        }
      }
      continue;
    }
    putg:
    if (copylev <= 0) {
      if (id > 0) {
        if (id < ctr) {
          if (ctr <= LEN) {
            if (!(id >= 0)) goto ERROR;
            id ++;
          } else {
            return ((char *)0);
          }
        } else {
          return ((char *)0);
        }
      }
      if (id < ctr) {
        if (ctr <= LEN) {
          if (!(id >= 0)) goto ERROR;
          id ++;
        } else {
          return ((char *)0);
        }
      } else {
        return ((char *)0);
      }
      if (id < ctr) {
        if (ctr <= LEN) {
          if (!(id >= 0)) goto ERROR;
          id ++;
        } else {
          return ((char *)0);
        }
      } else {
        return ((char *)0);
      }
    }
  }
  if (unknown()) {
    if (id < LEN) {
      id ++;
    }
  }
  while (1) {
    tmp___38 = realcmtlev;
    realcmtlev --;
    if (tmp___38 > 0) {
      if (! (id < LEN)) {
        break;
      }
    } else {
      break;
    }
    id ++;
  }
  if (unknown()) {
    if (id < LEN) {
      id ++;
    }
  }
  buf[0] = (char )'<';
  buf[1] = (char)-127;
  buf[2] = (char )'g';
  buf[3] = (char )'>';
  buf[4] = (char )'\000';
  return (buf);
ERROR: sassert(0);
}
int main(void)
{
  crackaddr();
  return (0);
}