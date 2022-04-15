/**
 * @author [Petr Oblouk]
 * @github [https://github.com/peoblouk]
 * @create date 07-04-2022 - 09:59:34
 * @modify date 07-04-2022 - 09:59:34
 * @desc [Use of timers blinking LED and displaying variable on segment display]
 */
#include "stm8s.h"

#define read1 GPIO_ReadInputPin(GPIOE, PIN_4)

uint16_t TIM4_pocet_int = 0;

uint8_t stav_now = 0;
uint8_t stav_past = 0;

uint8_t tim4_cnt_value = 0;

/* -------------------------------------------------------------------------------------------*/
// 7 segment display functions

//                   0    1    2    3    4    5    6    7    8    9    -
char numbers[11] = {192, 249, 164, 176, 153, 146, 130, 248, 128, 144, 255}; // numbers
char pin[4] = {PIN_7, PIN_6, PIN_5, PIN_4};                                 // array of digits
uint8_t dgt = 0;

// function to display number on segment
void segment_write(uint32_t display, uint8_t number_lenght)
{
    uint32_t temp1 = (display / 1000);       // thousands
    uint32_t temp2 = ((display / 100) % 10); // hundreds
    uint32_t temp3 = ((display / 10) % 10);  // tens
    uint32_t temp4 = (display % 10);         // unit

    char displayed_numbers[4] = {temp1, temp2, temp3, temp4}; // save variables to array

    for (uint8_t i = 0; i < number_lenght; i++)
    {
        GPIO_WriteLow(GPIOG, pin[i]);
        GPIO_Write(GPIOB, numbers[displayed_numbers[i]]);
        GPIO_WriteHigh(GPIOG, pin[i]);
        for (uint32_t i = 0; i < 1000; i++)
            ;
        GPIO_WriteLow(GPIOG, pin[i]);

        /*
                if (number_lenght >= 5) // write on DP
                {
                    temp4 = temp4 + 0b01111111;
                }*/
    }
}
/* -------------------------------------------------------------------------------------------*/
/* void delay(uint64_t time)
{

    if (TIM4_GetFlagStatus(TIM4_FLAG_UPDATE) == SET) // overfloat of counter register
    {
        TIM4_ClearFlag(TIM4_FLAG_UPDATE); // clear counter register
    }
} */

void main(void)
{
    GPIO_DeInit;
    TIM4_DeInit;
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV8);      // FREQ MCU 16MHz / 8 = 2 MHz
    GPIO_Init(GPIOC, PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW); // PC5

    // inicialization of timer
    TIM4_TimeBaseInit(TIM4_PRESCALER_128, 255); // Inicialization of timer
    TIM4_Cmd(ENABLE);                           // start of couter
    TIM4_ARRPreloadConfig(ENABLE);

    // inicialization of segment
    GPIO_Init(GPIOB, PIN_ALL, GPIO_MODE_OUT_PP_LOW_FAST); // PB port for A-G

    // Inicialization for pin for each digit
    GPIO_Init(GPIOG, PIN_4, GPIO_MODE_OUT_PP_LOW_FAST); // PG4 (D1) digit 1
    GPIO_Init(GPIOG, PIN_5, GPIO_MODE_OUT_PP_LOW_FAST); // PG5 (D2) digit 2
    GPIO_Init(GPIOG, PIN_6, GPIO_MODE_OUT_PP_LOW_FAST); // PG6 (D3) digit 3
    GPIO_Init(GPIOG, PIN_7, GPIO_MODE_OUT_PP_LOW_FAST); // PG7 (D4) digit 4
    //

    while (1)
    {
        // create delay
        if (TIM4_GetFlagStatus(TIM4_FLAG_UPDATE) == SET) // overfloat of counter register
        {
            TIM4_ClearFlag(TIM4_FLAG_UPDATE); // clear counter register
            GPIO_WriteReverse(GPIOC, PIN_5);
            tim4_cnt_value = TIM4_GetCounter(); // this function show number which are write to register

            if (TIM4_pocet_int == 9999)
            {
                TIM4_pocet_int = 0;
            }
        }

        if (read1 == RESET)
        {
            stav_now = 1;
        }
        else
        {
            stav_now = 0;
        }

        // change was unpressed
        if (stav_now == 1 && stav_past == 0)
        {
            TIM4_Cmd(DISABLE);
        }

        if (stav_now == 0 && stav_past == 1)
        {
            TIM4_Cmd(ENABLE);
        }

        segment_write(tim4_cnt_value, 4);
        stav_past = stav_now;
    }
}
