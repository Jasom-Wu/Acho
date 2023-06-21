#include "bsp_usart.h"
#include "usart.h"


_Bool Uart_Error_Flag =0;
__IO uint8_t USART_FLAG = 1;//1表示串口1输出输入模式，2表示串口2输出输入模式，依此推类。

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		if(USART_FLAG ==1)
		{
			HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
			while(!(USART1->SR & UART_FLAG_TXE));
		}
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
	uint8_t temp;
	if(USART_FLAG ==1)
	{
		HAL_UART_Receive(&huart1, &temp, 1, 0xffff);
	}
	return temp;
}
 
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1)
	{
		;
	}
}

 /**************************************************************************
  * @brief  串口错误回调函数
  * @retval 
**************************************************************************/
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if((__HAL_UART_GET_FLAG(huart, UART_FLAG_PE))!=RESET)
    {
   // HAL_UART_Transmit_DMA(&huart1, (uint8_t *)"00000", 5);
   __HAL_UNLOCK(huart);
   __HAL_UART_CLEAR_PEFLAG(huart);
    }
    if((__HAL_UART_GET_FLAG(huart, UART_FLAG_FE))!=RESET)
    {
   // HAL_UART_Transmit_DMA(&huart1, (uint8_t *)"00001", 5);
   __HAL_UNLOCK(huart);
   __HAL_UART_CLEAR_FEFLAG(huart);
    }
    
    if((__HAL_UART_GET_FLAG(huart, UART_FLAG_NE))!=RESET)
    {
   // HAL_UART_Transmit_DMA(&huart1, (uint8_t *)"00002", 5);
   __HAL_UNLOCK(huart);
   __HAL_UART_CLEAR_NEFLAG(huart);
    }       
    
    if((__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE))!=RESET)
    {
   // HAL_UART_Transmit_DMA(&huart1, (uint8_t *)"00003", 5);
   __HAL_UNLOCK(huart);
   __HAL_UART_CLEAR_OREFLAG(huart);
    }    
 
 // 错误标志位置 
 Uart_Error_Flag = 1;
}
