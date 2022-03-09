#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
	 
//#define KEY1 PGin(0)   	
//#define KEY2 PBin(15)	 	 

#define KEY1  GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_0)//��ȡ����0
#define KEY2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)//��ȡ����1

#define KEY1_PRES	1		//KEY1  
#define KEY2_PRES	2		//KEY2 

void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8 mode);  	//����ɨ�躯��					    
#endif
