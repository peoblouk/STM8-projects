#include "stm8s.h"
#include "__assert__.h"

// proměnné pro LED
uint8_t pwm_led = 255; // úroveň AB
uint8_t citac_pwm = 0;

// makro LED
#define LED_ON GPIO_WriteHigh(GPIOB, GPIO_PIN_1)
#define LED_OFF GPIO_WriteLow(GPIOB, GPIO_PIN_1)

// makro tlačítko
#define Read_1 GPIO_ReadInputPin(GPIOE, GPIO_PIN_4)
#define Read_2 GPIO_ReadInputPin(GPIOG, GPIO_PIN_4)

void delay(uint32_t time)
{
    for (uint32_t i = 0; i < time; i++)
    {
        ;
    }
}

void main(void)
{
    GPIO_DeInit;                                   // deinicializace
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // FREQ MCU 16MHz

    // Inicializace LED
    GPIO_Init(GPIOB, GPIO_PIN_1, GPIO_MODE_OUT_PP_HIGH_FAST); // PB1

    // Inicializace tlačítek
    GPIO_Init(GPIOE, GPIO_PIN_4, GPIO_MODE_IN_PU_NO_IT); // PE4
    GPIO_Init(GPIOG, GPIO_PIN_5, GPIO_MODE_IN_FL_NO_IT); // PG5

    while (1)
    {
        // kontrola tlačítek
        if (Read_1 == RESET)
        {
            pwm_led = 10;
        }
        else if (Read_2 == RESET)
        {
            pwm_led = 180;
        }

        else
        {
            pwm_led = 255;
        }

        // stmívání
        if (pwm_led < citac_pwm)
        {
            LED_ON;
        }

        else
        {
            LED_OFF;
        }

        delay(10);
        citac_pwm++;
    }
}
