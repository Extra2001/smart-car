#ifndef __INTERFACE_H_
#define __INTERFACE_H_

#include "stm32f10x.h"

//user LED PC13
#define LED_PIN         GPIO_Pin_13
#define LED_GPIO        GPIOC
#define LED_SET         GPIO_SetBits(LED_GPIO , LED_PIN)
#define LED_RESET       GPIO_ResetBits(LED_GPIO , LED_PIN)

//���������ȼ�� PG1
#define BRIGHT_PIN      GPIO_Pin_1
#define BRIGHT_GPIO     GPIOG
#define BRIGHT_IO       GPIO_ReadInputDataBit(BRIGHT_GPIO, BRIGHT_PIN)
#define BRIGHT_Y 0 			//��������
#define BRIGHT_N 1  		//�����ڰ�

//����ң�� ���������������,�ⲿ�ж� PE1
#define IRIN_PIN         GPIO_Pin_1
#define IRIN_GPIO        GPIOE
#define IRIN_PORTSOURCE  GPIO_PortSourceGPIOE
#define IRIN_PINSOURCE   GPIO_PinSource1
#define IRIN_EXITLINE    EXTI_Line1
#define IRIN_IRQCH       EXTI1_IRQn
#define IRIN             GPIO_ReadInputDataBit(IRIN_GPIO, IRIN_PIN)

//���������� �ⲿ�жϷ��� PB14
#define Echo_PIN         GPIO_Pin_14
#define Echo_GPIO        GPIOB
#define Echo_PORTSOURCE  GPIO_PortSourceGPIOB
#define Echo_PINSOURCE   GPIO_PinSource14
#define Echo_EXITLINE    EXTI_Line14
#define Echo_IRQCH       EXTI15_10_IRQn
#define Echo             GPIO_ReadInputDataBit(Echo_GPIO, Echo_PIN)
//����IO PD8
#define Trig_PIN         GPIO_Pin_8
#define Trig_GPIO        GPIOD
#define Trig_RESET       GPIO_SetBits(Trig_GPIO , Trig_PIN)
#define Trig_SET         GPIO_ResetBits(Trig_GPIO , Trig_PIN)//5ת3.3����һ�η��� modfied by LC 2015.09.20 10:18

//�������������IO PG3
#define Servo_PIN         GPIO_Pin_3
#define Servo_GPIO        GPIOG
#define Servo_SET         GPIO_SetBits(Servo_GPIO , Servo_PIN)
#define Servo_RESET       GPIO_ResetBits(Servo_GPIO , Servo_PIN)

// //����ͷ���¶������IO PF10
// #define CCD_UD_Servo_PIN         GPIO_Pin_10
// #define CCD_UD_Servo_GPIO        GPIOF
// #define CCD_UD_Servo_SET         GPIO_SetBits(CCD_UD_Servo_GPIO , CCD_UD_Servo_PIN)
// #define CCD_UD_Servo_RESET       GPIO_ResetBits(CCD_UD_Servo_GPIO , CCD_UD_Servo_PIN)

// //����ͷ���Ҷ������IO PC9
// #define CCD_LR_Servo_PIN         GPIO_Pin_9
// #define CCD_LR_Servo_GPIO        GPIOC
// #define CCD_LR_Servo_SET         GPIO_SetBits(CCD_LR_Servo_GPIO , CCD_LR_Servo_PIN)
// #define CCD_LR_Servo_RESET       GPIO_ResetBits(CCD_LR_Servo_GPIO , CCD_LR_Servo_PIN)

/*
//mini12864
//LCDCS PC10   
#define LCDCS_PIN         GPIO_Pin_10
#define LCDCS_GPIO        GPIOC
#define LCDCS_SET         GPIO_SetBits(LCDCS_GPIO , LCDCS_PIN)
#define LCDCS_RESET       GPIO_ResetBits(LCDCS_GPIO , LCDCS_PIN)
//LCDRST PC12
#define LCDRST_PIN         GPIO_Pin_12
#define LCDRST_GPIO        GPIOC
#define LCDRST_SET         GPIO_SetBits(LCDRST_GPIO , LCDRST_PIN)
#define LCDRST_RESET       GPIO_ResetBits(LCDRST_GPIO , LCDRST_PIN)
//LCDRS	PD1
#define LCDRS_PIN         GPIO_Pin_1
#define LCDRS_GPIO        GPIOD
#define LCDRS_SET         GPIO_SetBits(LCDRS_GPIO , LCDRS_PIN)
#define LCDRS_RESET       GPIO_ResetBits(LCDRS_GPIO , LCDRS_PIN)
//LCDSCL PD3
#define LCDSCL_PIN         GPIO_Pin_3
#define LCDSCL_GPIO        GPIOD
#define LCDSCL_SET         GPIO_SetBits(LCDSCL_GPIO , LCDSCL_PIN)
#define LCDSCL_RESET       GPIO_ResetBits(LCDSCL_GPIO , LCDSCL_PIN)
//LCDSID PD5
#define LCDSID_PIN         GPIO_Pin_5
#define LCDSID_GPIO        GPIOD
#define LCDSID_SET         GPIO_SetBits(LCDSID_GPIO , LCDSID_PIN)
#define LCDSID_RESET       GPIO_ResetBits(LCDSID_GPIO , LCDSID_PIN)
*/

