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
volatile int newDataFlag = 0;
volatile char g = 0;
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
void USART3_8_IRQHandler(){
	g = USART3->RDR;
	newDataFlag = 1;
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
RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
RCC->APB1ENR |=	RCC_APB1ENR_DACEN | RCC_APB1ENR_TIM2EN;
RCC->APB1ENR |= RCC_APB1ENR_USART3EN; // enable usart clock
NVIC_EnableIRQ(USART3_8_IRQn); /* (1) */
NVIC_SetPriority(USART3_8_IRQn,2);

TIM2->ARR = 20;
TIM2->PSC = 79;
TIM2->DIER |= (1);
TIM2->CR1 |= 1;

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

GPIOA->MODER |= (3 << 8);
DAC->SWTRIGR |= 1;
DAC->CR |= 1;

const int sawtooth_table[128] = {    2048, 2149, 2250, 2350, 2450, 2549, 2646, 2742, 2837, 2929, 3020, 3108, 3193, 3275, 3355,
    3431, 3504, 3574, 3639, 3701, 3759, 3812, 3861, 3906, 3946, 3982, 4013, 4039, 4060, 4076,
    4087, 4094, 4095, 4091, 4082, 4069, 4050, 4026, 3998, 3965, 3927, 3884, 3837, 3786, 3730,
    3671, 3607, 3539, 3468, 3394, 3316, 3235, 3151, 3064, 2975, 2883, 2790, 2695, 2598, 2500,
    2400, 2300, 2199, 2098, 1997, 1896, 1795, 1695, 1595, 1497, 1400, 1305, 1212, 1120, 1031,
    944, 860, 779, 701, 627, 556, 488, 424, 365, 309, 258, 211, 168, 130, 97,
    69, 45, 26, 13, 4, 0, 1, 8, 19, 35, 56, 82, 113, 149, 189,
    234, 283, 336, 394, 456, 521, 591, 664, 740, 820, 902, 987, 1075, 1166, 1258,
    1353, 1449, 1546, 1645, 1745, 1845, 1946, 2047};


int b = 0;
int w = 0;
	while(1){
		if(g == 'F'){
			if((TIM2->SR & 1) == 1){
				w++;
					{
					DAC->DHR12R1 = sawtooth_table[b]*2;
					 b++;
					 if (b == 128){
					 b=0;
					TIM2->SR = 0;
					 }
				 w = 0;
				}	
			}
		}
	}
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

