#ifndef __ULTRASONICCTROL_H_
#define __ULTRASONICCTROL_H_

extern volatile unsigned int distance_cm;//µ±«∞æ‡¿Î
extern volatile unsigned int distance_tran;//¥´ ‰æ‡¿Î
void UltraSoundInit(void);
short GetDistanceDelay(void);
#endif
