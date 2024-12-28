#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart.h"

#define GPIOCEN         (1U<<2)      // Enable clock for GPIOC
#define GPIOC_10        (1U<<10)     // Pin 10 for GPIOC (LED)

#define SR_RXNE         (1U<<5)      // USART status register RXNE bit
#define LED_PIN_C       GPIOC_10     // Alias for LED on PC10


char key;

static void uart_callback(void);
void uart1_rxtx_init(void);

int main(void)
{
    // Enable clock for GPIOC
    RCC->AHB1ENR |= GPIOCEN;

    // Set PC10 as output for LED (MODER = 0x01)
    GPIOC->MODER |= (1U<<20);
    GPIOC->MODER &= ~(1U<<21);

    // Initialize UART communication (USART1)
    uart1_rxtx_init();
    while(1)
    {
    	// Toggle LED on PC10
        GPIOC->BSRR = LED_PIN_C;
        for(volatile int i=0; i<100000; i++) {}

        GPIOC->BSRR = (1U<<26);
        for(volatile int i=0; i<100000; i++) {}
    }
}

static void uart_callback(void)
{
    key = USART1->DR;
    GPIOC->ODR ^= LED_PIN_C;
    printf("tecla pressionada: %c \n\r", key);
}
// UART1 Interrupt Handler
void USART1_IRQHandler(void)
{
    if (USART1->SR & SR_RXNE)
    {
        uart_callback();
    }
}
