#include "motor.h"
#include "interface.h"
#include "stm32f10x.h"

// ��ʼ�����
void MotorInit(void)
{
    MotorGPIO_Configuration();
    CarStop();
}

// GPIO���ú���
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

//����ռ�ձ��������ת����Ӧ���жϵ���
void CarMove(void)
{
    //����
    if (left_speed_duty > 0) //��ǰ
    {
        if (speed_count < left_speed_duty)
        {
            FRONT_LEFT_GO;
            BEHIND_LEFT_GO;
        }
        else
        {
            FRONT_LEFT_STOP;
            BEHIND_LEFT_STOP;
        }
    }
    else if (left_speed_duty < 0) //���
    {
        if (speed_count < (-1) * left_speed_duty)
        {
            FRONT_LEFT_BACK;
            BEHIND_LEFT_BACK;
        }
        else
        {
            FRONT_LEFT_STOP;
            BEHIND_LEFT_STOP;
        }
    }
    else //ֹͣ
    {
        FRONT_LEFT_STOP;
        BEHIND_LEFT_STOP;
    }
    //����
    if (right_speed_duty > 0) //��ǰ
    {
        if (speed_count < right_speed_duty)
        {
            BEHIND_RIGHT_GO;
            FRONT_RIGHT_GO;
        }
        else //ֹͣ
        {
            BEHIND_RIGHT_STOP;
            FRONT_RIGHT_STOP;
        }
    }
    else if (right_speed_duty < 0) //���
    {
        if (speed_count < (-1) * right_speed_duty)
        {
            FRONT_RIGHT_BACK;
            BEHIND_RIGHT_BACK;
        }
        else //ֹͣ
        {
            FRONT_RIGHT_STOP;
            BEHIND_RIGHT_STOP;
        }
    }
    else //ֹͣ
    {
        FRONT_RIGHT_STOP;
        BEHIND_RIGHT_STOP;
    }
}

//��ǰ
void CarGo(void)
{
    if (HalfSpeed)
        right_speed_duty = left_speed_duty = SPEED_DUTY / 2;
    else
        right_speed_duty = left_speed_duty = SPEED_DUTY;
}

//����
void CarBack(void)
{
    if (HalfSpeed)
        right_speed_duty = left_speed_duty = -SPEED_DUTY / 2;
    else
        right_speed_duty = left_speed_duty = -SPEED_DUTY;
}

//����
void CarLeft(void)
{
    if (HalfSpeed)
    {
        right_speed_duty = SPEED_DUTY / 2;
        left_speed_duty = -SPEED_DUTY / 2;
    }
    else
    {
        right_speed_duty = SPEED_DUTY;
        left_speed_duty = -SPEED_DUTY;
    }
}

//����
void CarRight(void)
{
    if (HalfSpeed)
    {
        right_speed_duty = -SPEED_DUTY / 2;
        left_speed_duty = SPEED_DUTY / 2;
    }
    else
    {
        right_speed_duty = -SPEED_DUTY;
        left_speed_duty = SPEED_DUTY;
    }
}

//ֹͣ
void CarStop(void)
{
    right_speed_duty = left_speed_duty = 0;
}
