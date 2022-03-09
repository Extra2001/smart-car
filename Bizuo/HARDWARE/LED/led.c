#include "led.h"
#include "interface.h"
#include "stm32f10x.h"

void LightInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //左前灯设置LF_Light_SET;
    GPIO_InitStructure.GPIO_Pin = LF_Light_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //配置GPIO端口速度
    GPIO_Init(LF_Light_GPIO, &GPIO_InitStructure);

    //右前灯设置RF_Light_SET;
    GPIO_InitStructure.GPIO_Pin = RF_Light_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //配置GPIO端口速度
    GPIO_Init(RF_Light_GPIO, &GPIO_InitStructure);

    //左后灯设置LB_Light_SET;
    GPIO_InitStructure.GPIO_Pin = LB_Light_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //配置GPIO端口速度
    GPIO_Init(LB_Light_GPIO, &GPIO_InitStructure);

    //右后灯设置RB_Light_SET;
    GPIO_InitStructure.GPIO_Pin = RB_Light_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //配置GPIO端口速度
    GPIO_Init(RB_Light_GPIO, &GPIO_InitStructure);
}

void BrightInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = BRIGHT_PIN;         //配置使能GPIO管脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     //配置GPIO模式,输入上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //配置GPIO端口速度
    GPIO_Init(BRIGHT_GPIO, &GPIO_InitStructure);
}

void UserLEDInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //配置GPIO端口速度
    GPIO_Init(LED_GPIO, &GPIO_InitStructure);
    LED_SET;
}

// 打开车灯
void LightOpen()
{
    LF_Light_SET;
    RF_Light_SET;
    LB_Light_SET;
    RB_Light_SET;
    LB_Light = RB_Light = LF_Light = RF_Light = 1;
}

// 关闭车灯
void LightClose()
{
    LF_Light_RESET;
    RF_Light_RESET;
    LB_Light_RESET;
    RB_Light_RESET;
    LB_Light = RB_Light = LF_Light = RF_Light = 0;
}

void LEDToggle(uint16_t Led)
{
    /* 指定管脚输出异或 1，实现对应的LED指示灯状态取反目的 */
    LED_GPIO->ODR ^= Led;
    //若要提高效率，建议直接调用 LEDnOBB = !LEDnOBB;
}

//环境光亮度检测
void BrightCheck(void)
{
    Bright_check = BRIGHT_IO;
    if (Bright_check == BRIGHT_Y)
        LightClose();
    else
        LightOpen();
}
