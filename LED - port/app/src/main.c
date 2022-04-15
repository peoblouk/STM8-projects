#include "stm8s.h"
#include "__assert__.h"

#define LEDb GPIOB

// definice makra
// #define LEDB GPIOB

void main(void)
{
    GPIO_DeInit;
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // FREQ MCU 16MHz
    GPIO_Init(GPIOC, PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);

    // 8 LED inicializace
    GPIO_Init(LEDb, PIN_ALL, GPIO_MODE_OUT_PP_HIGH_SLOW);

    // rozsvítí z 8 ledek všechny a postupně od středu jde k poslední a znovu...
    /*    while (1)
        {
            for (uint32_t i = 0; i < 100000; i++)
            {
                GPIO_Write(GPIOB, 0b11111111);
            }

            for (uint32_t i = 0; i < 100000; i++)
            {
                GPIO_Write(GPIOB, 0b11100111);
            }

            for (uint32_t i = 0; i < 100000; i++)
            {
                GPIO_Write(GPIOB, 0b11000011);
            }

            for (uint32_t i = 0; i < 100000; i++)
            {
                GPIO_Write(GPIOB, 0b10000001);
            }

            for (uint32_t i = 0; i < 100000; i++)
            {
                GPIO_Write(GPIOB, 0b00000000);
            }
        } */
    /*
        // pomocí pole
        uint8_t DLED[] = {0b11111111, 0b11100111, 0b11000011, 0b10000001};
        while (1)
        {
            for (uint8_t x = 0; x > 4; x++)
            {
                GPIO_Write(GPIOB, DLED[x]);
                for (uint32_t i = 0; i < 100000; i++);
            }
        }
    */

    for (uint32_t i = 0; i < 100000; i++)
    {
        GPIO_Write(LEDb, 0b11000011);
    }
}