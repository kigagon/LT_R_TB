/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

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
#define BALSIN_INT_Pin GPIO_PIN_13
#define BALSIN_INT_GPIO_Port GPIOC
#define BALJUN_INT_Pin GPIO_PIN_14
#define BALJUN_INT_GPIO_Port GPIOC
#define HANJUN_INT_Pin GPIO_PIN_15
#define HANJUN_INT_GPIO_Port GPIOC
#define DC2_MAIN_Pin GPIO_PIN_0
#define DC2_MAIN_GPIO_Port GPIOA
#define BAT_TEST_Pin GPIO_PIN_1
#define BAT_TEST_GPIO_Port GPIOA
#define BAT_CHARGE_Pin GPIO_PIN_2
#define BAT_CHARGE_GPIO_Port GPIOA
#define BAT_ON_Pin GPIO_PIN_3
#define BAT_ON_GPIO_Port GPIOA
#define AC_FAULT_Pin GPIO_PIN_4
#define AC_FAULT_GPIO_Port GPIOA
#define AC_FAULT_EXTI_IRQn EXTI4_IRQn
#define BAT_CVH_Pin GPIO_PIN_5
#define BAT_CVH_GPIO_Port GPIOA
#define BAT_CVL_Pin GPIO_PIN_6
#define BAT_CVL_GPIO_Port GPIOA
#define DC2_REP_Pin GPIO_PIN_7
#define DC2_REP_GPIO_Port GPIOA
#define DC1_SYS_Pin GPIO_PIN_0
#define DC1_SYS_GPIO_Port GPIOB
#define DC0_LAMP_Pin GPIO_PIN_1
#define DC0_LAMP_GPIO_Port GPIOB
#define RS485_RE_Pin GPIO_PIN_12
#define RS485_RE_GPIO_Port GPIOB
#define RS485_DE_Pin GPIO_PIN_13
#define RS485_DE_GPIO_Port GPIOB
#define FIRE_ALERT_Pin GPIO_PIN_4
#define FIRE_ALERT_GPIO_Port GPIOB
#define RUN_LED_Pin GPIO_PIN_5
#define RUN_LED_GPIO_Port GPIOB
#define ERR_LED_Pin GPIO_PIN_6
#define ERR_LED_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */


void SW_Com(void);
int Read_BALSIN(void);
int Read_HANJUN(void);
int Read_BALJUN(void);
void Read_ADC(void);
void Battery_Test(void);

void SUB_Com_SW_V(void);

extern uint32_t AC_FAULT_CNT ;

#define rx3_buf_len 40
extern uint8_t Uart_rx3_buf[rx3_buf_len] ;
extern uint8_t Uart_rx3_buf_tmp[3] ;

extern int rx3_State ;
extern int rx3_buf_count;
extern int rx3_buf_count_tmp;
extern int rx3_Receive_complete;

#define Balsin_CNT 2
extern int Balsin_Save[Balsin_CNT], Balsin_val;

#define AC_Mode 1
#define BAT_Mode 0

#define BAT_OK 1
#define BAT_False 2

extern int PYOSI_ADC, CCU_ADC, MAIN_ADC, REF_ADC, BAT_CVH_ADC, BAT_CVL_ADC , BAT_ADC;
extern int Bat_Charge_Currunt, Bat_Charge_Val, Bat_Currunt, Pow_Mode, Bat_Charge_Err;
extern int Charge_Stat, Charge_Cnt, AC_FAULT, BAT_State, Test_status,Pre_Pow_Mode;
#define tx_buf_len 18
extern uint8_t Uart_tx_buf[tx_buf_len] ;

#define Charge_Off_Mode		0
#define Immobility_Mode		1
#define Compensation_Mode	2


#define Charge_255_Mode		0
#define Charge_260_Mode		1
#define Charge_265_Mode		2
#define Charge_270_Mode		3
#define Charge_275_Mode		4


extern int Bat_Charge_Mode;
extern int Bat_Charge_Compensation_Volt;
extern int Immobility_Chargr_Table[2]; //1:On 0: Off
extern int Compensation_Chargr_Table[5][2]; //1:On 0: Off

extern uint8_t Bat_Test_Status;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
