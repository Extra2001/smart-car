#ifndef __MOTOR_H_
#define __MOTOR_H_

extern unsigned int speed_count; //占空比计数器 50次一周期

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
