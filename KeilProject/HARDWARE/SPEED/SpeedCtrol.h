#ifndef __SPEEDCTROL_H_
#define __SPEEDCTROL_H_

#define FOSC 48000000L //晶振设置，使用24M Hz 6T双速倍模式

extern unsigned char left_speed;
extern unsigned char right_speed;
extern unsigned char avg_speed;

void MeasureSpeed(void);
void MeasureInit(void);
#endif
