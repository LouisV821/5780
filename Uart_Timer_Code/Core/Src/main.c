/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

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
void	Transmit_Char(char);
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

void Transmit_Char(char x){
	while(!(USART3->ISR & (1 << 7)));
	USART3->TDR = x;
}
void EXTI0_1_IRQHandler(){
	Transmit_Char('N');
	GPIOC->ODR &= ~(1<<6);
	EXTI->PR = 1;
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	HAL_Init();
	SystemClock_Config();
	
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOAEN; // enable gpioc clock
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN; // enable usart clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	NVIC_EnableIRQ(EXTI0_1_IRQn); /* (1) */
	NVIC_SetPriority(EXTI0_1_IRQn,1);
	
	GPIOA->PUPDR |= 2;
	SYSCFG->EXTICR[0] = 0;
	EXTI->IMR |= 1;
	EXTI->RTSR |= 1;
	
	GPIOC->MODER |= (1 << 12); // Enable Red LED
	GPIOC->MODER |= (1 << 14); // Enable Blue LED
	GPIOC->MODER |= (1 << 16); // Enable Orange LED
	GPIOC->MODER |= (1 << 18); // Enable Green LED
	
	GPIOC->MODER |= (1 << 9); // Moder 4 set to alternate function mode
	GPIOC->MODER |= (1 << 11); // Moder 5 set to alternate function mode
	
	GPIOC->AFR[0] |= (1 << 16);  // Enable alternate function A1 for PC4
	GPIOC->AFR[0] |= (1 << 20); // Enable alternate function A1 for PC5
	
	USART3->BRR = HAL_RCC_GetHCLKFreq()/115200; // baud rate of 115200
	USART3->CR1 |= USART_CR1_RE | USART_CR1_TE | (1<<5);	// set active transmit and recieve bits for usart 3
	USART3->CR1 |= USART_CR1_UE; // enable usart3

  while (1)
	{
		if(GPIOC->ODR & 1<<6){
			 Transmit_Char('F');
			 GPIOC->ODR = 0;
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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

