#ifndef __BSP_USART_H__
#define __BSP_USART_H_

#include <stdio.h>
#include "usart.h"
#define TX_DATA_SIZE  128
#define RXBUFFERSIZE  7168     //最大接收字节数
extern char RxBuffer1[RXBUFFERSIZE];   //接收数据
extern __IO uint8_t aRxBuffer1;
extern __IO uint16_t Rec_Length1;
extern __IO _Bool Rec_Complete1;
int Printf(char *formatString, ...);
uint16_t DMA_Printf(char *formatString,...);
void USART_DMARec_Stop(UART_HandleTypeDef *huart);
void USART1_IDLE_Handler(void);
void USART1_REC_Handler(void);
#endif
