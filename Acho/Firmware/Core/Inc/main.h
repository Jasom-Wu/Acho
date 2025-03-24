/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MLCD_MODE_Pin GPIO_PIN_5
#define MLCD_MODE_GPIO_Port GPIOE
#define MLCD_DISP_Pin GPIO_PIN_6
#define MLCD_DISP_GPIO_Port GPIOE
#define KEY2_Pin GPIO_PIN_13
#define KEY2_GPIO_Port GPIOC
#define KEY1_Pin GPIO_PIN_0
#define KEY1_GPIO_Port GPIOA
#define KEY1_EXTI_IRQn EXTI0_IRQn
#define MLCD_EXTCOMIN_Pin GPIO_PIN_4
#define MLCD_EXTCOMIN_GPIO_Port GPIOA
#define MLCD_CS_Pin GPIO_PIN_6
#define MLCD_CS_GPIO_Port GPIOA
#define ENCODER_ENTER_Pin GPIO_PIN_4
#define ENCODER_ENTER_GPIO_Port GPIOC
#define USB_EN_Pin GPIO_PIN_6
#define USB_EN_GPIO_Port GPIOD
#define LED_BLUE_Pin GPIO_PIN_5
#define LED_BLUE_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
