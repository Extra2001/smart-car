#include "UltrasonicCtrol.h"
#include "interface.h"
#include "stm32f10x.h"

volatile unsigned char status = 0;       //程序当前状态，0,空闲 1 发送触发信号，2 等待信号返回,3
unsigned int dis_count = 0;              //脉宽长计时
volatile unsigned int distance_cm = 0;   //当前距离
volatile unsigned int distance_tran = 0; //传输距离

void Time6Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = 65535;
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1); // 72M / 72 = 1us
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
}

//外部中断配置 超声波,这里没用到中断计数方式
void UltraSoundInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure; //定义一个外部中断相关的结构体
    NVIC_InitTypeDef NVIC_InitStructure; //定义一个中断的结构体

    GPIO_InitStructure.GPIO_Pin = Echo_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //配置GPIO端口速度
    GPIO_Init(IRIN_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = Trig_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //配置GPIO端口速度
    GPIO_Init(Trig_GPIO, &GPIO_InitStructure);

    GPIO_EXTILineConfig(Echo_PORTSOURCE, Echo_PINSOURCE);   //将GPIO口配置成外部中断
    EXTI_InitStructure.EXTI_Line = Echo_EXITLINE;           //将对应的GPIO口连接到中断线上
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //中断事件类型，下降沿
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     //选择模式，中断型
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;              //使能该中断
    EXTI_Init(&EXTI_InitStructure);                         //将配置好的参数写入寄存器

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); //阶级为0，不可嵌套
    NVIC_InitStructure.NVIC_IRQChannel = Echo_IRQCH;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //主优先级0，最高
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //子优先级，最低
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //使能该模块中断
    NVIC_Init(&NVIC_InitStructure);                           //中断初始化，将结构体定义的数据执行

    Time6Init();
    Trig_RESET;
}

//延时的方式读取距离值
short GetDistanceDelay(void)
{
    if (status != 0 && status != 3)
        return 0;
    //发送触发信号
    Trig_SET;
    status = 1;
    TIM_SetCounter(TIM6, 0);
    TIM_Cmd(TIM6, ENABLE); //打开定时器
    while (TIM_GetCounter(TIM6) < 11)
        ; //延时超过10us
    status = 2;
    Trig_RESET;
    TIM_SetCounter(TIM6, 0);

    while (Echo == 0) //等待回向信号起始位置
    {
        if (TIM_GetCounter(TIM6) > 10000) //回应时间超出正常范围
        {
            status = 3;
            distance_cm = 100;      //失败后就后退
            TIM_Cmd(TIM6, DISABLE); //关闭定时器
            return 0;               //本次失败
        }
    }
    TIM_SetCounter(TIM6, 0);
    while (Echo == 1) //开始计算长度
    {
        int count = TIM_GetCounter(TIM6);
        if (count > 10000) //回应时间超出正常范围
        {
            status = 3;
            distance_cm = 100;      //失败后就后退
            TIM_Cmd(TIM6, DISABLE); //关闭定时器
            return 0;               //本次失败
        }
    }
    dis_count = TIM_GetCounter(TIM6);
    TIM_Cmd(TIM6, DISABLE);                                      //关闭定时器
    distance_cm = (unsigned int)(((long)(dis_count)*34) / 2000); //声速340m/s
    distance_tran = distance_cm;                                 //避免发出距离为0的数据
    status = 0;                                                  //准备下次发送
    return distance_tran;
}
