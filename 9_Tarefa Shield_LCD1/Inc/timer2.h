/*
 * timer2.h
 *
 *  Created on: Oct 7, 2024
 *      Author: alunoi4
 */

#ifndef TIMER2_H_
#define TIMER2_H_

#define SR_CC1IF (1U<<1)

void tim2_init(void);
void delay(uint16_t ms);
void delay_ms (uint16_t us);
void delayLCD (uint16_t us);

#endif /* TIMER2_H_ */
