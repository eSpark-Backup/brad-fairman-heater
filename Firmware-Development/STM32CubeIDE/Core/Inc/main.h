/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32g0xx_hal.h"

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
#define GEAR_MOTOR_CTRL_Pin GPIO_PIN_7
#define GEAR_MOTOR_CTRL_GPIO_Port GPIOB
#define BUTTON_Pin GPIO_PIN_0
#define BUTTON_GPIO_Port GPIOA
#define BUTTON_EXTI_IRQn EXTI0_1_IRQn
#define TEMP_SENSE_Pin GPIO_PIN_1
#define TEMP_SENSE_GPIO_Port GPIOA
#define BAT_SENSE_Pin GPIO_PIN_2
#define BAT_SENSE_GPIO_Port GPIOA
#define LED_G_Pin GPIO_PIN_6
#define LED_G_GPIO_Port GPIOA
#define LED_R_Pin GPIO_PIN_7
#define LED_R_GPIO_Port GPIOA
#define HEATER_CTRL_Pin GPIO_PIN_0
#define HEATER_CTRL_GPIO_Port GPIOB
#define VIBE_MOTOR_CTRL_Pin GPIO_PIN_3
#define VIBE_MOTOR_CTRL_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
