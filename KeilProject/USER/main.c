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

//全局变量定义
unsigned int speed_count = 0; //占空比计数器 50次一周期
char left_speed_duty = SPEED_DUTY;
char right_speed_duty = SPEED_DUTY;
char turn_speed_duty = 0;

unsigned char tick_5ms = 0;   // 5ms计数器，作为主函数的基本周期
unsigned char tick_200ms = 0; //刷新LED闪烁显示

unsigned char HalfSpeed = 0;

char ctrl_comm = COMM_STOP;    //控制指令
char WF_comm = COMM_STOP;      //控制指令
unsigned char wf_rec_flag = 0; // WIFI上位机控制标志位

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

char Is_Display = 0; //是否通过串口发送视频
char key = 0;        //按键输入

extern u8 ov_sta; //在exit.c里面定义

void GetIMUData(void)
{
    tempIMU = MPU_Get_Temperature();
    MPU_Get_Gyroscope(&gyrox, &gyroy, &gyroz);
    MPU_Get_Accelerometer(&aacx, &aacy, &aacz);
}

//控制信号接收
void SignalReceive(void)
{
    atk_8266_wifista_Rece();
    if (wf_rec_flag == 1) //接收到WIFI信号
    {
        wf_rec_flag = 0;
        switch (WF_comm)
        {
        case COMM_UP:
            ctrl_comm = COMM_UP;
            break; //前进指令
        case COMM_DOWN:
            ctrl_comm = COMM_DOWN;
            break; //后退指令
        case COMM_LEFT:
            ctrl_comm = COMM_LEFT;
            break; //左转指令
        case COMM_RIGHT:
            ctrl_comm = COMM_RIGHT;
            break; //右转指令
        case COMM_STOP:
            ctrl_comm = COMM_STOP;
            break; //停车指令
        default:
            break;
        }
    }
}

void func_5ms(void)
{
    key = KEY_Scan(0);
    if (key == KEY1_PRES) //按键1：是否通过串口发送图像
        Is_Display = !Is_Display;
    else if (key == KEY2_PRES) //按键2：是否循迹行车
        ToggleSearchLine();

    avg_speed = (left_speed + right_speed) / 2;
    BrightCheck();   //执行环境光亮度检测
    SignalReceive(); //控制信号接收
    Move();          //小车运动控制
}

void func_200ms(void)
{
    GetIMUData();
    LEDToggle(LED_PIN);      //用户指示灯闪烁,表明程序运行正常
    atk_8266_wifista_Tran(); // wifi发送数据至上位机 中速
}

//主函数
int main(void)
{
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
    EXTI8_Init();        //使能定时器捕获

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
