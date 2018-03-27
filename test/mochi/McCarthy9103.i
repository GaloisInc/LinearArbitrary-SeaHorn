#include "seahorn/seahorn.h"

int mc91_without_checking_1058(int ,int, int);

int mc91_1030 (int prev_set_flag_mc91_1049, int s_prev_mc91_n_1048, int n_1031) {
  if (prev_set_flag_mc91_1049) {
    if ((111 * 1) + (-s_prev_mc91_n_1048) > (111 * 1) + (-n_1031) &&
        (111 * 1) + (-n_1031) >= 0) 
      {;}          
    else
      sassert (0);
  }
  return mc91_without_checking_1058 (prev_set_flag_mc91_1049,
              s_prev_mc91_n_1048, n_1031);
}

int mc91_without_checking_1058 (int set_flag_mc91_1050, int s_mc91_n_1047, int n_1031) {
    set_flag_mc91_1050 = 1;
    s_mc91_n_1047 = n_1031;
    if (n_1031 > 100)
      return n_1031 - 10;
    else
      return mc91_1030 (set_flag_mc91_1050, s_mc91_n_1047,
           (mc91_without_checking_1058 (set_flag_mc91_1050, s_mc91_n_1047,
             (n_1031 + 11))));
}

int main () {
    int r = unknown();
    int set_flag_mc91_1050 = 0;
    int s_mc91_n_1047 = 0;
    return mc91_without_checking_1058 (set_flag_mc91_1050, s_mc91_n_1047, r);
}