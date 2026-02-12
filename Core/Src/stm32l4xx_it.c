/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32l4xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern PCD_HandleTypeDef hpcd_USB_FS;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart3;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32L4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line4 interrupt.
  */
void EXTI4_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_IRQn 0 */

  /* USER CODE END EXTI4_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(AC_FAULT_Pin);
  /* USER CODE BEGIN EXTI4_IRQn 1 */

  AC_FAULT_CNT++;

  /* USER CODE END EXTI4_IRQn 1 */
}

/**
  * @brief This function handles TIM1 capture compare interrupt.
  */
void TIM1_CC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_CC_IRQn 0 */

	int i , sum_tmp;

  /* USER CODE END TIM1_CC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_CC_IRQn 1 */

  HAL_GPIO_TogglePin(RUN_LED_GPIO_Port, RUN_LED_Pin);
  //HAL_GPIO_TogglePin(FIRE_ALERT_GPIO_Port, FIRE_ALERT_Pin);

  for(i =0;i<Balsin_CNT - 1;i++){
    Balsin_Save[i]  = Balsin_Save[i+1] ;
  }
  Balsin_Save[Balsin_CNT - 1] = Read_BALSIN();

  sum_tmp = 0;

  for(i =0;i<Balsin_CNT;i++){
    sum_tmp  = sum_tmp + Balsin_Save[i] ;
  }

  if(sum_tmp == Balsin_CNT){
    Balsin_val = 1;
  }
  else{
    Balsin_val = 0;
  }

  /* USER CODE END TIM1_CC_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  //HAL_GPIO_TogglePin(ERR_LED_GPIO_Port, ERR_LED_Pin);

/*
  if(Pow_Mode == AC_Mode){
	  if((AC_FAULT_CNT > 1)){//&(AC_FAULT_CNT < 200)){
		AC_FAULT = 1;

	  }
	  else{
		AC_FAULT = 0;
		Pow_Mode = BAT_Mode;
		HAL_GPIO_WritePin(BAT_ON_GPIO_Port, BAT_ON_Pin, GPIO_PIN_SET);
	  }

  }
  AC_FAULT_CNT = 0;
*/


  if((AC_FAULT_CNT > 1)){//&(AC_FAULT_CNT < 200)){
	  Pow_Mode = AC_Mode;
  }
  else{
	  Pow_Mode = BAT_Mode;
  }

  AC_FAULT_CNT = 0;

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */

  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */
  //HAL_GPIO_TogglePin(ERR_LED_GPIO_Port, ERR_LED_Pin);

