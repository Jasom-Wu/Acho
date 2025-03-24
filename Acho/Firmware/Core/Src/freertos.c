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
#include "../../User/bsp_key.h"
#include "usbd_def.h"
#include "usbd_core.h"
#include "sdio.h"
#include "ff.h"
#include "fatfs.h"
#include "pages/page_main.h"
#include "gui_setup.h"
#include "../../User/fops.h"
#include "../../User/bsp_usart.h"
#include "pages/page_audio.h"
#include "WM8960Apps.h"
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
extern TIM_HandleTypeDef htim4;
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

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize) {
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
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
    static portTickType PreviousWakeTime;
    const portTickType TimeIncrement = pdMS_TO_TICKS(5);
    PreviousWakeTime = xTaskGetTickCount();
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    lv_port_fs_init();
    GUIInit();
    HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_ALL);
    vTaskResume(mainTaskHandle);
    HAL_GPIO_WritePin(USB_EN_GPIO_Port, USB_EN_Pin, GPIO_PIN_SET);
    /* Infinite loop */
    for (;;) {
//        if(audio_play_state==AUDIO_PLAY){
//            vTaskDelay(100);
//            audio_play((uint8_t *) global_BUFF, &audio_play_state);
//        }
        USART1_REC_Handler();
        PlayWaveFile(global_BUFF);
        vTaskDelayUntil(&PreviousWakeTime, TimeIncrement);
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

    extern void lv_example4(void);
    lv_example4();
//	vTaskDelay(3000);
//	vTaskDelete(NULL);
    //lv_example1();
    /* Infinite loop */
//	lv_obj_t *sym_label = lv_label_create(lv_scr_act());
//	lv_obj_align(sym_label,LV_ALIGN_TOP_LEFT,0,0);
//	lv_obj_set_style_text_font(sym_label,&lv_font_montserrat_18,0);
//	lv_label_set_text(sym_label,"");
    for (;;) {
        lv_task_handler();
//		if(osSignalWait(EventBit_USBConnect,1).status==osEventSignal)
//		{
//			lv_label_set_text(sym_label, LV_SYMBOL_USB);
//		}
//		else if(osSignalWait(EventBit_USBDisconnect,1).status==osEventSignal)
//		{
//			lv_label_set_text(sym_label,"");
//		}
        vTaskDelayUntil(&PreviousWakeTime, TimeIncrement);
    }
  /* USER CODE END StartMainTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void lv_example4(void) {
    setup_ui(&ui_main, &ui_ROOT, NULL);
//    uint8_t path[] = "0:heaven.mp3";
//    uint8_t path[] = "0:song.flac";
//    audio_play(path);
//    uint8_t path2[] = "0:heaven.mp3";
//    audio_play(path2);
//    uint8_t path1[] = "0:travel.mp3";
//    audio_play(path1);
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

