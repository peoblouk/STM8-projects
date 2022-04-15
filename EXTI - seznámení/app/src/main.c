#include "stm8s.h"
#include "__assert__.h"

#define read1 GPIO_ReadInputPin(GPIOE, PIN_4) // read button

//                   0    1    2    3    4    5    6    7    8    9    a    b    c    d
char numbers[14] = {192, 249, 164, 176, 153, 146, 130, 248, 128, 144, 163, 131, 167, 161}; // numbers
char pin[4] = {PIN_4, PIN_5, PIN_6, PIN_7};                                                // digit
uint8_t x = 0;

INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
    /*GPIO_Write(GPIOB, 0x86)
    for (uint32_t c=0, c<1000000, c++)
    */
    x += 1;
    if (x == 14)
    {
        x = 0;
    }
}

// functions
void digit(bool value, char pin)
{
    if (value == TRUE)
    {
        GPIO_WriteHigh(GPIOG, pin); // on digit
    }

    else
    {
        GPIO_WriteLow(GPIOG, pin); // off digit
    }
}

void main(void)
{
    GPIO_DeInit;
    EXTI_DeInit;
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);        // FREQ MCU 16MHz
    GPIO_Init(GPIOB, PIN_ALL, GPIO_MODE_OUT_PP_LOW_SLOW); // inicialization
    GPIO_Init(GPIOE, PIN_4, GPIO_MODE_IN_FL_NO_IT);       // IT x NO_IT // button PE4
    GPIO_Init(GPIOD, PIN_4, GPIO_MODE_IN_FL_IT);          // PD4

    // INTERRUPT
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_FALL_ONLY);
    ITC_SetSoftwarePriority(ITC_IRQ_PORTD, ITC_PRIORITYLEVEL_0);
    enableInterrupts();

    uint8_t akt_stav = 0;
    uint8_t min_stav = 0;

    while (1)
    {
        // digit(TRUE, 1);
        GPIO_WriteHigh(GPIOG, PIN_5); // on digit
        GPIO_Write(GPIOB, numbers[x]);

        if (read1 == RESET)
        {
            akt_stav = 1;
        }
        else
        {
            akt_stav = 0;
        }
        // change
        if (akt_stav == 1 && min_stav == 0)
        {
            x += 1;
            if (x == 10)
            {
                x = 0;
            }
        }
        min_stav = akt_stav;
    }
}