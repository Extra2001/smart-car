#include "SpeedCtrol.h"
#include "interface.h"

//轮子直径66mm，光电码盘齿数为20，轮子周长 207mm = 20.7cm
//程序采用判断高低电平变化次数计数，也就是说轮子转一周计数次数为40
//一个计数变化表示轮子跑过的距离为 20.7/40 = 0.5175cm

unsigned char left_speed = 0;
unsigned char right_speed = 0;
unsigned char avg_speed = 0;

unsigned char left_speed_temp = 0;
unsigned char right_speed_temp = 0;

static unsigned char left_io = 0, right_io = 0;
static unsigned char left_count = 0, right_count = 0;
static unsigned char temp_io = 0;

void MeasureInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = FRONT_RIGHT_S_PIN;  //配置使能GPIO管脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     //配置GPIO模式,输入上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //配置GPIO端口速度
    GPIO_Init(FRONT_RIGHT_S_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = FRONT_LEFT_S_PIN;   //配置使能GPIO管脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     //配置GPIO模式,输入上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //配置GPIO端口速度
    GPIO_Init(FRONT_LEFT_S_GPIO, &GPIO_InitStructure);
}

/*******************************************************************************
 * 函 数 名 ：MeasureSpeed
 * 函数功能 ：速度测量，计算IO变化次数，该函数必须每0.1ms调用一次
 * 输    入 ：无
 * 输    出 ：无
 *******************************************************************************/
void MeasureSpeed(void)
{
    right_count++, left_count++;
    temp_io = FRONT_LEFT_S_IO;
    if (temp_io != left_io) //发生电平变化
    {
        left_speed_temp++;
        left_io = temp_io;
        if (left_speed_temp >= 4)
        {
            left_speed = (unsigned char)(5105.0 / left_count + 0.5);
            left_speed_temp = 0;
            left_count = 0;
        }
    }
    temp_io = FRONT_RIGHT_S_IO;
    if (temp_io != right_io) //发生电平变化
    {
        right_speed_temp++;
        right_io = temp_io;
        if (right_speed_temp >= 4)
        {
            right_speed = (unsigned char)(5105.0 / right_count + 0.5);
            right_speed_temp = 0;
            right_count = 0;
        }
    }
}
