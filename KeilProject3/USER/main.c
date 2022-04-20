#include "stm32f10x.h"
#include "interface.h"
#include "motor.h"
#include "UltrasonicCtrol.h"
#include "SpeedCtrol.h"
#include "sys.h"
#include "key.h"
#include "delay.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "malloc.h"
#include "usart.h"
#include "common.h"
#include "usart3.h"
#include "ov7670.h"
#include "exti.h"
#include "timer.h"
#include "led.h"
#include "motion.h"
#include <math.h>

void camera_refresh(void);

//全局变量定义
unsigned int speed_count = 0; //占空比计数器 50次一周期
char left_front_speed_duty = SPEED_DUTY, left_behind_speed_duty = SPEED_DUTY;
char right_front_speed_duty = SPEED_DUTY, right_behind_speed_duty = SPEED_DUTY;

unsigned char tick_5ms = 0;   // 5ms计数器，作为主函数的基本周期
unsigned char tick_200ms = 0; //刷新LED闪烁显示

char ctrl_comm = COMM_STOP; //控制指令

unsigned char duoji_count = 0;
unsigned char zhuanjiao = 11;

float pitch, roll, yaw;                                      //欧拉角
short aacx, aacy, aacz;                                      //加速度传感器原始数据
short gyrox, gyroy, gyroz;                                   //陀螺仪原始数据
short tempIMU;                                               //温度
char Bright_check = 0;                                       //亮度检测结果，1：明亮；0：黑暗
char Red_light = 0, Green_light = 0;                         //红绿灯识别结果，1：已识别；0：未识别
char Identification = 0;                                     //图形识别结果，待定
char void_right = 0, void_left = 0;                          //红外避障结果，1：无障碍；0：有障碍
char Search_L = 0, Search_M = 0, Search_R = 0;               //循迹结果，1：有黑线；0：无黑线
char LF_Light = 0, RF_Light = 0, LB_Light = 0, RB_Light = 0; //车灯状态，1：开灯；0：关灯
char dist[5] = {0}, cnt = 0;
short tmp_dist = 0;

char Is_Display = 0; //是否通过串口发送视频
char key = 0;        //按键输入

extern u8 ov_sta;   //在exit.c里面定义
extern u8 ov_frame; //在timer.c里面定义

char buff[1024];

void GetIMUData(void)
{
    tempIMU = MPU_Get_Temperature();
    MPU_Get_Gyroscope(&gyrox, &gyroy, &gyroz);
    MPU_Get_Accelerometer(&aacx, &aacy, &aacz);
}

void GetVoidData()
{
    void_right = VOID_R_IO;
    void_left = VOID_L_IO;
}

void func_5ms(void)
{
    key = KEY_Scan(0);
    if (key == KEY1_PRES) //按键1：是否通过串口发送图像
        Is_Display = !Is_Display;
    else if (key == KEY2_PRES) //按键2：是否循迹行车
        ToggleSearchLine();
    GetVoidData();
    GetDistanceDelay();
    BrightCheck(); //执行环境光亮度检测
    // atk_8266_wifista_Rece(); //控制信号接收
    Move(); //小车运动控制
    avg_speed = (left_speed + right_speed) / 2;
}

void func_200ms(void)
{
    GetIMUData();
    LEDToggle(LED_PIN); //用户指示灯闪烁,表明程序运行正常
    camera_refresh();   //摄像头刷新
}

void camera_refresh(void)
{
    u32 j;
    u16 color;
    char *p = buff;
    u3_printf("HHH%d", ov_sta);
    if (ov_sta) //有帧中断更新？
    {
        OV7670_RRST = 0; //开始复位读指针
        OV7670_RCK_L;
        OV7670_RCK_H;
        OV7670_RCK_L;
        OV7670_RRST = 1; //复位读指针结束
        OV7670_RCK_H;
        u3_printf("Hh");
        for (j = 0; j < 76800; j++)
        {
            OV7670_RCK_L;
            color = GPIOC->IDR & 0XFF; //读数据
            OV7670_RCK_H;
            color <<= 8;
            OV7670_RCK_L;
            color |= GPIOC->IDR & 0XFF; //读数据
            OV7670_RCK_H;

            //存储像素数据
            *p = color & 0x00FF;
            p++;
            *p = (color & 0xFF00) >> 8;
            p++;
            
            if (p - buff >= 640)
            {
                memcpy(USART3_TX_BUF, buff, 640);
                USART3_TX_BUF[640] = '\0';
                while (DMA_GetCurrDataCounter(DMA1_Channel2) != 0)
                    ;                                //等待通道2传输完成 ,USART2-TX 对应Channel7,USART3-TX对应Channel2
                UART_DMA_Enable(DMA1_Channel2, 640); //通过dma发送出去
                p = buff;
            }
        }
        u3_printf("hH");
        ov_sta = 0; //清零帧中断标记
        ov_frame++;
    }
}

//主函数
int main(void)
{
    u8 OV7670i = 3, OV7670res = 0;

    delay_init_O();      //外部时钟选择
    delay_init();        //初始化延迟函数
    GPIOCLKInit();       //使能所有GPIO时钟
    UserLEDInit();       //初始化用户指示灯(PC13端口)
    TIM2_Init();         //初始化定时器2
    MotorInit();         //初始化电机控制接口
    UltraSoundInit();    //初始化超声波测距模块
    RedRayInit();        //初始化循迹模块及避障模块的红外对管
    ServoInit();         //初始化3路舵机控制接口
    BrightInit();        //初始化亮度检测接口
    LightInit();         //初始化车灯接口
    KEY_Init();          //初始化按键接口
    mem_init();          //初始化内存池
    uart_init(115200);   //初始化串口1，波特率为115200，用于USMART
    USART3_Init(115200); //初始化串口3，波特率为115200，用于WIFI模块
    MPU_Init();          //初始化惯性测量模块
    mpu_dmp_init();      //初始化惯性测量模块DMP

    while (OV7670i--) //尝试3次初始化OV7670
    {
        OV7670res = OV7670_Init();
        if (OV7670res == 0)
            break; //成功
        else
            Delayms(50); //失败
    }
    EXTI8_Init();               //使能定时器捕获

    if (OV7670res == 0)
        OV7670_Window_Set(12, 176, 240, 320); //设置窗口
    OV7670_CS = 0;

    //=========================================================================
    // WIFI模块上电自动进入透传模式，如不需要修改WIFI配置，可将初始化部分注释掉
    atk_8266_wifista_Init();
    //=========================================================================

    //=======以下为主函数大循环==============================================
    while (1)
    {
        if (tick_5ms)
        {
            tick_5ms = 0;
            func_5ms();
        }
        if (tick_200ms)
        {
            tick_200ms = 0;
            func_200ms();
        }
    }
}
