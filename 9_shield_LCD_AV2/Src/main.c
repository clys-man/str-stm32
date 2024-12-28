#include "stm32f4xx.h"
#include "LCD1602.h"
#include "keypad.h"
#include "stdio.h"
#include "tim.h"
#include "uart.h"

int row = 0;
int col = 0;

int main(void)
{
    uint16_t key_val = 0;
    static char* key_name[] = {"RIGHT", "UP  ", "DOWN", "LEFT", "SELECT", "NONE   "};


    GPIO_init();
    tim2_init();

    keypad_init();

    lcd_init();
    uart2_rxtx_init();  // Inicializa UART para leitura e escrita

    // Mensagem de inicialização no LCD
    lcd_put_cur(0, 0);
    lcd_send_string("SIST. TEMPO REAL");

    lcd_put_cur(1, 0);
    lcd_send_string("* U N I F O R *");
    delay(3000);



    int bounce = 0;

    while (1)
    {
        // Leitura da tecla
        key_val = keypad_read_key();

        if ((key_val == 4) & (bounce == 0))
        {
            bounce++;
        }
        else
        {
            lcd_put_cur(1, 0);
            lcd_send_string(key_name[key_val]);
            delay(300);
            bounce = 0;
        }


    }
}

void USART2_IRQHandler(void) {
    if (USART2->SR & SR_RXNE) { // Verifica se o buffer de recepção não está vazio
        char received_char = (char)(USART2->DR & 0xFF); // Lê o dado
        display_serial_char_on_lcd(received_char); // Exibe no LCD
    }
}
