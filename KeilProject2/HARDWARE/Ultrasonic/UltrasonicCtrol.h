#ifndef __ULTRASONICCTROL_H_
#define __ULTRASONICCTROL_H_

extern volatile unsigned int distance_cm;//��ǰ����
extern volatile unsigned int distance_tran;//�������
void UltraSoundInit(void);
short GetDistanceDelay(void);
#endif
