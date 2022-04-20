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

//ȫ�ֱ�������
unsigned int speed_count = 0; //ռ�ձȼ����� 50��һ����
char left_front_speed_duty = SPEED_DUTY, left_behind_speed_duty = SPEED_DUTY;
char right_front_speed_duty = SPEED_DUTY, right_behind_speed_duty = SPEED_DUTY;

unsigned char tick_5ms = 0;   // 5ms����������Ϊ�������Ļ�������
unsigned char tick_200ms = 0; //ˢ��LED��˸��ʾ

char ctrl_comm = COMM_STOP; //����ָ��

unsigned char duoji_count = 0;
unsigned char zhuanjiao = 11;

float pitch, roll, yaw;                                      //ŷ����
short aacx, aacy, aacz;                                      //���ٶȴ�����ԭʼ����
short gyrox, gyroy, gyroz;                                   //������ԭʼ����
short tempIMU;                                               //�¶�
char Bright_check = 0;                                       //���ȼ������1��������0���ڰ�
char Red_light = 0, Green_light = 0;                         //���̵�ʶ������1����ʶ��0��δʶ��
char Identification = 0;                                     //ͼ��ʶ����������
char void_right = 0, void_left = 0;                          //������Ͻ����1�����ϰ���0�����ϰ�
char Search_L = 0, Search_M = 0, Search_R = 0;               //ѭ�������1���к��ߣ�0���޺���
char LF_Light = 0, RF_Light = 0, LB_Light = 0, RB_Light = 0; //����״̬��1�����ƣ�0���ص�
char dist[5] = {0}, cnt = 0;
short tmp_dist = 0;

char Is_Display = 0; //�Ƿ�ͨ�����ڷ�����Ƶ
char key = 0;        //��������

extern u8 ov_sta;   //��exit.c���涨��
extern u8 ov_frame; //��timer.c���涨��

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
    if (key == KEY1_PRES) //����1���Ƿ�ͨ�����ڷ���ͼ��
        Is_Display = !Is_Display;
    else if (key == KEY2_PRES) //����2���Ƿ�ѭ���г�
        ToggleSearchLine();
    GetVoidData();
    GetDistanceDelay();
    BrightCheck(); //ִ�л��������ȼ��
    // atk_8266_wifista_Rece(); //�����źŽ���
    Move(); //С���˶�����
    avg_speed = (left_speed + right_speed) / 2;
}

void func_200ms(void)
{
    GetIMUData();
    LEDToggle(LED_PIN); //�û�ָʾ����˸,����������������
    camera_refresh();   //����ͷˢ��
}

void camera_refresh(void)
{
    u32 j;
    u16 color;
    char *p = buff;
    u3_printf("HHH%d", ov_sta);
    if (ov_sta) //��֡�жϸ��£�
    {
        OV7670_RRST = 0; //��ʼ��λ��ָ��
        OV7670_RCK_L;
        OV7670_RCK_H;
        OV7670_RCK_L;
        OV7670_RRST = 1; //��λ��ָ�����
        OV7670_RCK_H;
        u3_printf("Hh");
        for (j = 0; j < 76800; j++)
        {
            OV7670_RCK_L;
            color = GPIOC->IDR & 0XFF; //������
            OV7670_RCK_H;
            color <<= 8;
            OV7670_RCK_L;
            color |= GPIOC->IDR & 0XFF; //������
            OV7670_RCK_H;

            //�洢��������
            *p = color & 0x00FF;
            p++;
            *p = (color & 0xFF00) >> 8;
            p++;
            
            if (p - buff >= 640)
            {
                memcpy(USART3_TX_BUF, buff, 640);
                USART3_TX_BUF[640] = '\0';
                while (DMA_GetCurrDataCounter(DMA1_Channel2) != 0)
                    ;                                //�ȴ�ͨ��2������� ,USART2-TX ��ӦChannel7,USART3-TX��ӦChannel2
                UART_DMA_Enable(DMA1_Channel2, 640); //ͨ��dma���ͳ�ȥ
                p = buff;
            }
        }
        u3_printf("hH");
        ov_sta = 0; //����֡�жϱ��
        ov_frame++;
    }
}

//������
int main(void)
{
    u8 OV7670i = 3, OV7670res = 0;

    delay_init_O();      //�ⲿʱ��ѡ��
    delay_init();        //��ʼ���ӳٺ���
    GPIOCLKInit();       //ʹ������GPIOʱ��
    UserLEDInit();       //��ʼ���û�ָʾ��(PC13�˿�)
    TIM2_Init();         //��ʼ����ʱ��2
    MotorInit();         //��ʼ��������ƽӿ�
    UltraSoundInit();    //��ʼ�����������ģ��
    RedRayInit();        //��ʼ��ѭ��ģ�鼰����ģ��ĺ���Թ�
    ServoInit();         //��ʼ��3·������ƽӿ�
    BrightInit();        //��ʼ�����ȼ��ӿ�
    LightInit();         //��ʼ�����ƽӿ�
    KEY_Init();          //��ʼ�������ӿ�
    mem_init();          //��ʼ���ڴ��
    uart_init(115200);   //��ʼ������1��������Ϊ115200������USMART
    USART3_Init(115200); //��ʼ������3��������Ϊ115200������WIFIģ��
    MPU_Init();          //��ʼ�����Բ���ģ��
    mpu_dmp_init();      //��ʼ�����Բ���ģ��DMP

    while (OV7670i--) //����3�γ�ʼ��OV7670
    {
        OV7670res = OV7670_Init();
        if (OV7670res == 0)
            break; //�ɹ�
        else
            Delayms(50); //ʧ��
    }
    EXTI8_Init();               //ʹ�ܶ�ʱ������

    if (OV7670res == 0)
        OV7670_Window_Set(12, 176, 240, 320); //���ô���
    OV7670_CS = 0;

    //=========================================================================
    // WIFIģ���ϵ��Զ�����͸��ģʽ���粻��Ҫ�޸�WIFI���ã��ɽ���ʼ������ע�͵�
    atk_8266_wifista_Init();
    //=========================================================================

    //=======����Ϊ��������ѭ��==============================================
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