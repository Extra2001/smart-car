#ifndef __MOTOR_H_
#define __MOTOR_H_

extern unsigned int speed_count; //ռ�ձȼ����� 50��һ����

extern char left_speed_duty;
extern char right_speed_duty;

void MotorGPIO_Configuration(void);
void CarMove(void);
void CarGo(void);
void CarBack(void);
void CarLeft(void);
void CarRight(void);
void CarStop(void);
void MotorInit(void);
#endif
