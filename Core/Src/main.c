/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Compile_Data.h"
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
ADC_HandleTypeDef hadc1;

IWDG_HandleTypeDef hiwdg;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
//
#define Off_Battery_val 210000
#define Err_Battery_val 220000
#define Low_Battery_val 256000
//#define High_Battery_val 270000
#define High_Battery_val 285000                 // 자립형

uint8_t Uart_tmp[] = "Test\r\n";

uint32_t AC_FAULT_CNT ;

uint8_t Uart_rx3_buf[rx3_buf_len] ;
uint8_t Uart_rx3_buf_tmp[3] ;
int rx3_State ;
int rx3_buf_count;
int rx3_buf_count_tmp;
int rx3_Receive_complete;

int Balsin_signal, AC_FAULT;
int PYOSI_ADC, CCU_ADC, MAIN_ADC, REF_ADC, BAT_CVH_ADC, BAT_CVL_ADC;
int BAT_CVH_ADC_O, BAT_CVL_ADC_O;
int BAT_ADC , Bat_Currunt, Bat_Charge_Currunt, Bat_Charge_Val, Charge_Stat = 0,Charge_Cnt;
int Pow_Mode, Bat_Charge_Err, BAT_State, Test_status, Pre_Pow_Mode;
int HANJUN_sig,BALJUN_sig;
uint8_t Uart_tx_buf[tx_buf_len] ;

int Fire_signal;

int Balsin_Save[Balsin_CNT], Balsin_val;

int Bat_Charge_Mode;
int Bat_Charge_Compensation_Volt;
int Immobility_Chargr_Table[2]; //1:On 0: Off
int Compensation_Chargr_Table[5][2]; //1:On 0: Off

uint8_t Bat_Test_Status;

uint8_t TB_Address = 1 ;
#define Sub_TB_length      15

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_IWDG_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

///////////////Start timer interrupt operation function///////////////////

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM1){
	  HAL_IWDG_Refresh(&hiwdg);
  }

}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART3_UART_Init();
  MX_USB_DEVICE_Init();
  MX_ADC1_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */

  /* huart3 RX Interrupt  Enable */
  /* Process Unlocked */
  __HAL_UNLOCK(&huart3);
  /* Enable the UART Parity Error Interrupt */
  __HAL_UART_ENABLE_IT(&huart3, UART_IT_PE);
  /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
  __HAL_UART_ENABLE_IT(&huart3, UART_IT_ERR);
  /* Enable the UART Data Register not empty Interrupt */
  __HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);


  HAL_TIM_OC_Start_IT(&htim1,TIM_CHANNEL_1);
  HAL_TIM_OC_Start_IT(&htim2,TIM_CHANNEL_1);
  HAL_TIM_OC_Start_IT(&htim3,TIM_CHANNEL_1);

  Pow_Mode = AC_Mode;
  BAT_State = BAT_OK;

  Immobility_Chargr_Table[0] = 6; //1:On 0: Off
  Immobility_Chargr_Table[1] = Immobility_Chargr_Table[0] + 38; //1:On 0: Off


  //25.5V
  Compensation_Chargr_Table[0][0] =  6; //1:On 0: Off
  Compensation_Chargr_Table[0][1] = 26. + Compensation_Chargr_Table[0][0]; //1:On 0: Off
  //26V
  Compensation_Chargr_Table[1][0] =  6; //1:On 0: Off
  Compensation_Chargr_Table[1][1] = 38 + Compensation_Chargr_Table[1][0]; //1:On 0: Off
  //26.5V
  Compensation_Chargr_Table[2][0] =  6; //1:On 0: Off
  Compensation_Chargr_Table[2][1] = 56 + Compensation_Chargr_Table[2][0]; //1:On 0: Off
  //27V
  Compensation_Chargr_Table[3][0] =  6; //1:On 0: Off
  Compensation_Chargr_Table[3][1] = 83 + Compensation_Chargr_Table[3][0]; //1:On 0: Off
  //27.5V
  Compensation_Chargr_Table[4][0] =  6; //1:On 0: Off
  Compensation_Chargr_Table[4][1]= 100 + Compensation_Chargr_Table[4][0] ; //1:On 0: Off

  Bat_Charge_Mode = Immobility_Mode;
  Bat_Test_Status = 0;
  //Bat_Charge_Mode = Compensation_Mode;

  Compile_Date();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

      /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
  __HAL_UART_ENABLE_IT(&huart3, UART_IT_ERR);
  /* Enable the UART Data Register not empty Interrupt */
  __HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);

    if(rx3_Receive_complete == 1){
      SW_Com();
    }

    //Balsin_signal = Read_BALSIN();
    //AC_FAULT = Read_AC_FAULT();


    //AC_FAULT = Read_AC_FAULT();

    Read_ADC();
    Pre_Pow_Mode = Pow_Mode;
