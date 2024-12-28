#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "adc.h"

#define GPIOCEN (1U<<2)
#define BTN_PIN (1U<<13)

static void adc_callback(void);
uint32_t sensor_value;
float volts;

int main(void)
{
	uart2_rxtx_init();
	pa1_adc_interrupt_init();

	RCC->AHB1ENR |= GPIOCEN;
	GPIOC->MODER &=~(1U<<26);
	GPIOC->MODER &=~(1U<<27);

	while(1)
	{
		if(!(GPIOC->IDR & BTN_PIN)){
			start_convertion();
			while(!(GPIOC->IDR & BTN_PIN)){}
		}
	}
}

static void adc_callback(void)
{
	for(int i=0;i<100000; i++){}
	sensor_value = ADC1->DR;
	printf("Sensor value : %.3f \n\r",(float)sensor_value);
	volts = sensor_value*3.3/4096;
	printf("Volts : %.3f \n\r\n\r", (float)volts);
}

void ADC_IRQHandler(void)
{
	if((ADC1->SR & SR_EOC) !=0)
	{
		ADC1->SR &=~SR_EOC;
		adc_callback();
	}
}
