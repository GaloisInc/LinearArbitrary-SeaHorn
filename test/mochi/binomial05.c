#include "seahorn/seahorn.h"

int bin_without_checking_1089 (int, int, int, int, int, int, int, int);

int bin_1030 (int x_DO_NOT_CARE_1091, int x_DO_NOT_CARE_1092, int x_DO_NOT_CARE_1093, int n_1031, int prev_set_flag_bin_1076, int s_prev_bin_n_1074, int s_prev_bin_k_1075, int k_1032)
{
  if (prev_set_flag_bin_1076) {
    if (((0 * 1) + (0 * s_prev_bin_n_1074)) + (1 * s_prev_bin_k_1075)
                 > ((0 * 1) + (0 * n_1031)) + (1 * k_1032) &&
                 ((0 * 1) + (0 * n_1031)) + (1 * k_1032) >= 0 ||
                 ((0 * 1) + (0 * s_prev_bin_n_1074)) + (1 * s_prev_bin_k_1075)
                 >= ((0 * 1) + (0 * n_1031)) + (1 * k_1032) &&
                 (((0 * 1) + (1 * s_prev_bin_n_1074)) + (-s_prev_bin_k_1075) >
                  ((0 * 1) + (1 * n_1031)) + (-k_1032) &&
                  ((0 * 1) + (1 * n_1031)) + (-k_1032) >= 0))
      {;}
    else
      sassert(0);
  }
  return bin_without_checking_1089 (x_DO_NOT_CARE_1091, x_DO_NOT_CARE_1092,
              x_DO_NOT_CARE_1093, n_1031, prev_set_flag_bin_1076,
              s_prev_bin_n_1074, s_prev_bin_k_1075, k_1032);
}

int bin_without_checking_1089 (int x_DO_NOT_CARE_1095, int x_DO_NOT_CARE_1096, int x_DO_NOT_CARE_1097, int n_1031, int set_flag_bin_1077, int s_bin_n_1072, int s_bin_k_1073, int k_1032)
{
    set_flag_bin_1077 = 1;
    s_bin_k_1073 = k_1032;
    s_bin_n_1072 = n_1031;
     
    if (n_1031 == 0)
      return 1;
    else
      if (k_1032 <= 0 || k_1032 >= n_1031)
        return 1;
      else
        return (bin_1030 (set_flag_bin_1077, s_bin_n_1072, s_bin_k_1073, (n_1031 - 1),
             set_flag_bin_1077, s_bin_n_1072, s_bin_k_1073, (k_1032 - 1))
           +
           bin_without_checking_1089 (set_flag_bin_1077, s_bin_n_1072,
             s_bin_k_1073, (n_1031 - 1), set_flag_bin_1077, s_bin_n_1072,
             s_bin_k_1073, k_1032));
}

int main_1033 (int set_flag_bin_1077, int s_bin_n_1072, int s_bin_k_1073, int n_1035, int k_1036)
{
  if (n_1035 >= 0 && k_1036 >= 0)
    return bin_without_checking_1089 (set_flag_bin_1077, s_bin_n_1072, s_bin_k_1073,
           n_1035, set_flag_bin_1077, s_bin_n_1072, s_bin_k_1073, k_1036);
  else
    return 0;
}

int main () {
  int n = unknown();
  int k = unknown();
  return main_1033 (0, 0, 0, n, k);
}