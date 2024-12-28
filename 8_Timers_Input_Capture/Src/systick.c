#include <systick.h>
#include "stm32f4xx.h"


#include "stm32f4xx.h"

#define SYSTICK_LOAD_VAL     16000 - 1
#define CTRL_ENABLE          (1U<<0)
#define CTRL_CLKSRC          (1U<<2)
#define CTRL_COUNTFLAG       (1U<<16)
#define CTRL_TICKINT		 (1U<<1)

#define ONE_SEC_LOAD 		16000000

void systick_1hz_interrupt(void)
{
    /* Recarrega com número de clocks por milissegundo */
    SysTick->LOAD = ONE_SEC_LOAD - 1;

    /* Zera o valor atual do registrador SysTick */
    SysTick->VAL = 0;

    /* Habilita o systick e seleciona a fonte de clock interno */
    SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC;


    SysTick->CTRL |= CTRL_TICKINT;
}






