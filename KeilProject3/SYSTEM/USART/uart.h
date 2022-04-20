#ifndef __UART_H_
#define __UART_H_

#include "stm32f10x.h"

void USART1Conf(u32 baudRate);
void USART1_PutChar(u8 Data);
void USART1_PutNChar(u8 *buf , u16 size);
void USART1_PutStr(char *str);
void USART3Conf(u32 baudRate);
void USART3_PutChar(u8 Data);
void USART3_PutNChar(u8 *buf , u16 size);
void USART3_PutStr(char *str);
#endif
