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
#include <string.h>

#include "LCD1602.h"
#include "tim.h"
#include "uart.h"
#include "keypad.h"

#include "queue.h"

#define LED GPIO_PIN_5


SemaphoreHandle_t xBinarySemaphore;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);



void keypad(void *pvParameters);
void UART(void *pvParameters);
void conta(void *pvParameters);
void toggleLedTask(void *pvParameters);



//QueueHandle_t yearQueue;


int cont = 0;

int main(void)
{
	//HAL_Init();

	MX_USART2_UART_Init();


	MX_FREERTOS_Init();


	GPIO_init();
	tim2_init();

	keypad_init();

	lcd_init();



	lcd_put_cur(0, 0);
	lcd_send_string("SIST. TEMPO REAL");

	lcd_put_cur(1, 0);
	lcd_send_string("* U N I F O R *");
	delay(3000);

    lcd_clear();

	lcd_put_cur(0, 0);
	lcd_send_string("TENSAO=");
	lcd_put_cur(0, 10);
	lcd_send_string("v");

	lcd_put_cur(1, 0);
	lcd_send_string("CONT=");

	lcd_put_cur(1, 10);
	lcd_send_string("UNIFOR");


	xBinarySemaphore = xSemaphoreCreateBinary();

  xTaskCreate(keypad,"Ler_teclas",128,NULL,osPriorityNormal,NULL);
  xTaskCreate(conta,"contar_up",128,NULL,osPriorityNormal,NULL);
  xTaskCreate(toggleLedTask, "Toggle LED", 128, NULL, 1, NULL);

  //xTaskCreate(UART,"uart",128,NULL,osPriorityNormal,NULL);

  xSemaphoreGive(xBinarySemaphore);


  vTaskStartScheduler();


  while (1)
  {


  }
  return 0;
}

void toggleLedTask(void *pvParameters)
{
    while (1)
    {
    	xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
    	GPIOA-> BSRR = (1U<<21);
        delay(500);
        GPIOA-> BSRR = LED;
        delay(500);
        xSemaphoreGive(xBinarySemaphore);
    }
}


void keypad(void * pvParameters)
{
	int bounce = 0;
    uint16_t key_val = 0;
    static char* key_name[] = {"RIGHT", "UP  ", "DOWN", "LEFT", "SELEC", "NONE "};

    while (1)
    {
    	xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
        // Leitura da tecla
        key_val = keypad_read_key();
        int sensor_value = ADC1->DR;
        float volts = (sensor_value*3.3)/4096;


        if ((key_val == 4) & (bounce == 0))
        {
            bounce++;
        }
        else
        {
            lcd_put_cur(0, 11);
            lcd_send_string(key_name[key_val]);

            char str[4];
			sprintf(str,"%.1f", volts);

			lcd_put_cur(0, 7);
			lcd_send_string(str);

	    	uint8_t txt1[14];

	    	sprintf((char*)txt1, "TENSAO=%.2fV\n", volts);
	    	HAL_UART_Transmit(&huart2, txt1, sizeof(txt1), 100);


            while(key_val==1){
				key_val = keypad_read_key();
				if(key_val!=1){
					cont++;
					break;
				}
            }
            bounce = 0;
            xSemaphoreGive(xBinarySemaphore);
			delay(500);


        }



    }
}


void UART(void * pvParameters)
{
    char receivedString[7];  // Buffer para string recebida (máximo 6 caracteres + '\0')

    while (1)
    {
    	xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
        // Recebe string pela UART (máximo 6 caracteres)
        if (HAL_UART_Receive(&huart2, (uint8_t *)receivedString, 6, HAL_MAX_DELAY) == HAL_OK)
        {

            lcd_put_cur(1, 10);

            lcd_send_string(receivedString);
        }

        xSemaphoreGive(xBinarySemaphore);
        delay(300);
    }
}


void conta(void *pvParameters)
{

	    while (1)
	    {
	    	xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);

			char num[4];
			sprintf(num,"%d", cont);

			uint8_t txt2[15];

			sprintf((char*)txt2, "CONTADOR=%d\n", cont);
			HAL_UART_Transmit(&huart2, txt2, sizeof(txt2), 100);

			lcd_put_cur(1, 5);
			lcd_send_string(num);

			xSemaphoreGive(xBinarySemaphore);
			delay(200);

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
