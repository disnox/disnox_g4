/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint32_t time1 = 0, time2 = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI3_Init();
  /* USER CODE BEGIN 2 */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
	
	
	LCD_Init();//LCD³õÊ¼»¯
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		if(HAL_GetTick() - time1 > 200)
		{
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
			time1 = HAL_GetTick();
		}
		
		if(HAL_GetTick() - time2 > 500)
		{
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_14);
			time2 = HAL_GetTick();
		}
		
		LCD_ShowString(10, 5,(uint8_t *)"[READY]", GREEN, BLACK, 32, 0);
		
		LCD_ShowString(130, 5,(uint8_t *)"Vbus:", WHITE, BLACK, 16, 0);
		LCD_ShowFloatNum1(170, 5, 24.12, 3, 2, GREEN, BLACK, 16);
		LCD_ShowString(220, 5, (uint8_t *)"V", WHITE, BLACK, 16, 0);
		
		LCD_ShowString(130, 21,(uint8_t *)"Ibus:", WHITE, BLACK, 16, 0);
		LCD_ShowFloatNum1(170, 21, 00.00, 3, 2, GREEN, BLACK, 16);
		LCD_ShowString(220, 21, (uint8_t *)"A", WHITE, BLACK, 16, 0);
		
		LCD_ShowString(130, 37,(uint8_t *)"Tmos:", WHITE, BLACK, 16, 0);
		LCD_ShowFloatNum1(170, 37, 31.27, 3, 2, GREEN, BLACK, 16);
		LCD_ShowString(220, 37, (uint8_t *)"C", WHITE, BLACK, 16, 0);
		
		LCD_DrawLine(10, 60, 10, 135, WHITE);
		LCD_DrawLine(230, 60, 230, 135, WHITE);
		
		LCD_ShowString(20, 40, (uint8_t *)"Mode :", WHITE, BLACK, 16, 0);
		LCD_ShowString(20, 60, (uint8_t *)"Iq.  :", WHITE, BLACK, 16, 0);
		LCD_ShowString(20, 80, (uint8_t *)"Vel. :", WHITE, BLACK, 16, 0);
		LCD_ShowString(20, 100,(uint8_t *)"Pos. :", WHITE, BLACK, 16, 0);
		LCD_ShowString(20, 120,(uint8_t *)"Tor. :", WHITE, BLACK, 16, 0);
		
		LCD_ShowString(76, 40, (uint8_t *)"[Pos.]", WHITE, BLACK, 16, 0);
		
		LCD_ShowFloatNum1(68, 60, 00.00, 3, 2, BRRED, BLACK, 16);
		LCD_ShowString(124, 60, (uint8_t *)"->", WHITE, BLACK, 16, 0);
		LCD_ShowFloatNum1(140, 60, 00.00, 3, 2, GREEN, BLACK, 16);
		LCD_ShowString(196, 60, (uint8_t *)"A", WHITE, BLACK, 16, 0);
		
		LCD_ShowFloatNum1(68, 80, 00.00, 3, 2, GREEN, BLACK, 16);
		LCD_ShowString(124, 80, (uint8_t *)"->", WHITE, BLACK, 16, 0);
		LCD_ShowFloatNum1(140, 80, 00.00, 3, 2, GREEN, BLACK, 16);
		LCD_ShowString(196, 80, (uint8_t *)"r/s", WHITE, BLACK, 16, 0);
		
		LCD_ShowFloatNum1(68, 100, 00.00, 3, 2, GREEN, BLACK, 16);
		LCD_ShowString(124, 100, (uint8_t *)"->", WHITE, BLACK, 16, 0);
		LCD_ShowFloatNum1(140, 100, 00.00, 3, 2, GREEN, BLACK, 16);
		LCD_ShowString(196, 100, (uint8_t *)"rad", WHITE, BLACK, 16, 0);
		
		LCD_ShowFloatNum1(68, 120, 00.00, 3, 2, GREEN, BLACK, 16);
		LCD_ShowString(124, 120, (uint8_t *)"->", WHITE, BLACK, 16, 0);
		LCD_ShowFloatNum1(140, 120, 00.00, 3, 2, GREEN, BLACK, 16);
		LCD_ShowString(196, 120, (uint8_t *)"N/m", WHITE, BLACK, 16, 0);
		
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV5;
  RCC_OscInitStruct.PLL.PLLN = 68;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
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

/* USER CODE BEGIN 4 */

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

#ifdef  USE_FULL_ASSERT
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
