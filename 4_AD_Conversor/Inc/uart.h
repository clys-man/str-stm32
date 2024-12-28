/*
 * uart.h
 *
 *  Created on: Sep 4, 2024
 *      Author: alunoi4
 */

#ifndef UART_H_
#define UART_H_
#include <stdint.h>

#include "stm32f4xx.h"

char uart2_read(void);
void uart2_rxtx_init(void);


#endif /* UART_H_ */