//OLED0.96'Һ����ʾ������
//LCDCS PE9   
#define LCD_CS_PIN        GPIO_Pin_9
#define LCD_CS_PORT       GPIOE
#define LCD_CS_SET        GPIO_SetBits(LCD_CS_PORT , LCD_CS_PIN)
#define LCD_CS_RESET      GPIO_ResetBits(LCD_CS_PORT , LCD_CS_PIN)
//LCDRST PE8
#define LCD_RST_PIN       GPIO_Pin_8
#define LCD_RST_PORT      GPIOE
#define LCD_RST_SET       GPIO_SetBits(LCD_RST_PORT , LCD_RST_PIN)
#define LCD_RST_RESET     GPIO_ResetBits(LCD_RST_PORT , LCD_RST_PIN)
//LCDA0	PE7
#define LCD_DC_PIN        GPIO_Pin_7
#define LCD_DC_PORT       GPIOE
#define LCD_DC_SET        GPIO_SetBits(LCD_DC_PORT , LCD_DC_PIN)
#define LCD_DC_RESET      GPIO_ResetBits(LCD_DC_PORT , LCD_DC_PIN)
//LCDSCK PA5
#define LCD_SCK_PIN       GPIO_Pin_5
#define LCD_SCK_GPIO      GPIOA
#define LCD_SCK_SET       GPIO_SetBits(LCD_SCK_GPIO , LCD_SCK_PIN)
#define LCD_SCK_RESET     GPIO_ResetBits(LCD_SCK_GPIO , LCD_SCK_PIN)
//LCDMOSI PA7
#define LCD_MOSI_PIN      GPIO_Pin_7
#define LCD_MOSI_GPIO     GPIOA
#define LCD_MOSI_SET      GPIO_SetBits(LCD_MOSI_GPIO , LCD_MOSI_PIN)
#define LCD_MOSI_RESET    GPIO_ResetBits(LCD_MOSI_GPIO , LCD_MOSI_PIN)

//���ƶ���LIGHT
//LF PB13   
#define LF_Light_PIN         GPIO_Pin_13
#define LF_Light_GPIO        GPIOB
#define LF_Light_SET         GPIO_SetBits(LF_Light_GPIO , LF_Light_PIN)
#define LF_Light_RESET       GPIO_ResetBits(LF_Light_GPIO , LF_Light_PIN)
//RF PE11
#define RF_Light_PIN         GPIO_Pin_11
#define RF_Light_GPIO        GPIOE
#define RF_Light_SET         GPIO_SetBits(RF_Light_GPIO , RF_Light_PIN)
#define RF_Light_RESET       GPIO_ResetBits(RF_Light_GPIO , RF_Light_PIN)
//LB PG11
#define LB_Light_PIN         GPIO_Pin_11
#define LB_Light_GPIO        GPIOG
#define LB_Light_SET         GPIO_SetBits(LB_Light_GPIO , LB_Light_PIN)
#define LB_Light_RESET       GPIO_ResetBits(LB_Light_GPIO , LB_Light_PIN)
//RB PE0
#define RB_Light_PIN         GPIO_Pin_0
#define RB_Light_GPIO        GPIOE
#define RB_Light_SET         GPIO_SetBits(RB_Light_GPIO , RB_Light_PIN)
#define RB_Light_RESET       GPIO_ResetBits(RB_Light_GPIO , RB_Light_PIN)

//�������IO���� 
/* 
FRONT_LEFT_F_PIN	PD13	��ǰǰ��IO
FRONT_LEFT_B_PIN	PD11	��ǰ����IO

FRONT_RIGHT_F_PIN	PE13	��ǰǰ��IO
FRONT_RIGHT_B_PIN	PE15	    ��ǰ����IO

BEHIND_LEFT_F_PIN	PD9	    ���ǰ��IO
BEHIND_LEFT_B_PIN	PD10	    ������IO

BEHIND_RIGHT_F_PIN	PE14	    �Һ�ǰ��IO
BEHIND_RIGHT_B_PIN	PE12	    �Һ����IO
 */
