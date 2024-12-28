#include "stm32f4xx.h"
#include "stdint.h"

#define TIM2EN      (1U<<0)
#define CR1_CEN     (1U<<0)
#define OC_TOGGLE	((1U<<4) | (1U<<5))
#define CCER_CC1E	(1U<<0)

#define GPIOAEN		(1U<<0)
#define AFR5_TIM	(1U<<20)

void tim2_pa5_output_compare(void)
{
    /* Habilita o clock para acesso ao timer2 */
    RCC->AHB1ENR |= GPIOAEN;

    GPIOA->MODER &=~(1U<<10);
    GPIOA->MODER |=(1U<<11);

    GPIOA->AFR[0] |=AFR5_TIM;

    RCC->APB1ENR |=TIM2EN;


    TIM2->PSC = 1600 - 1;

    /* Configura o valor de recarga automática (auto-reload) */
    TIM2->ARR = 10000 - 1;  // 10.000 / 10.000 = 1

    TIM2->CCMR1 = OC_TOGGLE;

    TIM2->CCER |= CCER_CC1E;

    /* Limpa o contador */
    TIM2->CNT = 0;

    /* Habilita o timer */
    TIM2->CR1 = CR1_CEN;

}


