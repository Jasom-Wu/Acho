#include <string.h>
#include "bsp_usart.h"
#include "stdarg.h"


_Bool Uart_Error_Flag = 0;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
char RxBuffer1[RXBUFFERSIZE];   //接收数据
__IO uint16_t Rec_Length1 = 0;
__IO _Bool Rec_Complete1 = 0;


///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f) {
    HAL_UART_Transmit(&huart1, (uint8_t *) &ch, 1, 0xffff);
    while (!(USART1->SR & UART_FLAG_TXE));
    return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f) {
    /* 等待串口输入数据 */
    uint8_t temp;
    HAL_UART_Receive(&huart1, &temp, 1, 0xffff);
    return temp;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart1) { ;
    }
}

/**************************************************************************
 * @brief  串口错误回调函数
 * @retval
**************************************************************************/
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
    if ((__HAL_UART_GET_FLAG(huart, UART_FLAG_PE)) != RESET) {
        // HAL_UART_Transmit_DMA(&huart1, (uint8_t *)"00000", 5);
        __HAL_UNLOCK(huart);
        __HAL_UART_CLEAR_PEFLAG(huart);
    }
    if ((__HAL_UART_GET_FLAG(huart, UART_FLAG_FE)) != RESET) {
        // HAL_UART_Transmit_DMA(&huart1, (uint8_t *)"00001", 5);
        __HAL_UNLOCK(huart);
        __HAL_UART_CLEAR_FEFLAG(huart);
    }

    if ((__HAL_UART_GET_FLAG(huart, UART_FLAG_NE)) != RESET) {
        // HAL_UART_Transmit_DMA(&huart1, (uint8_t *)"00002", 5);
        __HAL_UNLOCK(huart);
        __HAL_UART_CLEAR_NEFLAG(huart);
    }

    if ((__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE)) != RESET) {
        // HAL_UART_Transmit_DMA(&huart1, (uint8_t *)"00003", 5);
        __HAL_UNLOCK(huart);
        __HAL_UART_CLEAR_OREFLAG(huart);
    }

    // 错误标志位置
    Uart_Error_Flag = 1;
}

int Printf(char *formatString, ...) {
    char TXBuffer[TX_DATA_SIZE];
    int length;
    va_list args;
    va_start(args, formatString);
    length = vsnprintf(TXBuffer, TX_DATA_SIZE, formatString, args);
    va_end(args);
    HAL_UART_Transmit(&huart1, (const uint8_t *) TXBuffer, length, 1000);
    return length;
}

uint16_t DMA_Printf(char *formatString, ...) {
    char TXBuffer[TX_DATA_SIZE];
    uint16_t length;
    va_list args;
    va_start(args, formatString);
    length = vsnprintf(TXBuffer, TX_DATA_SIZE, formatString, args);
    while (HAL_DMA_GetState(&hdma_usart1_tx) == HAL_DMA_STATE_BUSY);
    if (huart1.gState != HAL_UART_STATE_READY)HAL_UART_Abort(&huart1);
    __HAL_DMA_DISABLE(&hdma_usart1_tx);
    if (HAL_UART_Transmit_DMA(&huart1, (uint8_t *) TXBuffer, length) != HAL_OK) //判断是否发送正常，如果出现异常则进入异常中断函数
    {
        //return;
    }
    while (__HAL_DMA_GET_COUNTER(&hdma_usart1_tx) != 0);//等发送完才能清
    va_end(args);
    return length;
}

void USART_DMARec_Stop(UART_HandleTypeDef *huart) {
    uint32_t dmarequest = 0x00U;
    /* Stop UART DMA Rx request if ongoing */
    dmarequest = HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR);
    if ((huart->RxState == HAL_UART_STATE_BUSY_RX) && dmarequest) {
        CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);

        /* Abort the UART DMA Rx channel */
        if (huart->hdmarx != NULL) {
            HAL_DMA_Abort(huart->hdmarx);
        }
        /* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
        CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));
        CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);

        /* In case of reception waiting for IDLE event, disable also the IDLE IE interrupt source */
        if (huart->ReceptionType == HAL_UART_RECEPTION_TOIDLE) {
            CLEAR_BIT(huart->Instance->CR1, USART_CR1_IDLEIE);
        }

        /* At end of Rx process, restore huart->RxState to Ready */
        huart->RxState = HAL_UART_STATE_READY;
        huart->ReceptionType = HAL_UART_RECEPTION_STANDARD;
    }
}

#define USART_IDLE_FLAG    (hdma_usart1_tx.State!=HAL_DMA_STATE_BUSY && hdma_usart2_tx.State!=HAL_DMA_STATE_BUSY)
#define USART_IDLE_OK 1

