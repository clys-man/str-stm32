#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"

#define GPIOAEN         (1U<<0)    // Enable clock for GPIOA
#define UART1EN         (1U<<4)    // Enable clock for UART1

#define CR1_TE          (1U<<3)    // Enable Transmit
#define CR1_RE          (1U<<2)    // Enable Receive
#define CR1_UE          (1U<<13)   // Enable UART1

#define SR_TXE          (1U<<7)    // Transmit buffer empty
#define SR_RXNE         (1U<<5)    // Receive buffer not empty
#define CR1_RXNEIE      (1U<<5)    // Enable interrupt on RXNE

#define SYS_FREQ        16000000   // CPU clock frequency
#define APB2_CLK        SYS_FREQ

#define UART_BAUDRATE   9600       // Baud rate

/* Function Prototypes */
static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);
void uart1_tx_init(void);
void uart1_write(int ch);


int __io_putchar(int ch)
{
    uart1_write(ch);
    return ch;
}

void uart1_rxtx_init(void)
{
    /*************** Configure GPIO for UART *****************/
    // GPIOA (for PA9 and PA10)
    RCC->AHB1ENR |= GPIOAEN;

    // Configure PA9 (AF07) UART_TX
    GPIOA->MODER &= ~(1U<<18);
    GPIOA->MODER |= (1U<<19);


    GPIOA->AFR[1] |= (1U<<4);
    GPIOA->AFR[1] |= (1U<<5);
    GPIOA->AFR[1] |= (1U<<6);
    GPIOA->AFR[1] &= ~(1U<<7);


    // Configure PA10 (AF07) UART_RX
    GPIOA->MODER &= ~(1U<<20);
    GPIOA->MODER |= (1U<<21);

    GPIOA->AFR[1] |= (1U<<8);
    GPIOA->AFR[1] |= (1U<<9);
    GPIOA->AFR[1] |= (1U<<10);
    GPIOA->AFR[1] &= ~(1U<<11);


    /*************** Configure UART Module *****************/

    RCC->APB2ENR |= UART1EN;

    // Configure  baud rate
    uart_set_baudrate(USART1, APB2_CLK, UART_BAUDRATE);

    // Enable UART transmit and receive
    USART1->CR1 |= (CR1_TE | CR1_RE);

    // Enable RXNE interrupt
    USART1->CR1 |= CR1_RXNEIE;
    // Enable the UART1 interrupt in the NVIC
    NVIC_EnableIRQ(USART1_IRQn);
    // Enable UART1
    USART1->CR1 |= CR1_UE;
}
char uart1_read(void)
{
	/* Verifica se o buffer serial de recepção não está vazio */
    while(!(USART1->SR & SR_RXNE)) {}

    return USART1->DR;
}

void uart1_write(int ch)
{
    // Wait until TX buffer is empty
    while (!(USART1->SR & SR_TXE)) {}
    // Transmit the data
    USART1->DR = (ch & 0xFF);
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
    USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
    return (PeriphClk + (BaudRate / 2U)) / BaudRate;
}
