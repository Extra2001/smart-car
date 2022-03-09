#include "SpeedCtrol.h"
#include "interface.h"

//����ֱ��66mm��������̳���Ϊ20�������ܳ� 207mm = 20.7cm
//��������жϸߵ͵�ƽ�仯����������Ҳ����˵����תһ�ܼ�������Ϊ40
//һ�������仯��ʾ�����ܹ��ľ���Ϊ 20.7/40 = 0.5175cm

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
    GPIO_InitStructure.GPIO_Pin = FRONT_RIGHT_S_PIN;  //����ʹ��GPIO�ܽ�
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     //����GPIOģʽ,��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����GPIO�˿��ٶ�
    GPIO_Init(FRONT_RIGHT_S_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = FRONT_LEFT_S_PIN;   //����ʹ��GPIO�ܽ�
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     //����GPIOģʽ,��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����GPIO�˿��ٶ�
    GPIO_Init(FRONT_LEFT_S_GPIO, &GPIO_InitStructure);
}

/*******************************************************************************
 * �� �� �� ��MeasureSpeed
 * �������� ���ٶȲ���������IO�仯�������ú�������ÿ0.1ms����һ��
 * ��    �� ����
 * ��    �� ����
 *******************************************************************************/
void MeasureSpeed(void)
{
    right_count++, left_count++;
    temp_io = FRONT_LEFT_S_IO;
    if (temp_io != left_io) //������ƽ�仯
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
    if (temp_io != right_io) //������ƽ�仯
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
