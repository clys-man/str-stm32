#include "stm32f4xx.h"
#include "keypad.h"


uint8_t keypad_init()
{

	RCC->AHB1ENR |= (1U<<0);	// Habilita o clock para acessar o GPIOA
	RCC->APB2ENR |= (1U<<8);	// Habilita o clock para acessar o ADC1

	// Configura o modo de PA0 para analógico
	GPIOA->MODER |= 0x3;

	/*** Configura o módulo ADC ***/
	ADC1->CR2 = 0;
	ADC1->SQR3 = 0;		// Inicia a sequência de conversão

	ADC1->SQR1 = 4;		// Configura o tamanho da sequência de conversão
	ADC1->CR2 |= 1;		// Habilita o módulo ADC

	return 0;
}

uint16_t keypad_read_key()
{
	uint16_t adc_readout = 0;

	ADC1->CR2 |= (1U<<30);	// Inicia a conversão
	while (!(ADC1->SR & 2)){}	// Espera o final da conversão
	adc_readout = ADC1->DR;		// Retorna o resultado

	if(adc_readout > 750 && adc_readout < 850)
	{
		return KEY_UP;
	}
	else if(adc_readout > 1800 && adc_readout < 2050)
	{
		return KEY_DOWN;
	}
	else if(adc_readout > 3000 && adc_readout < 3100)
	{
		return KEY_LEFT;
	}
	else if(adc_readout >= 0 && adc_readout < 50)
	{
		return KEY_RIGHT;
	}
	else if(adc_readout > 4000 && adc_readout < 5050)
	{
		return KEY_SELECT;
	}
	return KEY_NONE;




}
