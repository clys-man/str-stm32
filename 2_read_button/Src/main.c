#include "stm32f4xx.h"


#define GPIOAEN         (1U<<0)
#define GPIOCEN         (1U<<2)

#define PIN5            (1U<<5)
#define PIN13            (1U<<13)

#define LED_PIN			PIN5
#define BTN_PIN			PIN13

int main(void) {
    // Habilita o clock para GPIOA
    RCC->AHB1ENR |= GPIOAEN;
    RCC->AHB1ENR |= GPIOCEN;

    // Configura o pino 5 como saída
    GPIOA->MODER |= (1U<<10);
    GPIOA->MODER &=~(1U<<11);

    GPIOA->MODER |= (1U<<26);
    GPIOA->MODER &=~(1U<<27);

    while(1) {

    	if(GPIOC -> IDR & BTN_PIN){
    		// Liga o Led
    		GPIOA-> BSRR = (1U<<21);

    	}else{
    		// Desliga o Led
    		GPIOA-> BSRR = LED_PIN;
    	}
    }
}


