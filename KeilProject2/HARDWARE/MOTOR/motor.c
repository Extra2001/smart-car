#include "motor.h"
#include "interface.h"
#include "stm32f10x.h"

// 初始化电机
void MotorInit(void)
{
    MotorGPIO_Configuration();
    SetSpeedDutyAll(0);
}

// GPIO配置函数
void MotorGPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = FRONT_LEFT_F_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(FRONT_LEFT_F_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = FRONT_LEFT_B_PIN;
    GPIO_Init(FRONT_LEFT_B_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = FRONT_RIGHT_F_PIN;
    GPIO_Init(FRONT_RIGHT_F_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = FRONT_RIGHT_B_PIN;
    GPIO_Init(FRONT_RIGHT_B_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = BEHIND_LEFT_F_PIN;
    GPIO_Init(BEHIND_LEFT_F_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = BEHIND_LEFT_B_PIN;
    GPIO_Init(BEHIND_LEFT_B_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = BEHIND_RIGHT_F_PIN;
    GPIO_Init(BEHIND_RIGHT_F_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = BEHIND_RIGHT_B_PIN;
    GPIO_Init(BEHIND_RIGHT_B_GPIO, &GPIO_InitStructure);
}

void SetSpeedDuty(char lf, char lb, char rf, char rb)
{
    left_front_speed_duty = lf;
    left_behind_speed_duty = lb;
    right_front_speed_duty = rf;
    right_behind_speed_duty = rb;
}

void SetSpeedDutyAll(char duty)
{
    left_behind_speed_duty = left_front_speed_duty = duty;
    right_behind_speed_duty = right_front_speed_duty = duty;
}

void SetSpeedDutySide(char left, char right)
{
    left_behind_speed_duty = left_front_speed_duty = left;
    right_behind_speed_duty = right_front_speed_duty = right;
}

//根据占空比驱动电机转动，应由中断调用
void CarMove(void)
{
    if (left_front_speed_duty >= 0)
    {
        if (speed_count < left_front_speed_duty)
        {
            FRONT_LEFT_GO;
        }
        else
        {
            FRONT_LEFT_STOP;
        }
    }
    else
    {
        if (speed_count < -left_front_speed_duty)
        {
            FRONT_LEFT_BACK;
        }
        else
        {
            FRONT_LEFT_STOP;
        }
    }

    if (left_behind_speed_duty >= 0)
    {
        if (speed_count < left_behind_speed_duty)
        {
            BEHIND_LEFT_GO;
        }
        else
        {
            BEHIND_LEFT_STOP;
        }
    }
    else
    {
        if (speed_count < -left_behind_speed_duty)
        {
            BEHIND_LEFT_BACK;
        }
        else
        {
            BEHIND_LEFT_STOP;
        }
    }

    if (right_front_speed_duty >= 0)
    {
        if (speed_count < right_front_speed_duty)
        {
            FRONT_RIGHT_GO;
        }
        else
        {
            FRONT_RIGHT_STOP;
        }
    }
    else
    {
        if (speed_count < -right_front_speed_duty)
        {
            FRONT_LEFT_BACK;
        }
        else
        {
            FRONT_RIGHT_STOP;
        }
    }

    if (right_behind_speed_duty >= 0)
    {
        if (speed_count < right_behind_speed_duty)
        {
            BEHIND_RIGHT_GO;
        }
        else
        {
            BEHIND_RIGHT_STOP;
        }
    }
    else
    {
        if (speed_count < -right_behind_speed_duty)
        {
            BEHIND_RIGHT_BACK;
        }
        else
        {
            BEHIND_RIGHT_STOP;
        }
    }
}
