#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "adc.h"
#include "systick.h"

#define GPIOAEN           (1U<<0)
#define PIN5              (1U<<5)

#define LED               PIN5

static void systick_callback(void);

int main(void)
{
    /* Habilita o clock para acessar o GPIOA */
    RCC->AHB1ENR |= GPIOAEN;

    /* Configura PA5 como pino de saída - LED */
    GPIOA->MODER |=  (1U<<10);
    GPIOA->MODER &= ~(1U<<11);

    /* Inicializa o canal serial */
    uart2_rxtx_init();
    systick_1hz_interrupt();

    while(1)
    {

    }
}

static void systick_callback(void)
{
	printf("Passou 1 segundo !! \n\r");
	GPIOA->ODR ^= LED;
}

void SysTick_Handler(void)
{
	systick_callback();
}