//  int charge_time = 18;
//    int wait_time = 40 + charge_time;

    if(Charge_Stat == 0){
      Charge_Cnt = 0;

      HAL_GPIO_WritePin(BAT_CHARGE_GPIO_Port , BAT_CHARGE_Pin , GPIO_PIN_RESET);

    }
    else if(Charge_Stat == 1){

    	/*
      if(Charge_Cnt < charge_time){

        HAL_GPIO_WritePin(BAT_CHARGE_GPIO_Port , BAT_CHARGE_Pin , GPIO_PIN_SET);

        Charge_Cnt++;
      }
      else if((Charge_Cnt >= charge_time) & (Charge_Cnt <wait_time)){

        HAL_GPIO_WritePin(BAT_CHARGE_GPIO_Port , BAT_CHARGE_Pin , GPIO_PIN_RESET);

        Charge_Cnt++;
      }
      else if(Charge_Cnt >=wait_time ){
        Charge_Cnt = 0;

        HAL_GPIO_WritePin(BAT_CHARGE_GPIO_Port , BAT_CHARGE_Pin , GPIO_PIN_RESET);

      }
      */

    	if(Bat_Charge_Mode == Immobility_Mode){
    		if(Charge_Cnt == 0){
    			Charge_Cnt++;
    		}
    		else if ((Charge_Cnt > 0)&(Charge_Cnt <= Immobility_Chargr_Table[0] )){
    			//HAL_GPIO_WritePin(BAT_CHARGE_GPIO_Port , BAT_CHARGE_Pin , GPIO_PIN_RESET);
    			HAL_GPIO_WritePin(BAT_CHARGE_GPIO_Port , BAT_CHARGE_Pin , GPIO_PIN_SET);
    			Charge_Cnt++;
    		}
    		else if ((Immobility_Chargr_Table[0] > 0)&(Charge_Cnt <= Immobility_Chargr_Table[1] )){
    			//HAL_GPIO_WritePin(BAT_CHARGE_GPIO_Port , BAT_CHARGE_Pin , GPIO_PIN_SET);
    			HAL_GPIO_WritePin(BAT_CHARGE_GPIO_Port , BAT_CHARGE_Pin , GPIO_PIN_RESET);
    			Charge_Cnt++;
    		}
    		else if (Immobility_Chargr_Table[1] > 0){
				//HAL_GPIO_WritePin(BAT_CHARGE_GPIO_Port , BAT_CHARGE_Pin , GPIO_PIN_RESET);
				HAL_GPIO_WritePin(BAT_CHARGE_GPIO_Port , BAT_CHARGE_Pin , GPIO_PIN_SET);
				Charge_Cnt = 0;
				//Bat_Charge_Mode = Compensation_Mode;
			}
    	}
    	else if(Bat_Charge_Mode == Compensation_Mode){
    		if(Charge_Cnt == 0){
    			Charge_Cnt++;
    		}
    		else if ((Charge_Cnt > 0)&(Charge_Cnt <= Compensation_Chargr_Table[Bat_Charge_Compensation_Volt][0] )){
    			//HAL_GPIO_WritePin(BAT_CHARGE_GPIO_Port , BAT_CHARGE_Pin , GPIO_PIN_RESET);
    			HAL_GPIO_WritePin(BAT_CHARGE_GPIO_Port , BAT_CHARGE_Pin , GPIO_PIN_SET);
    			Charge_Cnt++;
    		}
    		else if ((Compensation_Chargr_Table[Bat_Charge_Compensation_Volt][0] > 0)&(Charge_Cnt <= Compensation_Chargr_Table[Bat_Charge_Compensation_Volt][1] )){
    			//HAL_GPIO_WritePin(BAT_CHARGE_GPIO_Port , BAT_CHARGE_Pin , GPIO_PIN_SET);
    			HAL_GPIO_WritePin(BAT_CHARGE_GPIO_Port , BAT_CHARGE_Pin , GPIO_PIN_RESET);
    			Charge_Cnt++;
    		}
    		else if (Compensation_Chargr_Table[Bat_Charge_Compensation_Volt][1] > 0){
				//HAL_GPIO_WritePin(BAT_CHARGE_GPIO_Port , BAT_CHARGE_Pin , GPIO_PIN_RESET);
				HAL_GPIO_WritePin(BAT_CHARGE_GPIO_Port , BAT_CHARGE_Pin , GPIO_PIN_SET);
				Charge_Cnt = 0;
			}
    	}
    }

  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */

  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */

  Uart_rx3_buf_tmp[0] = USART3->RDR;

    if((Uart_rx3_buf_tmp[0] == 0x53)&( rx3_State == 0)){
        rx3_State = 1;
        rx3_buf_count = 0;
      }

    if(rx3_State == 1){
      Uart_rx3_buf[rx3_buf_count] = Uart_rx3_buf_tmp[0];
      rx3_buf_count++;
    }

    // 헤더확인을 통한 잘못된 데이타 확인
    if(rx3_buf_count > 3){
      if((Uart_rx3_buf[0] == 0x53) &(Uart_rx3_buf[1] == 0x54)){
      }
      else{
        rx3_State = 0;
        rx3_buf_count = 0;
      }
    }

    if((Uart_rx3_buf[rx3_buf_count - 1] == 0x44) & (Uart_rx3_buf[rx3_buf_count - 2] == 0x45)){

      rx3_Receive_complete = 1;
      rx3_buf_count_tmp = rx3_buf_count;
      rx3_State = 0;
      rx3_buf_count = 0;
    }
  /* USER CODE END USART3_IRQn 1 */
}

/**
  * @brief This function handles USB event interrupt through EXTI line 17.
  */
void USB_IRQHandler(void)
{
  /* USER CODE BEGIN USB_IRQn 0 */

  /* USER CODE END USB_IRQn 0 */
  HAL_PCD_IRQHandler(&hpcd_USB_FS);
  /* USER CODE BEGIN USB_IRQn 1 */

  /* USER CODE END USB_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