#define FRONT_LEFT_F_PIN         GPIO_Pin_13
#define FRONT_LEFT_F_GPIO        GPIOD
#define FRONT_LEFT_F_SET         GPIO_SetBits(FRONT_LEFT_F_GPIO , FRONT_LEFT_F_PIN)
#define FRONT_LEFT_F_RESET       GPIO_ResetBits(FRONT_LEFT_F_GPIO , FRONT_LEFT_F_PIN)

#define FRONT_LEFT_B_PIN         GPIO_Pin_11
#define FRONT_LEFT_B_GPIO        GPIOD
#define FRONT_LEFT_B_SET         GPIO_SetBits(FRONT_LEFT_B_GPIO , FRONT_LEFT_B_PIN)
#define FRONT_LEFT_B_RESET       GPIO_ResetBits(FRONT_LEFT_B_GPIO , FRONT_LEFT_B_PIN)

#define FRONT_RIGHT_F_PIN         GPIO_Pin_13
#define FRONT_RIGHT_F_GPIO        GPIOE
#define FRONT_RIGHT_F_SET         GPIO_SetBits(FRONT_RIGHT_F_GPIO , FRONT_RIGHT_F_PIN)
#define FRONT_RIGHT_F_RESET       GPIO_ResetBits(FRONT_RIGHT_F_GPIO , FRONT_RIGHT_F_PIN)

#define FRONT_RIGHT_B_PIN         GPIO_Pin_15
#define FRONT_RIGHT_B_GPIO        GPIOE
#define FRONT_RIGHT_B_SET         GPIO_SetBits(FRONT_RIGHT_B_GPIO , FRONT_RIGHT_B_PIN)
#define FRONT_RIGHT_B_RESET       GPIO_ResetBits(FRONT_RIGHT_B_GPIO , FRONT_RIGHT_B_PIN)

#define BEHIND_LEFT_F_PIN         GPIO_Pin_9
#define BEHIND_LEFT_F_GPIO        GPIOD
#define BEHIND_LEFT_F_SET         GPIO_SetBits(BEHIND_LEFT_F_GPIO , BEHIND_LEFT_F_PIN)
#define BEHIND_LEFT_F_RESET       GPIO_ResetBits(BEHIND_LEFT_F_GPIO , BEHIND_LEFT_F_PIN)

#define BEHIND_LEFT_B_PIN         GPIO_Pin_10
#define BEHIND_LEFT_B_GPIO        GPIOD
#define BEHIND_LEFT_B_SET         GPIO_SetBits(BEHIND_LEFT_B_GPIO , BEHIND_LEFT_B_PIN)
#define BEHIND_LEFT_B_RESET       GPIO_ResetBits(BEHIND_LEFT_B_GPIO , BEHIND_LEFT_B_PIN)

#define BEHIND_RIGHT_F_PIN         GPIO_Pin_14
#define BEHIND_RIGHT_F_GPIO        GPIOE
#define BEHIND_RIGHT_F_SET         GPIO_SetBits(BEHIND_RIGHT_F_GPIO , BEHIND_RIGHT_F_PIN)
#define BEHIND_RIGHT_F_RESET       GPIO_ResetBits(BEHIND_RIGHT_F_GPIO , BEHIND_RIGHT_F_PIN)

#define BEHIND_RIGHT_B_PIN         GPIO_Pin_12
#define BEHIND_RIGHT_B_GPIO        GPIOE
#define BEHIND_RIGHT_B_SET         GPIO_SetBits(BEHIND_RIGHT_B_GPIO , BEHIND_RIGHT_B_PIN)
#define BEHIND_RIGHT_B_RESET       GPIO_ResetBits(BEHIND_RIGHT_B_GPIO , BEHIND_RIGHT_B_PIN)

//ѭ�����Թܶ���
/* 
��ѭ��	SEARCH_L_PIN	PG4
��ѭ��	SEARCH_M_PIN	PG6
��ѭ��	SEARCH_R_PIN	PG8
 */
#define SEARCH_L_PIN         GPIO_Pin_4
#define SEARCH_L_GPIO        GPIOG
#define SEARCH_L_IO          GPIO_ReadInputDataBit(SEARCH_L_GPIO, SEARCH_L_PIN)

#define SEARCH_M_PIN         GPIO_Pin_6
#define SEARCH_M_GPIO        GPIOG
#define SEARCH_M_IO          GPIO_ReadInputDataBit(SEARCH_M_GPIO, SEARCH_M_PIN)

#define SEARCH_R_PIN         GPIO_Pin_8
#define SEARCH_R_GPIO        GPIOG
#define SEARCH_R_IO          GPIO_ReadInputDataBit(SEARCH_R_GPIO, SEARCH_R_PIN)

