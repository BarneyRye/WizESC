/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32g4xx_hal.h"

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
#define VBAT_ADC_Pin GPIO_PIN_0
#define VBAT_ADC_GPIO_Port GPIOA
#define BEMF_A_Pin GPIO_PIN_1
#define BEMF_A_GPIO_Port GPIOA
#define BEMF_B_Pin GPIO_PIN_2
#define BEMF_B_GPIO_Port GPIOA
#define BEMF_C_Pin GPIO_PIN_3
#define BEMF_C_GPIO_Port GPIOA
#define V_NEUT_Pin GPIO_PIN_0
#define V_NEUT_GPIO_Port GPIOB
#define PWM_CL_Pin GPIO_PIN_13
#define PWM_CL_GPIO_Port GPIOB
#define PWM_BL_Pin GPIO_PIN_14
#define PWM_BL_GPIO_Port GPIOB
#define PWM_AL_Pin GPIO_PIN_15
#define PWM_AL_GPIO_Port GPIOB
#define PWM_CH_Pin GPIO_PIN_8
#define PWM_CH_GPIO_Port GPIOA
#define PWM_BH_Pin GPIO_PIN_9
#define PWM_BH_GPIO_Port GPIOA
#define PWM_AH_Pin GPIO_PIN_10
#define PWM_AH_GPIO_Port GPIOA
#define DSHOT_Pin GPIO_PIN_4
#define DSHOT_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
