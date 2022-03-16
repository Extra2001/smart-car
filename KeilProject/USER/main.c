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

//ȫ�ֱ�������
unsigned int speed_count = 0; //ռ�ձȼ����� 50��һ����
char left_speed_duty = SPEED_DUTY;
char right_speed_duty = SPEED_DUTY;
char turn_speed_duty = 0;

unsigned char tick_5ms = 0;   // 5ms����������Ϊ�������Ļ�������
unsigned char tick_200ms = 0; //ˢ��LED��˸��ʾ

unsigned char HalfSpeed = 0;

char ctrl_comm = COMM_STOP;    //����ָ��
char WF_comm = COMM_STOP;      //����ָ��
unsigned char wf_rec_flag = 0; // WIFI��λ�����Ʊ�־λ

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

char Is_Display = 0; //�Ƿ�ͨ�����ڷ�����Ƶ
char key = 0;        //��������

extern u8 ov_sta; //��exit.c���涨��

void GetIMUData(void)
{
    tempIMU = MPU_Get_Temperature();
    MPU_Get_Gyroscope(&gyrox, &gyroy, &gyroz);
    MPU_Get_Accelerometer(&aacx, &aacy, &aacz);
}

//�����źŽ���
void SignalReceive(void)
{
    atk_8266_wifista_Rece();
    if (wf_rec_flag == 1) //���յ�WIFI�ź�
    {
        wf_rec_flag = 0;
        switch (WF_comm)
        {
        case COMM_UP:
            ctrl_comm = COMM_UP;
            break; //ǰ��ָ��
        case COMM_DOWN:
            ctrl_comm = COMM_DOWN;
            break; //����ָ��
        case COMM_LEFT:
            ctrl_comm = COMM_LEFT;
            break; //��תָ��
        case COMM_RIGHT:
            ctrl_comm = COMM_RIGHT;
            break; //��תָ��
        case COMM_STOP:
            ctrl_comm = COMM_STOP;
            break; //ͣ��ָ��
        default:
            break;
        }
    }
}

void func_5ms(void)
{
    key = KEY_Scan(0);
    if (key == KEY1_PRES) //����1���Ƿ�ͨ�����ڷ���ͼ��
        Is_Display = !Is_Display;
    else if (key == KEY2_PRES) //����2���Ƿ�ѭ���г�
        ToggleSearchLine();

    avg_speed = (left_speed + right_speed) / 2;
    BrightCheck();   //ִ�л��������ȼ��
    SignalReceive(); //�����źŽ���
    Move();          //С���˶�����
}

void func_200ms(void)
{
    GetIMUData();
    LEDToggle(LED_PIN);      //�û�ָʾ����˸,����������������
    atk_8266_wifista_Tran(); // wifi������������λ�� ����
}

//������
int main(void)
{
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
    EXTI8_Init();        //ʹ�ܶ�ʱ������

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
