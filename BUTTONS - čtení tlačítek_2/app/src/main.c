#include "stm8s.h"
#include "__assert__.h"

// piny led
#define led GPIOB, PIN_4
char b = PIN_2;

// piny tlačitko
char c = PIN_4;
char d = PIN_5;
// proměnné
uint8_t aktualnistav1 = 0;
uint8_t minulystav1 = 0;

uint8_t aktualnistav2 = 0;
uint8_t minulystav2 = 0;

void main(void)
{
    GPIO_DeInit;
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // FREQ MCU 16MHz
    // incializace tlačítek
    GPIO_Init(GPIOG, c, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(GPIOG, d, GPIO_MODE_IN_FL_NO_IT);

    // 2 LED inicializace
    GPIO_Init(led, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(GPIOB, b, GPIO_MODE_OUT_PP_HIGH_SLOW);

    while (1)
    {
        // kontrola tlačítka
        if (GPIO_ReadInputPin(GPIOG, c) == RESET)
        {
            aktualnistav1 = 1;
        }
        else
        {
            aktualnistav1 = 0;
        }
        // zaregistrována změna tlačitko 1
        if ((aktualnistav1 = 1) && (minulystav1 = 0))
        {
            GPIO_WriteHigh(GPIOB, PIN_2);
        }
        if ((aktualnistav1 = 0) && (minulystav1 = 1))
        {
            GPIO_WriteLow(GPIOB, PIN_2);
        }
        minulystav1 = aktualnistav1;

        // kontrola tlačítka 2
        if (GPIO_ReadInputPin(GPIOG, d) == RESET)
        {
            aktualnistav2 = 1;
        }
        else
        {
            aktualnistav2 = 0;
        }
        // zaregistrována změna tlačitko 2
        if ((aktualnistav2 = 1) && (minulystav2 = 0))
        {
            GPIO_WriteHigh(GPIOB, PIN_1);
        }
        if ((aktualnistav2 = 0) && (minulystav2 = 1))
        {
            GPIO_WriteLow(GPIOB, PIN_1);
        }
        minulystav2 = aktualnistav2;
    }
}