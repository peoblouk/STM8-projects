#include "stm8s.h"
#include "__assert__.h"

/*
void main(void)
{
    GPIO_DeInit;
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // FREQ MCU 16MHz
    GPIO_Init(GPIOC, PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);

    4 LED inicializace
    GPIO_Init(GPIOB, PIN_0, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(GPIOB, PIN_1, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(GPIOB, PIN_2, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(GPIOB, PIN_3, GPIO_MODE_OUT_PP_HIGH_SLOW);

    while (1)
    {
        GPIO_WriteReverse(GPIOC, PIN_5);

        // blikání 4 ledek na GPIOC
        GPIO_WriteReverse(GPIOB, PIN_0);
        GPIO_WriteReverse(GPIOB, PIN_1);
        GPIO_WriteReverse(GPIOB, PIN_2);
        GPIO_WriteReverse(GPIOB, PIN_3);

        // BLIKÁNÍ BUILDIN LED
        for (uint32_t i = 0; i < 100000; i++)
        {
            /*
            GPIO_WriteLow(GPIOB, PIN_0);
            GPIO_WriteLow(GPIOB, PIN_1);
            GPIO_WriteLow(GPIOB, PIN_2);
            GPIO_WriteLow(GPIOB, PIN_3);

            GPIO_WriteHigh(GPIOB, PIN_0);
            GPIO_WriteHigh(GPIOB, PIN_1);
            GPIO_WriteHigh(GPIOB, PIN_2);
            GPIO_WriteHigh(GPIOB, PIN_3);
        }
    }
}  */

void main(void)
{
    GPIO_DeInit;
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // FREQ MCU 16MHz
    GPIO_Init(GPIOC, PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);
}