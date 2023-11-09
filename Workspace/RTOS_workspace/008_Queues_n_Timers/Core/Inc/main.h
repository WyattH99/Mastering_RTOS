/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include <stdio.h>

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */


// Define a command
typedef struct{
	uint8_t payload[10]; // stores the data bytes
	uint8_t len; // length
}command_t;

// Application states
typedef enum{
	sMainMenu = 0,
	sLedEffect,
	sRtcMenu,
	sRtcTimeConfig,
	sRtcDateConfig,
	sRtcReport,
}state_t;

extern TaskHandle_t menu_task_handle;
extern TaskHandle_t cmd_task_handle;
extern TaskHandle_t print_task_handle;
extern TaskHandle_t led_task_handle;
extern TaskHandle_t rtc_task_handle;

extern QueueHandle_t q_data_handle;
extern QueueHandle_t q_print_handle;

extern TimerHandle_t led_timer_handle[4];
extern TimerHandle_t rtc_timer;

extern state_t curr_state;

extern RTC_HandleTypeDef hrtc;
extern UART_HandleTypeDef huart2;


/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

void menu_task_handler(void* param);
void cmd_task_handler(void* param);
void print_task_handler(void* param);
void led_task_handler(void* param);
void rtc_task_handler(void* param);

void led_effect_stop(void);
void led_effect(int n);

void LED_effect1(void);
void LED_effect2(void);
void LED_effect3(void);
void LED_effect4(void);

void show_time_date_itm(void);
void show_time_date(void);
void rtc_configure_time(RTC_TimeTypeDef* time);
void rtc_configure_date(RTC_DateTypeDef* date);
int validate_rtc_information(RTC_TimeTypeDef* time, RTC_DateTypeDef* date);



/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define LD5_Pin GPIO_PIN_10
#define LD5_GPIO_Port GPIOB
#define LD4_Pin GPIO_PIN_8
#define LD4_GPIO_Port GPIOA
#define LD3_Pin GPIO_PIN_9
#define LD3_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define LD6_Pin GPIO_PIN_4
#define LD6_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

#define RED1_LED LD3_Pin
#define RED2_LED LD4_Pin
#define GREEN1_LED LD5_Pin
#define GREEN2_LED LD6_Pin


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
