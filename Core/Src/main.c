/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "crc.h"
#include "dac.h"
#include "dma2d.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "LibPerso.h"
#include "shell.h"

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
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

SemaphoreHandle_t MonSem, MonSemUART;
char QMessage[20];
QueueHandle_t  BaL1;
//char prompt[] = " > ";



//Redéfinition de la fonction printf ... qui utilise io_putchar()
int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
	return ch;
}


/////////////////////////////////////////////////////////////////////////////////////////
// FCT DISPLAY SPINNING WHEEL
// Handy function to show user the app is running (and not stuck)
//
// @PARAM: none
//
// @RETURN: none
//
/////////////////////////////////////////////////////////////////////////////////////////
void displaySpiningWheel(void){
	printf("T1: BONJOUR\r\n");
	char i = 1;
	while (1)
	{
		switch (i)
			{
			case 1:
				//printf("|\033[1D>");
				sprintf(prompt,"\r|>");
				printf("%s",prompt);
//				printf("\r|>");
				fflush(stdout);
				break;
			case 2:
				//printf("/\033[1D>");
				sprintf(prompt,"\r/>");
				printf("%s",prompt);
				fflush(stdout);
				break;
			case 3:
				//printf("-\033[1D>");
				sprintf(prompt,"\r->");
				printf("%s",prompt);
				fflush(stdout);
				break;
			case 4:
				//printf("\\\033[1D>");
				sprintf(prompt,"\r\\>");
				printf("%s",prompt);
				fflush(stdout);
				break;
			default:
				i = 1;
			}
	i = ++i > 4  ? 1 : i ;
	vTaskDelay(100);
	}
	vTaskDelete(NULL);
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
uint8_t bidon = 0x58;
uint8_t message[] = {"On va commencer\r\n"};
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
  MX_CRC_Init();
  MX_DMA2D_Init();
  MX_FMC_Init();
  MX_I2C3_Init();
  MX_SPI5_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_USB_OTG_HS_HCD_Init();
  MX_TIM2_Init();
  MX_ADC1_Init();
  MX_DAC_Init();
  /* USER CODE BEGIN 2 */

  //HAL_UART_Transmit(&huart1, message, sizeof(message), 0xff);
  printf("T0: Tache INIT commence\r\n");


  BaseType_t xReturned;
  TaskHandle_t xHandle = NULL;
  int p1=1;
  int p2=2;

  //Création d'un Meesage/Qeue
  //BaL1 = xQueueCreate( 5, sizeof( QMessage ) );

  //Création du Semaphore
  printf("T0: Creation du semaphore (vide)\r\n");
  //MonSem=semBCreate(SEM_Q_FIFO,SEM_EMPTY);		//Façon VxWorks
  MonSem = xSemaphoreCreateBinary();				//Façon FreeRTOS
  MonSemUART = xSemaphoreCreateBinary();				//Façon FreeRTOS
  //shell_init();


  /* Create the task, storing the handle. */

  /* ------------------------------ CREATION desw TACHES ------------------------------------------
   * ==============================================================================================
   printf("T0: Creation tache 1\r\n");
  xReturned = xTaskCreate(
				  (void*)displaySpiningWheel,
				  "SpiningWheel",
				  1000,      		/* Stack size in words, not bytes.
				  NULL,    			/* Parameter passed into the task.
				  p1,				/* Priority at which the task is created.
				  &xHandle );      /* Used to pass out the created task's handle.
  if( xReturned == pdPASS )
  printf("T0: Tache ''Bidon'' cree avec priorite %d\r\n", p1);
*/

  printf("T0: Creation tache 2\r\n");
  xReturned = xTaskCreate(
		  	  	  (void*)Test,				/* RENSEINGER ICI la FONCTION A LANCER ....*/
				  "Test",
		  	  	  1000,      		/* Stack size in words, not bytes.*/
				  NULL,    			/* Parameter passed into the task.*/
				  p2,				/* Priority at which the task is created.*/
				  &xHandle );       /* Used to pass out the created task's handle.*/
  if( xReturned == pdPASS )
	  printf("T0: Tache ''test'' cree avec priorite %d\r\n", p2);

  printf("T0: Fin fct main x_x\r\n\n");

   /* ==============================================================================================
  -----------------------------------------------------------------------------------------------*/

  //Initier le timer TIM2
  HAL_TIM_Base_Start_IT (&htim2);

  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init(); 
  /* Start scheduler */
  osKernelStart();
 
  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  //..
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
