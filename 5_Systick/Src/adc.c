#include "stm32f4xx.h"
#include "adc.h"

#define GPIOAEN     (1U<<0)
#define ADC1EN      (1U<<8)
#define ADC_CH1     (1U<<0)
#define ADC_SEQ_LEN_1 0x00
#define CR2_ADON    (1U<<0)
#define CR2_SWSTART (1U<<30)
#define SR_EOC      (1U<<1)
#define CR2_CONT	(1U<<1)
#define CR1_EOCIE	(1U<<5)


void pa1_adc_interrupt_init(void)
{
    /*** Configura o pino GPIO do ADC ***/

    // Habilita o clock para acessar o GPIOA
    RCC->AHB1ENR |= GPIOAEN;

    // Configura o modo de PA1 para analógico
    GPIOA->MODER |= (1U<<2);
    GPIOA->MODER |= (1U<<3);

    /*** Configura o módulo ADC ***/

    // Habilita o clock para acessar o ADC1
    RCC->APB2ENR |= ADC1EN;

    ADC1->CR1 |= CR1_EOCIE;

    NVIC_EnableIRQ(ADC_IRQn);

    // Inicia a sequência de conversão
    ADC1->SQR3 = ADC_CH1;

    // Configura o tamanho da sequência de conversão
    ADC1->SQR1 = ADC_SEQ_LEN_1;

    // Habilita o módulo ADC
    ADC1->CR2 |= CR2_ADON;
}

void start_convertion(void)
{
	//Habilita a conversão continua
	ADC1->CR2 &= ~CR2_CONT;
	// Inicia a conversão AD
	ADC1->CR2 |= CR2_SWSTART;
}

uint32_t adc_read(void)
{
	//Espera a conversão ter terminada
	while(!(ADC1->SR & SR_EOC)){}

	return (ADC1->DR);
}

