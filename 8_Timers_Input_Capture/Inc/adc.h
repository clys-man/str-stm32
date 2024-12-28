/*
 * adc.h
 *
 *  Created on: Sep 4, 2024
 *      Author: alunoi4
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>


void pa1_adc_interrupt_init(void);
uint32_t adc_read(void);
void start_convertion(void);

#define SR_EOC      (1U<<1)


#endif /* ADC_H_ */