#define BLACK_AREA 1
#define WHITE_AREA 0

//������϶���
/* 
�����	VOID_L_PIN	PG2
�ұ���	VOID_R_PIN	PE10
 */
#define VOID_L_PIN         GPIO_Pin_2
#define VOID_L_GPIO        GPIOG
#define VOID_L_IO          GPIO_ReadInputDataBit(VOID_L_GPIO, VOID_L_PIN)

#define VOID_R_PIN         GPIO_Pin_10
#define VOID_R_GPIO        GPIOE
#define VOID_R_IO          GPIO_ReadInputDataBit(VOID_R_GPIO, VOID_R_PIN)

#define BARRIER_Y 0 			//���ϰ���
#define BARRIER_N 1  			//���ϰ���

//����ģ�鶨��
/* 
�ٶ�������	FRONT_RIGHT_S_PIN	PA11
�ٶ�������	FRONT_LEFT_S_PIN	PA12
 */
#define FRONT_RIGHT_S_PIN         GPIO_Pin_11
#define FRONT_RIGHT_S_GPIO        GPIOA
#define FRONT_RIGHT_S_IO          GPIO_ReadInputDataBit(FRONT_RIGHT_S_GPIO, FRONT_RIGHT_S_PIN)

#define FRONT_LEFT_S_PIN         GPIO_Pin_12
#define FRONT_LEFT_S_GPIO        GPIOA
#define FRONT_LEFT_S_IO          GPIO_ReadInputDataBit(FRONT_LEFT_S_GPIO, FRONT_LEFT_S_PIN)

//��ǰ
#define FRONT_LEFT_GO    FRONT_LEFT_F_SET; FRONT_LEFT_B_RESET		//ǰ��
#define FRONT_LEFT_BACK  FRONT_LEFT_F_RESET; FRONT_LEFT_B_SET		//����
#define FRONT_LEFT_STOP  FRONT_LEFT_F_RESET; FRONT_LEFT_B_RESET	//ֹͣ

//��ǰ
#define FRONT_RIGHT_GO     FRONT_RIGHT_F_SET;  FRONT_RIGHT_B_RESET
#define FRONT_RIGHT_BACK   FRONT_RIGHT_F_RESET;FRONT_RIGHT_B_SET
#define FRONT_RIGHT_STOP   FRONT_RIGHT_F_RESET;FRONT_RIGHT_B_RESET

//���
#define BEHIND_LEFT_GO     BEHIND_LEFT_F_SET;BEHIND_LEFT_B_RESET
#define BEHIND_LEFT_BACK   BEHIND_LEFT_F_RESET;BEHIND_LEFT_B_SET
#define BEHIND_LEFT_STOP   BEHIND_LEFT_F_RESET;BEHIND_LEFT_B_RESET

//�Һ�
#define BEHIND_RIGHT_GO    BEHIND_RIGHT_F_SET;BEHIND_RIGHT_B_RESET
#define BEHIND_RIGHT_BACK  BEHIND_RIGHT_F_RESET;BEHIND_RIGHT_B_SET
#define BEHIND_RIGHT_STOP  BEHIND_RIGHT_F_RESET;BEHIND_RIGHT_B_RESET

//����ռ�ձȶ���
#define SPEED_DUTY 50	//Ĭ��ռ�ձ� ��1ms��С�ֱ��� ����50ms����

//ָ���
#define COMM_STOP  'e'//ֹͣ
#define COMM_UP    'a'//ǰ��
#define COMM_DOWN  'b'//����
#define COMM_LEFT  'c'//��ת
#define COMM_RIGHT 'd'//��ת

#define CCD_UP  		'f'//CCD��
#define CCD_DOWN   	'g'//CCD��
#define CCD_LEFT  	'i'//CCD��
#define CCD_RIGHT  	'k'//CCD��
#define CCD_CENTER 	'm'//CCD����

extern unsigned char tick_0_1ms;	//0.1ms����������Ϊ����Ļ���������
extern unsigned char tick_5ms;		//5ms����������Ϊ�������Ļ�������
extern unsigned char tick_200ms;	//200ms����������Ϊ�˶��滮�Ļ���������
extern unsigned int speed_count;	//ռ�ձȼ����� 50��һ����
extern unsigned char bt_rec_flag;	//����/�������Ʊ�־
extern unsigned char HalfSpeed;		//�����г���־

void delay_init_O(void);
void Delayms(u32 Nms);
void Delay_us(u32 Nus);

void IRIN_Configuration(void);
void TIM2_Init(void);
void RedRayInit(void);

void ServoInit(void);
void GPIOCLKInit(void);

#endif

