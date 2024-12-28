#include "uart.h"

#define GPIOAEN         (1U<<0)
#define UART2EN         (1U<<17)

#define CR1_TE          (1U<<3)
#define CR1_RE          (1U<<2)

#define CR1_UE          (1U<<13)
#define SR_TXE          (1U<<7)
#define SR_RXNE         (1U<<5)

#define SYS_FREQ        16000000     /* Constante do clock da CPU */
#define APB1_CLK        SYS_FREQ

#define UART_BAUDRATE   115200

/* Declarações das funções protótipos */
static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);


void uart2_write(int ch);


int __io_putchar(int ch)
{
	uart2_write(ch);
	return ch;
}


void uart2_rxtx_init(void)
{
    /*************** Configura o pino GPIO para UART *****************/
    /* Habilita o clock para acessar GPIOA */
    RCC->AHB1ENR |= GPIOAEN;

    /* Configura o modo de PA2 para função alternativa */
    GPIOA->MODER &= ~(1U<<4);
    GPIOA->MODER |= (1U<<5);

    /* Configura PA2 para a função alternativa UART_TX (AF07) */
    GPIOA->AFR[0] |= (1U<<8);
    GPIOA->AFR[0] |= (1U<<9);
    GPIOA->AFR[0] |= (1U<<10);
    GPIOA->AFR[0] &= ~(1U<<11);

    /* Configura o modo de PA3 para função alternativa UART_RX (AF07) */
	GPIOA->MODER &= ~(1U<<6);
	GPIOA->MODER |= (1U<<7);

	/* Configura PA3 para a função alternativa UART_RX (AF07) */
	GPIOA->AFR[0] |= (1U<<12);
	GPIOA->AFR[0] |= (1U<<13);
	GPIOA->AFR[0] |= (1U<<14);
	GPIOA->AFR[0] &= ~(1U<<15);

    /*************** Configura o UART módulo *****************/
    /* Habilita o clock de acesso ao UART2 */
    RCC->APB1ENR |= UART2EN;

    /* Configura o baudrate */
    uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

    /*Habilita a transmissão serial */
    USART2->CR1 |= (CR1_TE | CR1_RE);

    // Habilita a uart2
    USART2->CR1 |= CR1_UE;
}

char uart2_read(void)
{
    /* Verifica se o buffer serial de recepção não está vazio */
    while(!(USART2->SR & SR_RXNE)){}

    // Ler o dado
    return USART2 -> DR;
}


void uart2_write(int ch)
{
    /*Testa se o buffer de transmissão está vazio */
    while (!(USART2->SR & SR_TXE)) {}

    /* Escreva dado no registrador de saída da uart2 (data register) */
    USART2->DR = (ch & 0xFF);
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
    USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
    return (PeriphClk + (BaudRate / 2U)) / BaudRate;
}