//    if(AC_FAULT == 1){
//    	if(PYOSI_ADC < 220000){
//    		Pow_Mode = BAT_Mode;
//    	}
//    	else{
//    		Pow_Mode = AC_Mode;
//    	}
//    }
//    else{
//      Pow_Mode = BAT_Mode;
//    }

    /*
    if(Pow_Mode == BAT_Mode){
		if(PYOSI_ADC < 10000){
			Pow_Mode = BAT_Mode;
		}
		else{
			Pow_Mode = AC_Mode;
		}
    }
    */

    //Pow_Mode = AC_Mode;
    if(Pow_Mode == AC_Mode){

//    	if(Pre_Pow_Mode == BAT_Mode){
//    		HAL_Delay(300);
//    	}

      HAL_GPIO_WritePin(BAT_ON_GPIO_Port, BAT_ON_Pin, GPIO_PIN_RESET);

	  //HAL_GPIO_WritePin(BAT_CHARGE_GPIO_Port , BAT_CHARGE_Pin , GPIO_PIN_RESET);
	  HAL_Delay(50);

      Read_ADC();

      //BAT_ADC = 230000;

      /*
      if(Bat_Test_Status == 1){
    	  Bat_Test_Status = 0;
    	  Battery_Test();
      }
*/

      if((BAT_ADC >= Err_Battery_val)&(BAT_ADC < High_Battery_val )){
        Charge_Stat = 1;
        BAT_State = BAT_OK;
        if(Bat_Charge_Mode == Immobility_Mode){

        	if(BAT_ADC > 280000){
        		Bat_Charge_Mode = Compensation_Mode;
        	}
        }
        else if(Bat_Charge_Mode == Compensation_Mode){
        	if((BAT_ADC >= 255000)&(BAT_ADC < 260000 )){
        		Bat_Charge_Compensation_Volt = Charge_255_Mode;
        	}
        	else if((BAT_ADC >= 260000)&(BAT_ADC < 265000 )){
        		Bat_Charge_Compensation_Volt = Charge_260_Mode;
        	}
        	else if((BAT_ADC >= 265000)&(BAT_ADC < 270000 )){
        		Bat_Charge_Compensation_Volt = Charge_265_Mode;
        	}
        	else if((BAT_ADC >= 270000)&(BAT_ADC < 275000 )){
        		Bat_Charge_Compensation_Volt = Charge_270_Mode;
        	}
        	else if((BAT_ADC >= 275000)&(BAT_ADC < 280000 )){
        		Bat_Charge_Compensation_Volt = Charge_275_Mode;
        	}
        	else{
        		Bat_Charge_Compensation_Volt = Charge_Off_Mode;
        	}
        }
      }

      else if(BAT_ADC >= High_Battery_val ){
        Charge_Stat = 0;
        Bat_Charge_Currunt = 0;
        BAT_State = BAT_OK;
      }
      else if(BAT_ADC < Err_Battery_val){
        Charge_Stat = 2;
        Bat_Charge_Currunt = 0;
        BAT_State = BAT_False;
      }
    }
    else{
      Charge_Stat = 0;
      Read_ADC();
      BAT_ADC = BAT_CVL_ADC;

//      BAT_ADC

      if(BAT_ADC < Off_Battery_val){
        HAL_GPIO_WritePin(BAT_ON_GPIO_Port, BAT_ON_Pin, GPIO_PIN_RESET);
//        while(1){
//        }
      }
      else{
        HAL_GPIO_WritePin(BAT_ON_GPIO_Port, BAT_ON_Pin, GPIO_PIN_SET);
      }
    }

    if(Charge_Stat != 1){
      Read_ADC();
      BAT_ADC = BAT_CVL_ADC;
    }
    else{
      if(Charge_Cnt == 4){
        Read_ADC();       //충전중에는 베터리 값을 갱신하지 않는다.
        Bat_Charge_Currunt = Bat_Currunt ;
        Bat_Charge_Val = BAT_CVH_ADC;
      }
      else if((Charge_Cnt >= 15) & (Charge_Cnt <40)){
      Read_ADC();
      BAT_ADC = BAT_CVL_ADC;
      }
    }
    if(BAT_ADC < 270000){
      for(int i=0;i<100;i++){
        if(rx3_Receive_complete == 1){
          SW_Com();
        }
        HAL_Delay(1);
      }
    }

  }

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 24;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK|RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_247CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_32;
  hiwdg.Init.Window = 4095;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 8000;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 10000;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 99;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 800;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 10000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, BAT_TEST_Pin|BAT_CHARGE_Pin|BAT_ON_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RS485_RE_Pin|RS485_DE_Pin|FIRE_ALERT_Pin|RUN_LED_Pin
                          |ERR_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : BALSIN_INT_Pin BALJUN_INT_Pin HANJUN_INT_Pin */
  GPIO_InitStruct.Pin = BALSIN_INT_Pin|BALJUN_INT_Pin|HANJUN_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : BAT_TEST_Pin BAT_CHARGE_Pin BAT_ON_Pin */
  GPIO_InitStruct.Pin = BAT_TEST_Pin|BAT_CHARGE_Pin|BAT_ON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : AC_FAULT_Pin */
  GPIO_InitStruct.Pin = AC_FAULT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(AC_FAULT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RS485_RE_Pin RS485_DE_Pin FIRE_ALERT_Pin RUN_LED_Pin
                           ERR_LED_Pin */
  GPIO_InitStruct.Pin = RS485_RE_Pin|RS485_DE_Pin|FIRE_ALERT_Pin|RUN_LED_Pin
                          |ERR_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

int Uart3_crc = 0;
int Uart_crc = 0;

void SW_Com(void){

  // CRC 체크

  int i,pow_st;

  // 전송할 중계기 정보를 확인한다.

  Uart3_crc = 0;

  for(i = 0; i <rx3_buf_count_tmp - 5 ; i++){
    Uart3_crc = Uart3_crc ^ Uart_rx3_buf[i+2];
  }

  if(Uart3_crc == Uart_rx3_buf[rx3_buf_count_tmp - 3]){
    //0x51 , 'Q' ,중계기 정보 요청 (UI에서 설정한 값만)

    if(Uart_rx3_buf[4] == 0x54){        // TB board: '0x54'
      if(Uart_rx3_buf[3] == 0x51){



        Uart_tx_buf[0] = 0x53;    //S
        Uart_tx_buf[1] = 0x54;    //T
        Uart_tx_buf[2] = 0x54;    //T
        Uart_tx_buf[3] = 0x52;    //R
        Uart_tx_buf[4] = 0x01;    //address
        Uart_tx_buf[5] = 0x0A;    //version 10 -> 1.0
        Uart_tx_buf[6] = 0x00;    //외부 발신
        Uart_tx_buf[7] = 0x00;    //외부 전원
        Uart_tx_buf[8] = 0x00;    //베터리 AC 상태
        Uart_tx_buf[9] = 0x00;    //베터리 전압
        Uart_tx_buf[10] = 0x00;   //메인 전압
        Uart_tx_buf[11] = 0x00;   //충전 전압
        Uart_tx_buf[12] = 0x00;   //베터리 테스트 결과
        Uart_tx_buf[13] = 0x00;   //dummy1
        Uart_tx_buf[14] = 0x00;   //dummy2
        Uart_tx_buf[15] = 0x00;   //CRC
        Uart_tx_buf[16] = 0x45;   //E
        Uart_tx_buf[17] = 0x44;   //D

        Uart_crc = 0;

        //Balsin_signal = Read_BALSIN();
        Balsin_signal = Balsin_val;
        HANJUN_sig = Read_HANJUN();
        BALJUN_sig = Read_BALJUN();

        if((HANJUN_sig == 0) &(BALJUN_sig ==0)){
          pow_st = 0;
        }
        else if((HANJUN_sig == 1) &(BALJUN_sig ==0)){
          pow_st = 1;
        }
        else if((HANJUN_sig == 0) &(BALJUN_sig ==1)){
          pow_st = 2;
        }
        else if((HANJUN_sig == 1) &(BALJUN_sig ==1)){
          pow_st = 3;
        }

        Uart_tx_buf[6] = Balsin_signal;
        Uart_tx_buf[7] = pow_st;    //외부 전원 1: 한전 , 2:발전
        //Uart_tx_buf[8] = AC_FAULT;    //베터리 AC 상태
        Uart_tx_buf[8] = Pow_Mode;

        if(((BAT_ADC / 1000) - 100)<0){
          Uart_tx_buf[9] = 0;    //베터리 전압
        }
        else {
          Uart_tx_buf[9] = (BAT_ADC / 1000) - 100;    //베터리 전압
        }

        if(((MAIN_ADC / 1000) - 100)<0){
          Uart_tx_buf[10] = 0;    //베터리 전압
        }
        else {
          Uart_tx_buf[10] = (MAIN_ADC / 1000) - 100;    //베터리 전압
        }

         if(((Bat_Charge_Val / 1000) - 100)<0){
          Uart_tx_buf[11] = 0;    //베터리 전압
        }
        else {
          Uart_tx_buf[11] = (Bat_Charge_Val / 1000) - 100;    //베터리 전압
        }


        //Uart_tx_buf[10] = (MAIN_ADC / 1000) - 100;   //메인 전압
        //Uart_tx_buf[11] = (Bat_Charge_Val / 1000) - 100;   //충전 전압
        Uart_tx_buf[12] = ((Charge_Stat << 4 )&0xF0) | (Test_status&0x0F);   //베터리 테스트 결과

        Uart_tx_buf[14] = 24 ;//dummy2   표시등 퓨즈 단선 전압값 , 벽부형만 적용 자립형은 24v고정
        for(i = 0; i <tx_buf_len - 5 ; i++){
          Uart_crc = Uart_crc ^ Uart_tx_buf[i+2];
        }
        Uart_tx_buf[15] = Uart_crc;


        HAL_GPIO_WritePin(RS485_DE_GPIO_Port , RS485_DE_Pin , GPIO_PIN_SET);
        HAL_GPIO_WritePin(RS485_RE_GPIO_Port , RS485_RE_Pin , GPIO_PIN_SET);

        HAL_Delay(1);
        if(HAL_UART_Transmit(&huart3, Uart_tx_buf, sizeof(Uart_tx_buf), 1000)!= HAL_OK)
            {
              Error_Handler();
            }

        HAL_Delay(1);

        HAL_GPIO_WritePin(RS485_DE_GPIO_Port , RS485_DE_Pin , GPIO_PIN_RESET);
        HAL_GPIO_WritePin(RS485_RE_GPIO_Port , RS485_RE_Pin , GPIO_PIN_RESET);


      }
      else if(Uart_rx3_buf[3] == 0x53){

        if(Uart_rx3_buf[6] == 0x01){
        	if(Bat_Test_Status == 0){
        		Bat_Test_Status = 1;
        		Battery_Test();
        	}
        	Bat_Test_Status = 0;
        }
        else if(Uart_rx3_buf[6] == 0x00){
        	Test_status = 0;
        }

        if(Uart_rx3_buf[8] == 0x01){
          Fire_signal = 1;
          HAL_GPIO_WritePin(FIRE_ALERT_GPIO_Port , FIRE_ALERT_Pin , GPIO_PIN_SET);
        }
        else if(Uart_rx3_buf[8] == 0x00){
          Fire_signal = 0;
          HAL_GPIO_WritePin(FIRE_ALERT_GPIO_Port , FIRE_ALERT_Pin , GPIO_PIN_RESET);
        }
      }
      else if(Uart_rx3_buf[3] == 0x56){
    	  SUB_Com_SW_V();
      }
    }
  }

  for(i = 0; i <rx3_buf_len ; i++){
    Uart_rx3_buf[i] = 0;
  }


  rx3_Receive_complete = 0;

}

int Read_BALSIN(void){
  int Read_val = 0;
  if(HAL_GPIO_ReadPin(BALSIN_INT_GPIO_Port, BALSIN_INT_Pin) == GPIO_PIN_RESET){
    Read_val = 1;
  }
  else{
    Read_val = 0;
  }

  return Read_val;
}

int Read_HANJUN(void){
  int Read_val = 0;
  if(HAL_GPIO_ReadPin(HANJUN_INT_GPIO_Port, HANJUN_INT_Pin) == GPIO_PIN_RESET){
    Read_val = 1;
  }
  else{
    Read_val = 0;
  }

  return Read_val;
}

int Read_BALJUN(void){
  int Read_val = 0;
  if(HAL_GPIO_ReadPin(BALJUN_INT_GPIO_Port, BALJUN_INT_Pin) == GPIO_PIN_RESET){
    Read_val = 1;
  }
  else{
    Read_val = 0;
  }

  return Read_val;
}


void Read_ADC(void){

  //ADC Setting
  float adc_dalta ;

  adc_dalta = 8.06;

  ADC_ChannelConfTypeDef sConfig = {0};

  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */


   ///////////////////////////////////////////////////////////////////

  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_47CYCLES_5;
  //sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  int REF_ADC_tmp;
  HAL_ADC_Start(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, 1000);
  REF_ADC_tmp = HAL_ADC_GetValue(&hadc1);
  HAL_ADC_Stop(&hadc1);

  REF_ADC_tmp =(int) ( REF_ADC_tmp * adc_dalta * 8 );

  REF_ADC = REF_ADC_tmp;

  ///////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////

  sConfig.Channel = ADC_CHANNEL_16;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  int PYOSI_ADC_tmp;
  HAL_ADC_Start(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, 1000);
  PYOSI_ADC_tmp = HAL_ADC_GetValue(&hadc1);
  HAL_ADC_Stop(&hadc1);

  PYOSI_ADC_tmp =(int) ( PYOSI_ADC_tmp * adc_dalta * 15 );

  PYOSI_ADC = PYOSI_ADC_tmp;

  ///////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////

  sConfig.Channel = ADC_CHANNEL_15;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  int CCU_ADC_tmp;
  HAL_ADC_Start(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, 1000);
  CCU_ADC_tmp = HAL_ADC_GetValue(&hadc1);
  HAL_ADC_Stop(&hadc1);

  CCU_ADC_tmp = (int) (CCU_ADC_tmp * adc_dalta * 15) ;

  CCU_ADC = CCU_ADC_tmp;
  MAIN_ADC = CCU_ADC;

  ///////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////
  sConfig.Channel = ADC_CHANNEL_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  int MAIN_ADC_tmp;
  HAL_ADC_Start(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, 1000);
  MAIN_ADC_tmp = HAL_ADC_GetValue(&hadc1);
  HAL_ADC_Stop(&hadc1);

  MAIN_ADC_tmp = (int) (MAIN_ADC_tmp * adc_dalta * 15) ;

  //MAIN_ADC = MAIN_ADC_tmp;

  ///////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////
  sConfig.Channel = ADC_CHANNEL_10;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_ADC_Start(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, 1000);
  BAT_CVH_ADC_O = HAL_ADC_GetValue(&hadc1);
  HAL_ADC_Stop(&hadc1);

  BAT_CVH_ADC = (int) (BAT_CVH_ADC_O * adc_dalta * 15) ;

  if(BAT_CVH_ADC > 5000){
    BAT_CVH_ADC = BAT_CVH_ADC + 11000;
  }


  ///////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////

  sConfig.Channel = ADC_CHANNEL_11;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }


  HAL_ADC_Start(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, 1000);
  BAT_CVL_ADC_O = HAL_ADC_GetValue(&hadc1);
  HAL_ADC_Stop(&hadc1);

  BAT_CVL_ADC = (int) (BAT_CVL_ADC_O * adc_dalta * 15) ;

  ///////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////

  if(BAT_CVH_ADC > 5000){
    BAT_CVL_ADC = BAT_CVL_ADC+ 11000;;
  }

  Bat_Currunt = (BAT_CVH_ADC - BAT_CVL_ADC)/2;
}


