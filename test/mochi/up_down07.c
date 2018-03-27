#include "seahorn/seahorn.h"

#define c5_COEFFICIENT_1082 1
#define c4_COEFFICIENT_1081 1
#define c3_COEFFICIENT_1080 1
#define c2_COEFFICIENT_1078 1
#define c1_COEFFICIENT_1077 1
#define c0_COEFFICIENT_1076 1

int up_1032 (int, int, int, int);

int down_1031(int, int, int, int);

int app_without_checking_1193 (int x_DO_NOT_CARE_1221, int x_DO_NOT_CARE_1222, int x_DO_NOT_CARE_1223, int f_EXPARAM_1084, int x_DO_NOT_CARE_1218, int x_DO_NOT_CARE_1219, int x_DO_NOT_CARE_1220, int set_flag_app_1179, int s_app_f_EXPARAM_1172, int s_app_x_1174, int x_1034) 
{
    set_flag_app_1179 = 1;
    s_app_x_1174 = x_1034;
    s_app_f_EXPARAM_1172 = f_EXPARAM_1084;
    return up_1032 (set_flag_app_1179, s_app_f_EXPARAM_1172, s_app_x_1174, x_1034);
}

int app_without_checking_1194 (int x_DO_NOT_CARE_1221, int x_DO_NOT_CARE_1222, int x_DO_NOT_CARE_1223, int f_EXPARAM_1084, int x_DO_NOT_CARE_1218, int x_DO_NOT_CARE_1219, int x_DO_NOT_CARE_1220, int set_flag_app_1179, int s_app_f_EXPARAM_1172, int s_app_x_1174, int x_1034) 
{
     set_flag_app_1179 = 1;
     s_app_x_1174 = x_1034;
     s_app_f_EXPARAM_1172 = f_EXPARAM_1084;
     return down_1031 (set_flag_app_1179, s_app_f_EXPARAM_1172, s_app_x_1174, x_1034);
}

int app_1030 (int x_DO_NOT_CARE_1198, int x_DO_NOT_CARE_1199, int x_DO_NOT_CARE_1200, int f_EXPARAM_1084, int x_DO_NOT_CARE_1195, int x_DO_NOT_CARE_1196, int x_DO_NOT_CARE_1197, int prev_set_flag_app_1178, int s_prev_app_f_EXPARAM_1175, int s_prev_app_x_1177, int x_1034)
{
    if (prev_set_flag_app_1178)
      sassert(0);
            
    return app_without_checking_1194 (x_DO_NOT_CARE_1198, x_DO_NOT_CARE_1199,
              x_DO_NOT_CARE_1200, f_EXPARAM_1084, x_DO_NOT_CARE_1195,
              x_DO_NOT_CARE_1196, x_DO_NOT_CARE_1197,
              prev_set_flag_app_1178, s_prev_app_f_EXPARAM_1175,
              s_prev_app_x_1177, x_1034);
}

int down_1031 (int set_flag_app_1179, int s_app_f_EXPARAM_1172, int s_app_x_1174, int x_1035)
{
    if (x_1035 == 0)
    {return 0;}
    else {
       return down_1031 (set_flag_app_1179, s_app_f_EXPARAM_1172, s_app_x_1174,
         (x_1035 - 1));
    }
}

int up_1032 (int set_flag_app_1179, int s_app_f_EXPARAM_1172, int s_app_x_1174, int x_1036) 
{
     if (x_1036 == 0)
       {return 0;}
     else
       return up_1032 (set_flag_app_1179, s_app_f_EXPARAM_1172, s_app_x_1174, (x_1036 + 1));
}

int main () {
    int t1_1039 = unknown();
    int t2_1040 = unknown();
    int set_flag_app_1179 = 0;
    int s_app_f_EXPARAM_1172 = 0;
    int s_app_x_1174  = 0;
    if (t1_1039 > 0)
         return app_1030 (set_flag_app_1179, s_app_f_EXPARAM_1172, s_app_x_1174,
           ((c3_COEFFICIENT_1080 * t2_1040) +
            ((c4_COEFFICIENT_1081 * t1_1039) + c5_COEFFICIENT_1082)),
           set_flag_app_1179, s_app_f_EXPARAM_1172, s_app_x_1174,
           set_flag_app_1179, s_app_f_EXPARAM_1172, s_app_x_1174, t1_1039);
    else {
         if (t2_1040 < 0)
           return app_without_checking_1193 (set_flag_app_1179, s_app_f_EXPARAM_1172,
             s_app_x_1174,
             ((c0_COEFFICIENT_1076 * t2_1040) +
              ((c1_COEFFICIENT_1077 * t1_1039) + c2_COEFFICIENT_1078)),
             set_flag_app_1179, s_app_f_EXPARAM_1172, s_app_x_1174,
             set_flag_app_1179, s_app_f_EXPARAM_1172, s_app_x_1174, t2_1040);
    }
    return 0;
}