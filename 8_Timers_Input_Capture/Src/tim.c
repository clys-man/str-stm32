#include "stm32f4xx.h"
#include "stdint.h"

#define TIM2EN      (1U<<0)
#define TIM3EN 		(1U<<1)

#define CR1_CEN     (1U<<0)
#define OC_TOGGLE	((1U<<4) | (1U<<5))
#define CCER_CC1E	(1U<<0)

#define GPIOAEN		(1U<<0)
#define AFR5_TIM	(1U<<20)
#define AFR6_TIM	(1U<<25)
#define CCER_CC1S	(1U<<0)

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


void tim3_pa6_input_capture(void)
{
    /* Habilita o clock para acessar o GPIOA */
    RCC->AHB1ENR |= GPIOAEN;

    /* Configura PA6 para o modo alternate function */
    GPIOA->MODER &= ~(1U<<12);
    GPIOA->MODER |= (1U<<13);

    /* Configura PA6 alternate function tipo TIM2_CH1 (AF01) */
    GPIOA->AFR[0] = AFR6_TIM;

    /* Habilita o clock para acessar o tim3 */
    RCC->APB1ENR |= TIM3EN;

    /* Configura o Prescaler*/
    TIM3->PSC = 16000 - 1; // 16 000 000 /16 000 = 1.000

    /* Configura o CH1 para captura de entrada - input capture*/
    TIM3->CCMR1 = CCER_CC1S;

    /* Configura o CH1 para captura na borda de subida */
    TIM3->CCER = CCER_CC1E;

    /* Habilita o TIM3*/
    TIM3->CR1 = CR1_CEN;
}
