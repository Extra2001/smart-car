#ifndef __MOTOR_H_
#define __MOTOR_H_

extern unsigned int speed_count; //ռ�ձȼ����� 50��һ����

extern char left_front_speed_duty;
extern char left_behind_speed_duty;
extern char right_front_speed_duty;
extern char right_behind_speed_duty;

void MotorGPIO_Configuration(void);
void SetSpeedDutyAll(char);
void SetSpeedDutySide(char, char);
void SetSpeedDuty(char, char, char, char);
void CarMove(void);
void MotorInit(void);
#endif
