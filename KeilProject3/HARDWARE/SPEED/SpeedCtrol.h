#ifndef __SPEEDCTROL_H_
#define __SPEEDCTROL_H_

#define FOSC 48000000L //�������ã�ʹ��24M Hz 6T˫�ٱ�ģʽ

extern unsigned char left_speed;
extern unsigned char right_speed;
extern unsigned char avg_speed;

void MeasureSpeed(void);
void MeasureInit(void);
#endif
