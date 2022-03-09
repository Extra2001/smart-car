#ifndef __LED_H_
#define __LED_H_

#include <stdint.h>

extern char LF_Light;
extern char RF_Light;
extern char LB_Light;
extern char RB_Light;

extern char Bright_check;

void LEDToggle(uint16_t Led);
void LightInit(void);
void LightOpen(void);
void LightClose(void);
void UserLEDInit(void);
void BrightInit(void);
void BrightCheck(void);

#endif
