/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "gpio.h"
#include <stdio.h>

#include "queue.h"

#define LED GPIO_PIN_5

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
//void MX_FREERTOS_Init(void);

//static void MX_GPIO_Init(void);
//static void MX_USART2_UART_Init(void);


void SenderTask1(void *pvParameters);
void SenderTask2(void *pvParameters);
void ReceiverTask(void *pvParameters);


QueueHandle_t yearQueue;

int main(void)
{


  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  yearQueue = xQueueCreate(5,sizeof(int32_t));

  /* Função que cria uma tarefa para controle do LED */
  xTaskCreate(ReceiverTask,"Tarefa_Receptor",128,NULL,osPriorityAboveNormal,NULL);

  xTaskCreate(SenderTask1,"Tarefa_Transmissor1",128,NULL,osPriorityNormal,NULL);

  xTaskCreate(SenderTask2,"Tarefa_Transmissor2",128,NULL,osPriorityNormal,NULL);


  vTaskStartScheduler(); /* Inicia o escalonador de tarefas - Scheduler */



  while (1)
  {

  }
  return 0;
}




void SenderTask1(void * pvParameters)
{

	int32_t value_to_send = 2222;
	BaseType_t qStatus;

    while (1)
    {
		qStatus = xQueueSend(yearQueue,&value_to_send,0);
		if (qStatus != pdPASS)
		{
			uint8_t txt1[]="Erro: dado da tarefa 1 nao pode ser enviado\n";
			HAL_UART_Transmit(&huart2, txt1, sizeof(txt1), 100);
		}
		osDelay(500);
    }
}

void SenderTask2(void * pvParameters)
{

	int32_t value_to_send = 5555;
	BaseType_t qStatus;

    while (1)
    {
		qStatus = xQueueSend(yearQueue,&value_to_send,0);
		if (qStatus != pdPASS)
		{
			uint8_t txt1[]="Erro: dado da tarefa 2 nao pode ser enviado\n";
			HAL_UART_Transmit(&huart2, txt1, sizeof(txt1), 100);
		}
		osDelay(500);
    }
}


void ReceiverTask(void * pvParameters)
{

	int value_received;
	const TickType_t wait_time = pdMS_TO_TICKS(800);

	BaseType_t qStatus;
	uint8_t txt1[28];

    while (1)
    {
        qStatus = xQueueReceive(yearQueue, &value_received, wait_time);
        if(qStatus == pdPASS)
        {
        	sprintf((char*)txt1, "o valor recebido foi: %d\n", value_received);
        	HAL_UART_Transmit(&huart2, txt1, sizeof(txt1), 100);
        }
        else
        {
        	uint8_t txt1[]="Erro, nao foi possivel receber\n";
        	HAL_UART_Transmit(&huart2, txt1, sizeof(txt1), 100);
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }

}


void Error_Handler(void)
{

  __disable_irq();
  while (1)
  {
  }

}




#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif
