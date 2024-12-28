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

#include "semphr.h"
#include "task.h"


UART_HandleTypeDef huart2;
SemaphoreHandle_t xBinarySemaphore;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
//void MX_FREERTOS_Init(void);

//static void MX_GPIO_Init(void);
//static void MX_USART2_UART_Init(void);


void StartNormalTask(void *pvParameters);
void StartHighTask(void *pvParameters);
void StartLowTask(void *pvParameters);

#define LED GPIO_PIN_5
int counter1, counter2;

int main(void)
{


  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_USART2_UART_Init();

  //MX_FREERTOS_Init();

  xBinarySemaphore = xSemaphoreCreateBinary();

  /* Função que cria uma tarefa para controle do LED */
  xTaskCreate(StartNormalTask, "tarefa normal", 100, NULL, 2, NULL);

 /* Função que cria uma tarefa para controle da Serial */
  xTaskCreate(StartHighTask, "tarefa alta", 100, NULL, 3, NULL);

  xTaskCreate(StartLowTask, "tarefa low", 100, NULL, 1, NULL);


  xSemaphoreGive(xBinarySemaphore);

  vTaskStartScheduler(); /* Inicia o escalonador de tarefas - Scheduler */



  while (1)
  {

  }
  return 0;
}



/* Tarefa de prioridade normal */
void StartNormalTask(void * pvParameters)
{
    /* loop infinito para tarefa n */
    while (1)
    {
        printf("Entrando na tarefa normal\n");
        printf("Saindo da tarefa normal\n\n");
        vTaskDelay(1000);
    }
}

/* Tarefa de prioridade alta */
void StartHighTask(void * pvParameters)
{
    /* loop infinito para tarefa alta */
    while (1)
    {
        printf("Entrando na tarefa alta e esperando Semaforo\n");
        xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
        printf("Semaforo adquirido pela tarefa alta\n");

        printf("Saindo da tarefa alta e liberando Semaforo\n\n");
        xSemaphoreGive(xBinarySemaphore);
        vTaskDelay(1000);
    }
}

/* Tarefa de prioridade baixa */
void StartLowTask(void * pvParameters)
{
    /* loop infinito para tarefa baixa */
    while (1)
    {
        printf("Entrando na tarefa baixa e esperando Semaforo\n");
        xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
        printf("Semaforo adquirido pela tarefa baixa\n");

        // Espera até o pino baixar
        while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13));

        vTaskDelay(1000);
        printf("Saindo da tarefa baixa e liberando Semaforo\n\n");
        xSemaphoreGive(xBinarySemaphore);
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