void USART1_IDLE_Handler(void) {
    uint16_t temp;
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) != RESET) {
        if (hdma_usart1_tx.State != HAL_DMA_STATE_BUSY && Rec_Complete1 == 0)
            //检测DMA发送是否完成，不加这句的话，当数据流进空闲中断太频繁，会导致DMA提前结束以致发送中止卡住
        {
            USART_DMARec_Stop(&huart1);
            temp = __HAL_DMA_GET_COUNTER(
                    &hdma_usart1_rx);// 获取DMA中未传输的数据个数   //temp  = hdma_usart1_rx.Instance->NDTR;//读取NDTR寄存器 获取DMA中未传输的数据个数，
            Rec_Length1 = RXBUFFERSIZE - temp; //总计数减去未传输的数据个数，得到已经接收的数据个数
            Rec_Complete1 = 1;    // 接受完成标志位置1
        }
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);//清除标志位要放出来！！不然无法清除标志位导致一直在中断handler死循环
    }
}


static struct UartHandler_DriverStruct *
USART_HandlerMount(UartHandle_FuncTypedef handler_func, UartHandle_FinishCallBackFuncTypedef callback_func,
                   void *handler_userdata, void *callback_userdata) {
    if (handler_func == NULL)
        return &HandlerManager;
    while (!HandlerManager.is_idle);
    HandlerManager.handler_function = handler_func;
    HandlerManager.callback_func = callback_func;
    HandlerManager.handler_userdata = handler_userdata;
    HandlerManager.callback_userdata = callback_userdata;
    return &HandlerManager;
}

static void USART_HandlerRun(uint16_t timeout_ms, uint8_t need_reset) {
    if (HandlerManager.handler_function == NULL)return;
    HandlerManager.timeout_ms = timeout_ms;
    HandlerManager.need_reset = need_reset;
    HandlerManager.is_idle = 0;
}

static void USART_ManagerReset(void) {
    HandlerManager.is_idle = 1;
    HandlerManager.handler_function = NULL;
    HandlerManager.callback_func = NULL;
    HandlerManager.handler_userdata = NULL;
    HandlerManager.callback_userdata = NULL;
    HandlerManager.timeout_ms = 1000;
    HandlerManager.need_reset = 1;
}

UartHandler_DriverTypedef HandlerManager = {
        .handler_function = NULL,
        .callback_func = NULL,
        .mount = USART_HandlerMount,
        .run = USART_HandlerRun,
        .reset = USART_ManagerReset,
        .handler_userdata = NULL,
        .callback_userdata = NULL,
        .timeout_ms = 1000,
        .is_idle = 1,
        .need_reset = 1,
};

void USART1_REC_Handler(void) {
    static uint32_t time_begin = 0;
    static uint8_t flag = 0;
    if (Rec_Complete1 == 1)  //接收完成标志
    {
        __HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE);
        //======USART1_LOGIC_BEGIN======
//        if (HAL_UART_Transmit_DMA(&huart1, (uint8_t *) RxBuffer1, Rec_Length1) != HAL_OK) //判断是否发送正常，如果出现异常则进入异常中断函数
//        {
//            //return;
//        }
//        while (__HAL_DMA_GET_COUNTER(&hdma_usart1_tx) != 0);//等发送完才能清
        if (!HandlerManager.is_idle && HandlerManager.handler_function != NULL) {
            HandlerManager.handler_function(RxBuffer1, Rec_Length1, HandlerManager.handler_userdata);
            HandlerManager.is_idle = 1;//successful -> set is_idel to 1 !
        }
        //======USART1_LOGIC_END======
        memset(RxBuffer1, 0, Rec_Length1);
        Rec_Length1 = 0;//清除计数
        Rec_Complete1 = 0;//清除接收结束标志位
        HAL_UART_Receive_DMA(&huart1, (uint8_t *) RxBuffer1, RXBUFFERSIZE);//重新打开DMA接收
        __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    }
    switch (flag) {
        case 0: {
            if (!HandlerManager.is_idle) {
                time_begin = HAL_GetTick();
                flag = 1;
            }
            break;
        }
        case 1: {
            if (HandlerManager.is_idle || HAL_GetTick() - time_begin >= HandlerManager.timeout_ms)
                flag = 2;
            break;
        }
        case 2: {
            if (HandlerManager.callback_func) {
                if (HandlerManager.is_idle) {
                    HandlerManager.callback_func(HAL_OK, HandlerManager.callback_userdata);
                } else {
                    HandlerManager.callback_func(HAL_TIMEOUT, HandlerManager.callback_userdata);
                }
                if (HandlerManager.need_reset)HandlerManager.reset();
            }
            flag = 0;
            break;
        }
        default:
            flag = 0;
            break;
    }
}