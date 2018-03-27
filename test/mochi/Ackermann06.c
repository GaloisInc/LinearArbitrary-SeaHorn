#include "seahorn/seahorn.h"

  int ack_without_checking_1087(int, int, int, int, int, int, int, int);

  int ack_1030 (int x_DO_NOT_CARE_1089, int x_DO_NOT_CARE_1090, int x_DO_NOT_CARE_1091, int m_1031, int prev_set_flag_ack_1074, int s_prev_ack_m_1072, int s_prev_ack_n_1073, int n_1032) {
     if (prev_set_flag_ack_1074) {
              if (((0 * 1) + (1 * s_prev_ack_m_1072)) + (0 * s_prev_ack_n_1073)
                 > ((0 * 1) + (1 * m_1031)) + (0 * n_1032) &&
                 ((0 * 1) + (1 * m_1031)) + (0 * n_1032) >= 0 ||
                 ((0 * 1) + (1 * s_prev_ack_m_1072)) + (0 * s_prev_ack_n_1073)
                 >= ((0 * 1) + (1 * m_1031)) + (0 * n_1032) &&
                 (((0 * 1) + (0 * s_prev_ack_m_1072)) + (1 * s_prev_ack_n_1073)
                  > ((0 * 1) + (0 * m_1031)) + (1 * n_1032) &&
                  ((0 * 1) + (0 * m_1031)) + (1 * n_1032) >= 0)) {;}
              else
                sassert (0);
      }
      return ack_without_checking_1087 (x_DO_NOT_CARE_1089, x_DO_NOT_CARE_1090,
              x_DO_NOT_CARE_1091, m_1031, prev_set_flag_ack_1074,
              s_prev_ack_m_1072, s_prev_ack_n_1073, n_1032);
    }

  int ack_without_checking_1087 (int x_DO_NOT_CARE_1093, int x_DO_NOT_CARE_1094, int x_DO_NOT_CARE_1095, int m_1031, int set_flag_ack_1075, int s_ack_m_1070, int s_ack_n_1071, int n_1032) {
    set_flag_ack_1075 = 1;
    s_ack_n_1071 = n_1032;
    s_ack_m_1070 = m_1031;
    if (m_1031 == 0)
      return n_1032 + 1;
    else
      if (n_1032 == 0) 
        return ack_1030 (set_flag_ack_1075, s_ack_m_1070, s_ack_n_1071, (m_1031 - 1),
             set_flag_ack_1075, s_ack_m_1070, s_ack_n_1071, 1);
      else
        return ack_without_checking_1087 (set_flag_ack_1075, s_ack_m_1070,
             s_ack_n_1071, (m_1031 - 1), set_flag_ack_1075, s_ack_m_1070,
             s_ack_n_1071,
             (ack_without_checking_1087 (set_flag_ack_1075, s_ack_m_1070,
               s_ack_n_1071, m_1031, set_flag_ack_1075, s_ack_m_1070, s_ack_n_1071,
               (n_1032 - 1))));
  }

  int main_1033 (int set_flag_ack_1075, int s_ack_m_1070, int s_ack_n_1071, int m_1035, int n_1036) {
      if (n_1036 > 0 && m_1035 > 0)
        return ack_without_checking_1087 (set_flag_ack_1075, s_ack_m_1070, s_ack_n_1071,
           m_1035, set_flag_ack_1075, s_ack_m_1070, s_ack_n_1071, n_1036);
      else
        return 0;
  }

  int main () {
    int m = unknown();
    int n = unknown();
    return main_1033 (0, 0, 0, m, n);
   }