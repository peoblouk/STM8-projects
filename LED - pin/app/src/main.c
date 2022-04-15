#include "stm8s.h"
#include "__assert__.h"

// definice makra
// #define LEDB GPIOB

void main(void)
{
    GPIO_DeInit;
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // FREQ MCU 16MHz
    GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);

    // 8 LED inicializace
    GPIO_Init(GPIOB, GPIO_PIN_ALL, GPIO_MODE_OUT_PP_HIGH_SLOW);

    while (1)
    {
        /*GPIO_WriteReverse(GPIOC, GPIO_PIN_5);
        for (uint16_t i = 0; i < 10000; i++)
        GPIO_WriteHigh(GPIOB, GPIO_PIN_ALL);
        for (uint32_t i = 0; i < 500000; i++)
            ;
        GPIO_WriteLow(GPIOB, GPIO_PIN_ALL);
        for (uint32_t i = 0; i < 500000; i++)
            ;*/
        // 8 LED bliká od začatku po konec
        for (uint8_t p = 0; p < 7; p++)
        {
            GPIO_Write(GPIOB, (129 >> p));
            for (uint32_t i = 0; i < 100000; i++)
                ;
            GPIO_Write(GPIOB, (0 << p));
            for (uint32_t i = 0; i < 100000; i++)
                ;
        }

        // GPIO_Write(GPIOB, ~(1 << 7));
        // for (uint32_t i = 0; i < 500000; i++);
    }
}