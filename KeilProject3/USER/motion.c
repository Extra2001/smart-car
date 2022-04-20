#include "motion.h"
#include "interface.h"
#include "motor.h"
#include "led.h"
#include "UltrasonicCtrol.h"

// PID 控制量
float Kp = 40, Ki = 0.25, Kd = 0.3;
float error = 0, previous_error = 0, P = 0, I = 0, D = 0, PID_value = 0;
char ctrl_comm_last = COMM_STOP;      //上一次的指令
char move_status = 0;                 // 0:按指令行车, 1:等待循迹启停板撤除, 2:循迹中
char search_line_mode = 0;            // 0:差速因子,1:PID,2:占空比
char front_duty = 5, behind_duty = 1; // 转弯时的前后轮占空比
char lfsdbak = 0, lbsdbak = 0, rfsdbak = 0, rbsdbak = 0;

void ToggleSearchLine(void)
{
    error = 0, previous_error = 0, P = 0, I = 0, D = 0, PID_value = 0;
    SetSpeedDutyAll(0);
    if (move_status == 0)
    {
        move_status = 1;
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
    if (I > 20)
        I = 20;
    if (I < -20)
        I = -20;
    D = error - previous_error;
    PID_value = (Kp * P) + (Ki * I) + (Kd * D);
    previous_error = error;
}

void SearchLine(void)
{
    Search_L = SEARCH_L_IO;
    Search_M = SEARCH_M_IO;
    Search_R = SEARCH_R_IO;

    if (Search_L == BLACK_AREA && Search_M == BLACK_AREA && Search_R == BLACK_AREA)
    {
        if (void_left)
        {
            SetSpeedDutyAll(0);
            lfsdbak = lbsdbak = rfsdbak = rbsdbak = 0;
            return;
        }
    }
    switch (search_line_mode)
    {
    case 0:
        if (Search_L == BLACK_AREA)
            lfsdbak = front_duty, lbsdbak = behind_duty, rfsdbak = rbsdbak = SPEED_DUTY;
        else if (Search_R == BLACK_AREA)
            lfsdbak = lbsdbak = SPEED_DUTY, rfsdbak = front_duty, rbsdbak = behind_duty;
        else if (Search_M == BLACK_AREA)
            lfsdbak = lbsdbak = rfsdbak = rbsdbak = SPEED_DUTY;
        break;
    case 1:
        if (Search_L == BLACK_AREA)
            error = -1;
        else if (Search_R == BLACK_AREA)
            error = 1;
        else if (Search_M == BLACK_AREA)
            error = 0;
        CalcPID();
        SetSpeedDutySide(SPEED_DUTY + (char)(PID_value + 0.5), SPEED_DUTY - (char)(PID_value + 0.5));
        break;
    default:
        break;
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
            SetSpeedDutyAll(SPEED_DUTY);
            break;
        case COMM_DOWN: //小车后退
            SetSpeedDutyAll(-SPEED_DUTY);
            break;
        case COMM_LEFT: //小车左转
            SetSpeedDutySide(10, SPEED_DUTY);
            break;
        case COMM_RIGHT: //小车右转
            SetSpeedDutySide(SPEED_DUTY, 10);
            break;
        case COMM_STOP: //小车停车
            SetSpeedDutyAll(0);
            break;
        default:
            break;
        }
        Delayms(2); //防抖
    }
}

void Upward()
{
    if (distance_cm < 5)
    {
        SetSpeedDutyAll(-15);
    }
    else if (distance_cm < 8)
    {
        SetSpeedDutyAll(0);
    }
    else if (distance_cm < 10)
    {
        SetSpeedDutyAll(15);
    }
    else if (distance_cm < 15)
    {
        SearchLine();
        SetSpeedDuty(lfsdbak / 6, lbsdbak / 6, rfsdbak / 6, rbsdbak / 6);
    }
    else if (distance_cm < 30)
    {
        SearchLine();
        SetSpeedDuty(lfsdbak / 4, lbsdbak / 4, rfsdbak / 4, rbsdbak / 4);
    }
    else
    {
        SearchLine();
        SetSpeedDuty(lfsdbak, lbsdbak, rfsdbak, rbsdbak);
    }
}

//小车运动控制
void Move(void)
{
    switch (move_status)
    {
    case 0:
        FollowInstruction();
        break;
    case 1:
        Upward();
        break;
    case 2:
        break;
    default:
        break;
    }
}
