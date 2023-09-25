#ifndef __BSP_USART_H__
#define __BSP_USART_H_

#include <stdio.h>
#include "usart.h"
#define TX_DATA_SIZE  128
#define RXBUFFERSIZE  7168     //最大接收字节数
typedef void (*UartHandle_FuncTypedef)(char *payload, uint16_t size,void*handler_userdata);
typedef void (*UartHandle_FinishCallBackFuncTypedef)(HAL_StatusTypeDef state,void *callback_userdata);
typedef struct UartHandler_DriverStruct{
    UartHandle_FuncTypedef handler_function;
    UartHandle_FinishCallBackFuncTypedef callback_func;
    struct UartHandler_DriverStruct *(*mount)(UartHandle_FuncTypedef handler_func,
                                              UartHandle_FinishCallBackFuncTypedef callback_func,
                                              void *handler_userdata, void *callback_userdata);
    void (*run)(uint16_t timeout_ms,uint8_t need_reset);
    void (*reset)(void);
    void * handler_userdata;
    void * callback_userdata;
    uint16_t timeout_ms;
    uint8_t is_idle: 1;
    uint8_t need_reset: 1;
} UartHandler_DriverTypedef;


extern UartHandler_DriverTypedef HandlerManager;
extern char RxBuffer1[RXBUFFERSIZE];
extern __IO uint16_t Rec_Length1;
extern __IO _Bool Rec_Complete1;
int Printf(char *formatString, ...);
uint16_t DMA_Printf(char *formatString,...);
void USART_DMARec_Stop(UART_HandleTypeDef *huart);
void USART1_IDLE_Handler(void);
void USART1_REC_Handler(void);
#endif