void Battery_Test(void){

  int i;

  Test_status = 0;

  if(Pow_Mode == AC_Mode){
    Charge_Stat = 2;
    HAL_GPIO_WritePin(BAT_CHARGE_GPIO_Port , BAT_CHARGE_Pin , GPIO_PIN_RESET);

    for(i=0;i<100;i++){

      HAL_GPIO_WritePin(BAT_TEST_GPIO_Port , BAT_TEST_Pin , GPIO_PIN_SET);
      HAL_Delay(80);

      if(rx3_Receive_complete == 1){
        SW_Com();
      }

      Read_ADC();
      BAT_ADC = BAT_CVL_ADC;

      if(BAT_ADC < Err_Battery_val){
        HAL_GPIO_WritePin(BAT_TEST_GPIO_Port , BAT_TEST_Pin , GPIO_PIN_RESET);
        Test_status = 2;
        break;
      }
    }

    if(i == 100){
      Test_status = 1;
    }

    Charge_Stat = 0;
    HAL_GPIO_WritePin(BAT_TEST_GPIO_Port , BAT_TEST_Pin , GPIO_PIN_RESET);
  }
  else{
    Test_status = 1;
  }
}

void SUB_Com_SW_V(void){

	uint8_t Uart_crc;

    Uart_tx_buf[0] = 0x53;    //S
    Uart_tx_buf[1] = 0x54;    //T
    Uart_tx_buf[2] = 0x53;    //S
    Uart_tx_buf[3] = 0x76;    //v
    Uart_tx_buf[4] = 0x54;    //TB
    Uart_tx_buf[5] = TB_Address;    //address
    Uart_tx_buf[6] = F_Version_Year;
    Uart_tx_buf[7] = F_Version_Month;
    Uart_tx_buf[8] = F_Version_Day;
    Uart_tx_buf[9] = F_Version_Hour;
    Uart_tx_buf[10] = F_Version_Min;
    Uart_tx_buf[11] = F_Version_Sec;
    Uart_tx_buf[12] = 0x00;   //CRC
    Uart_tx_buf[13] = 0x45;   //E
    Uart_tx_buf[14] = 0x44;   //D


    for(int i = 0; i <Sub_TB_length - 5 ; i++){
      Uart_crc = Uart_crc ^ Uart_tx_buf[i+2];
    }
    Uart_tx_buf[Sub_TB_length-3] = Uart_crc;


    HAL_GPIO_WritePin(RS485_DE_GPIO_Port , RS485_DE_Pin , GPIO_PIN_SET);
    HAL_GPIO_WritePin(RS485_RE_GPIO_Port , RS485_RE_Pin , GPIO_PIN_SET);

    HAL_Delay(1);
    if(HAL_UART_Transmit(&huart3, Uart_tx_buf, Sub_TB_length, 1000)!= HAL_OK)
        {
          Error_Handler();
        }

    HAL_Delay(1);

    HAL_GPIO_WritePin(RS485_DE_GPIO_Port , RS485_DE_Pin , GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RS485_RE_GPIO_Port , RS485_RE_Pin , GPIO_PIN_RESET);

}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
