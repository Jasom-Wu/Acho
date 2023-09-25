/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_fs.h"
#include "lv_port_indev.h"
#include "EPD_1in54_V2.h"
#include "GUI_Paint.h"
#include "bsp_ink_paper.h"
#include "bsp_key.h"
#include "usbd_def.h"
#include "usbd_core.h"
#include "sdio.h"
#include "ff.h"
#include "fatfs.h"
#include "audio.h"
#include "bsp_vs10xx.h"
#include "page_test.h"
#include "gui_setup.h"
#include "fops.h"
#include "bsp_usart.h"
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
uint32_t defaultTaskBuffer[ 1024 ];
osStaticThreadDef_t defaultTaskControlBlock;
osThreadId mainTaskHandle;
uint32_t mainTaskBuffer[ 1024 ];
osStaticThreadDef_t mainTaskControlBlock;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartMainTask(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadStaticDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 1024, defaultTaskBuffer, &defaultTaskControlBlock);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of mainTask */
  osThreadStaticDef(mainTask, StartMainTask, osPriorityNormal, 0, 1024, mainTaskBuffer, &mainTaskControlBlock);
  mainTaskHandle = osThreadCreate(osThread(mainTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  vTaskSuspend(mainTaskHandle);
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for USB_DEVICE */
  /* USER CODE BEGIN StartDefaultTask */
	static portTickType PreviousWakeTime;
	const portTickType TimeIncrement = pdMS_TO_TICKS(5);
	PreviousWakeTime = xTaskGetTickCount();
	InkPaperInit();
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
	lv_port_fs_init();
	vTaskResume(mainTaskHandle);
    HAL_GPIO_WritePin(USB_EN_GPIO_Port,USB_EN_Pin,GPIO_PIN_SET);
  /* Infinite loop */
  for(;;)
  {
      USART1_REC_Handler();
      vTaskDelayUntil( &PreviousWakeTime,TimeIncrement );
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartMainTask */
/**
* @brief Function implementing the mainTask thread.
* @param argument: Not used
* @retval None
*/

#define EventBit_USBConnect 0x01
#define EventBit_USBDisconnect 0x02
/* USER CODE END Header_StartMainTask */
void StartMainTask(void const * argument)
{
  /* USER CODE BEGIN StartMainTask */
	static portTickType PreviousWakeTime;
	const portTickType TimeIncrement = pdMS_TO_TICKS(5);
	PreviousWakeTime = xTaskGetTickCount();
	
	extern void lv_example3(void);
	lv_example3();
//	vTaskDelay(3000);
//	vTaskDelete(NULL);
	//lv_example1();
  /* Infinite loop */
//	lv_obj_t *sym_label = lv_label_create(lv_scr_act());
//	lv_obj_align(sym_label,LV_ALIGN_TOP_LEFT,0,0);
//	lv_obj_set_style_text_font(sym_label,&lv_font_montserrat_18,0);
//	lv_label_set_text(sym_label,"");
  for(;;)
  {
      lv_task_handler();
//		if(osSignalWait(EventBit_USBConnect,1).status==osEventSignal)
//		{
//			lv_label_set_text(sym_label, LV_SYMBOL_USB);
//		}
//		else if(osSignalWait(EventBit_USBDisconnect,1).status==osEventSignal)
//		{
//			lv_label_set_text(sym_label,"");
//		}
    vTaskDelayUntil( &PreviousWakeTime,TimeIncrement );
  }
  /* USER CODE END StartMainTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void lv_example1(void)
{
		taskENTER_CRITICAL();
		int16_t time_s=10;
		lv_obj_t * label = lv_label_create(lv_scr_act());
		lv_obj_t * label1 = lv_label_create(lv_scr_act());
		lv_obj_t * label2 = lv_label_create(lv_scr_act());
		lv_obj_center(label);
		lv_obj_align(label,LV_ALIGN_TOP_MID,0,0);
		lv_obj_align_to(label1,label,LV_ALIGN_OUT_BOTTOM_LEFT,-20,0);
		lv_obj_align_to(label2,label1,LV_ALIGN_OUT_BOTTOM_MID,0,0);
		lv_label_set_text_fmt(label," NUM:%d ",time_s);
		lv_label_set_text_fmt(label1," NUM:%d ",time_s);
		lv_label_set_text_fmt(label2," NUM:%d ",time_s);
		lv_obj_set_style_text_font(label1,&custom_yaheiBold_22,0);
		lv_obj_set_style_text_font(label2,&custom_yaheiBold_26,0);
		
		taskEXIT_CRITICAL();
		for(time_s--;time_s>=0;time_s--)
		{
			vTaskDelay(1000);
			lv_label_set_text_fmt(label," NUM:%d ",time_s);
			lv_label_set_text_fmt(label1," NUM:%d ",time_s);
			lv_label_set_text_fmt(label2," NUM:%d ",time_s);
		}
		taskENTER_CRITICAL();
		lv_obj_del_async(label1);
		lv_obj_del_async(label2);
		lv_label_set_text(label," Hello World! ");
		lv_obj_set_style_text_color(label,lv_color_hex(0XFF),0);
		lv_obj_set_style_bg_color(label,lv_color_hex(0X00),0);
		lv_obj_set_style_bg_opa(label,LV_OPA_100,0);
		taskEXIT_CRITICAL();
		vTaskDelay(3000);

		taskENTER_CRITICAL();
		lv_obj_set_width(label,10*13);
		lv_label_set_text(label," LVGL has been equipped on STM32! ");
		lv_label_set_long_mode(label,LV_LABEL_LONG_SCROLL);
		taskEXIT_CRITICAL();
}
void lv_example2(void)
{
		taskENTER_CRITICAL();
		extern lv_indev_t * indev_encoder;
	
		static lv_style_t style_defocus,style_focus;
		lv_style_init(&style_defocus);
		lv_style_set_text_font(&style_defocus,&custom_yaheiBold_18);
		lv_style_set_bg_color(&style_defocus,lv_color_hex(0XFF));
		lv_style_set_text_color(&style_defocus,lv_color_hex(0X00));
		lv_style_set_bg_opa(&style_defocus,LV_OPA_100);
		
		lv_style_init(&style_focus);
		lv_style_set_text_font(&style_focus,&custom_yaheiBold_18);
		lv_style_set_bg_color(&style_focus,lv_color_hex(0X00));
		lv_style_set_text_color(&style_focus,lv_color_hex(0XFF));
		lv_style_set_bg_opa(&style_focus,LV_OPA_100);
		
		lv_group_t * g = lv_group_create();
		lv_group_set_default(g);
		
		lv_obj_t * btn = lv_btn_create(lv_scr_act());
		lv_obj_t * label = lv_label_create(btn);
		lv_obj_add_style(btn,&style_defocus,LV_STATE_DEFAULT);
		lv_obj_add_style(btn,&style_focus,LV_STATE_FOCUSED);
		lv_label_set_text(label,"Label[1]");
		lv_obj_align(btn,LV_ALIGN_TOP_MID,0,5);
		//lv_obj_add_event_cb(btn,&label_style_cb,LV_EVENT_DEFOCUSED|LV_EVENT_FOCUSED,NULL);
		lv_obj_t * label_temp = label,*btn_last = btn,*btn_temp;
		
		for(int x=2;x<=4;x++)
	{
		btn_temp = lv_btn_create(lv_scr_act());
		label_temp = lv_label_create(btn_temp);
		lv_obj_add_style(btn_temp,&style_defocus,LV_STATE_DEFAULT);
		lv_obj_add_style(btn_temp,&style_focus,LV_STATE_FOCUSED);
		lv_label_set_text_fmt(label_temp,"Label[%d]",x);
		lv_obj_align_to(btn_temp,btn_last,LV_ALIGN_OUT_BOTTOM_MID,0,5);
		btn_last = btn_temp;
		
//		lv_obj_add_event_cb(label_temp,&label_style_cb,LV_EVENT_DEFOCUSED|LV_EVENT_FOCUSED,NULL);
	}
		
		lv_indev_set_group(indev_encoder,g);
		taskEXIT_CRITICAL();
}

void lv_example3(void)
{
    lv_obj_t *img = lv_img_create(lv_scr_act());  // don't use lv_obj_create!!!   qwq
    //lv_obj_set_style_text_font(img,&lv_font_montserrat_18,0);
//    lv_img_set_src(img,"P:me2.bin");
    lv_obj_center(img);
    //lv_obj_t *label = lv_label_create(lv_scr_act());
//    taskEXIT_CRITICAL();
//    vTaskDelay(3000);
//    taskENTER_CRITICAL();
//    EPD_1N54_V2_FullClearToPartial();
//    lv_img_set_src(img,"P:Images\\777.bin");
    lv_img_set_src(img,"P:hhh.bin");
//    vTaskDelay(5000);
//    EPD_1N54_V2_FullClearToPartial();
    while(HAL_SD_GetState(&hsd)==HAL_SD_STATE_BUSY);
//    printf("\n%d",audio_play((uint8_t *)"0:song1.mp3"));
//    vTaskDelay(10000);
//    printf("recording...\n");
//    FRESULT state =  f_opendir(&SDDir,"0:Records");
//    if(state != FR_OK)f_closedir(&SDDir);
//    else{
//        printf("\n%d",audio_recorde(1,10));
//    }
    MX_USB_DEVICE_Init();
    HAL_GPIO_WritePin(USB_EN_GPIO_Port,USB_EN_Pin,GPIO_PIN_RESET);
//    vTaskDelay(7000);
//    while(HAL_SD_GetState(&hsd)!=HAL_SD_STATE_READY);
//    HAL_GPIO_WritePin(USB_EN_GPIO_Port,USB_EN_Pin,GPIO_PIN_SET);

}
void lv_example4(void)
{
    setup_ui(&ui_test,NULL,NULL);
}










/**USB???**/
//void App_USB_State_IRQHandler(void)
//{
// static uint8_t old_state = 0;
//  /* ???usb?è±????????ä»? */
//  if(old_state)
//  {
//    /* USB???? */
//   if(pdev->dev_state == USBD_STATE_CONFIGURED) 
//   {       
//     osSignalSet (mainTaskHandle, EventBit_USBConnect);  
//   }
//   /* USB??? */
//   else if(pdev->dev_state==USBD_STATE_SUSPENDED)
//   {
//     osSignalSet (mainTaskHandle, EventBit_USBDisconnect);  
//   }
//    old_state = pdev->dev_state;
//  }
//}
/* USER CODE END Application */

