#include "motion.h"
#include "interface.h"
#include "motor.h"
#include "led.h"

// PID 控制量
float error = 0, previous_error = 0, P = 0, I = 0, D = 0, PID_value = 0;
char ctrl_comm_last = COMM_STOP; //上一次的指令
char Is_Movebyline = 0;          //是否循迹
char move_status = 0;            // 0:按指令行车, 1:等待循迹启停板, 2:等待循迹启停板撤除, 3:循迹中
char change_mode_counter = 0;
char pid_on = 0; // 0:差速因子,1:PID,2:占空比
float Kp = 30, Ki = 0.1, Kd = 0.1;
float factor = 0.3; // 差速因子
char striaght_duty = 50, turn_duty = 10;

void ToggleSearchLine(void)
{
    error = 0, previous_error = 0, P = 0, I = 0, D = 0, PID_value = 0;
    if (move_status == 0)
    {
        left_speed_duty = right_speed_duty = 0;
        move_status = 2;
        LightOpen();
        Delayms(300);
        LightClose();
        Delayms(150);
        LightOpen();
        Delayms(300);
        LightClose();
    }
    else
    {
        left_speed_duty = right_speed_duty = 0;
        move_status = 0;
        LightOpen();
        Delayms(700);
        LightClose();
    }
}

void CalcPID(void)
{
    P = error;
    I = I + error;
    D = error - previous_error;
    PID_value = (Kp * P) + (Ki * I) + (Kd * D);
    previous_error = error;
}

//循迹检测,通过判断三个光电对管的状态来控制小车运动
void SearchLine(void) //从传感器输出判断各路是否检测到黑线，给Search_L，Search_M，Search_R赋值，在interface.h中找循迹光电对管定义
{
    Search_L = SEARCH_L_IO;
    Search_M = SEARCH_M_IO;
    Search_R = SEARCH_R_IO;

    if (Search_L == BLACK_AREA && Search_M == BLACK_AREA && Search_R == BLACK_AREA)
    {
        left_speed_duty = right_speed_duty = 0;
        move_status = 2;
        return;
    }
    switch (pid_on)
    {
    case 0:
        if (Search_L == BLACK_AREA)
            left_speed_duty = SPEED_DUTY * factor, right_speed_duty = SPEED_DUTY;
        else if (Search_R == BLACK_AREA)
            left_speed_duty = SPEED_DUTY, right_speed_duty = SPEED_DUTY * factor;
        else if (Search_M == BLACK_AREA)
            left_speed_duty = SPEED_DUTY, right_speed_duty = SPEED_DUTY;
        break;
    case 1:
        if (Search_L == BLACK_AREA)
            error = -1;
        else if (Search_R == BLACK_AREA)
            error = 1;
        else if (Search_M == BLACK_AREA)
            error = 0;
        CalcPID();
        left_speed_duty = SPEED_DUTY + (char)(PID_value + 0.5);
        right_speed_duty = SPEED_DUTY - (char)(PID_value + 0.5);
        break;
    case 2:
        if (Search_L == BLACK_AREA)
            left_speed_duty = turn_duty, right_speed_duty = striaght_duty;
        else if (Search_R == BLACK_AREA)
            left_speed_duty = striaght_duty, right_speed_duty = turn_duty;
        else if (Search_M == BLACK_AREA)
            left_speed_duty = striaght_duty, right_speed_duty = striaght_duty;
        break;
    default:
        break;
    }
}
// 等待启停板装入
void WaitForStart(void)
{
    Search_L = SEARCH_L_IO;
    Search_M = SEARCH_M_IO;
    Search_R = SEARCH_R_IO;

    if (Search_L == BLACK_AREA && Search_M == BLACK_AREA && Search_R == BLACK_AREA)
    {
        if (change_mode_counter++ >= 3)
        {
            change_mode_counter = 0;
            move_status = 2;
        }
    }
}
// 等待启停板撤离
void WaitForGo(void)
{
    Search_L = SEARCH_L_IO;
    Search_M = SEARCH_M_IO;
    Search_R = SEARCH_R_IO;

    if (Search_L == WHITE_AREA || Search_M == WHITE_AREA || Search_R == WHITE_AREA)
    {
        if (change_mode_counter++ >= 3)
        {
            change_mode_counter = 0;
            move_status = 3;
        }
    }
}

//按指令行车
void FollowInstruction(void)
{
    if (ctrl_comm_last != ctrl_comm) //指令发生变化
    {
        ctrl_comm_last = ctrl_comm;
        switch (ctrl_comm)
        {
        case COMM_UP: //小车前进
            CarGo();
            break;
        case COMM_DOWN: //小车后退
            CarBack();
            break;
        case COMM_LEFT: //小车左转
            CarLeft();  // motor.c里编程
            break;
        case COMM_RIGHT: //小车右转
            CarRight();  // motor.c里编程
            break;
        case COMM_STOP: //小车停车
            CarStop();
            break;
        default:
            break;
        }
        Delayms(2); //防抖
    }
}

//小车运动控制
void Move(void)
{
    if (move_status == 0)
        FollowInstruction();
    else if (move_status == 1)
        WaitForStart();
    else if (move_status == 2)
        WaitForGo();
    else if (move_status == 3)
        SearchLine();
}
