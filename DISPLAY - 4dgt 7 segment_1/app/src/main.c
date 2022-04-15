/**
 * @author [Petr Oblouk]
 * @github [https://github.com/peoblouk]
 * @create date 02-04-2022 - 12:40:50
 * @modify date 03-04-2022 - 21:12:10
 * @desc [Display on 7 segment]
 */

#include "stm8s.h"
#include "__assert__.h"
// #include "7_segment.h"

// #define read1 GPIO_ReadInputPin(GPIOE, PIN_4) // read button on PE4

// variables for read button
uint8_t aktualni_stav = 0;
uint8_t minuly_stav = 0;

//                   0    1    2    3    4    5    6    7    8    9    -
char numbers[11] = {192, 249, 164, 176, 153, 146, 130, 248, 128, 144, 255}; // numbers
char pin[4] = {PIN_7, PIN_6, PIN_5, PIN_4};                                 // array of digits
uint8_t dgt = 0;

// letters
char letters[12] = {
    163, // a
    131, // b
    167, // c
    161, // d

    6,   // E
    14,  // F
    11,  // h
    27,  // n
    243, // o
    12,  // P
    175, // r
    255, // -
};

// write on one digit
void digit(bool value, uint8_t dgt) // functions for control each digit
{
    if (value == TRUE)
    {
        GPIO_WriteHigh(GPIOG, pin[dgt]); // on digit
    }

    else
    {
        GPIO_WriteLow(GPIOG, pin[dgt]); // off digit
    }
}

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

/*
char keyboard[4][4] = {
    1, 2, 3, A,
    4, 5, 6, B,
    7, 8, 9, C,
                 *, 0, #, D};

char keyboard_2[4][4] = {
    numbers[1].numbers[2], numbers[3], letters[0],
    numbers[4], numbers[5], numbers[6], letters[1],
    numbers[7], numbers[8], numbers[9], letters[2],
    numbers[10], numbers[0], numbers[10], letters[3]};

// inicialization of segment
char port_select[] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOI};
char pin_select[4] = {PIN_7, PIN_6, PIN_5, PIN_4};
uint8_t port_sel = 0;

void segment_init(char port_select[port_sel], uint8_t pin_select[])
{
    GPIO_Init(port_select[port_sel], pin_select);
}

*/

void delay(uint32_t time) // function for delay
{
    time = time * 5555;
    for (uint32_t i = 0; i < time; i++)
    {
        ;
    }
}

void main(void)
{
    GPIO_DeInit;                                   // deinicializace
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // FREQ MCU 16MHz

    GPIO_Init(GPIOB, PIN_ALL, GPIO_MODE_OUT_PP_LOW_FAST); // PB port for A-G
    GPIO_Init(GPIOE, PIN_4, GPIO_MODE_IN_FL_NO_IT);       // PE4

    // Inicialization for pin for each digit
    GPIO_Init(GPIOG, PIN_4, GPIO_MODE_OUT_PP_LOW_FAST); // PG4 (D1) digit 1
    GPIO_Init(GPIOG, PIN_5, GPIO_MODE_OUT_PP_LOW_FAST); // PG5 (D2) digit 2
    GPIO_Init(GPIOG, PIN_6, GPIO_MODE_OUT_PP_LOW_FAST); // PG6 (D3) digit 3
    GPIO_Init(GPIOG, PIN_7, GPIO_MODE_OUT_PP_LOW_FAST); // PG7 (D4) digit 4

    uint8_t numero = 0b11111110;

    while (1)
    {
        // snake
        digit(TRUE, 0);
        GPIO_Write(GPIOB, 0b11111110);
        delay(10);
        digit(FALSE, 0);

        digit(TRUE, 1);
        GPIO_Write(GPIOB, 0b11111110);
        delay(10);
        digit(FALSE, 1);

        digit(TRUE, 2);
        GPIO_Write(GPIOB, 0b11111110);
        delay(10);
        digit(FALSE, 2);

        digit(TRUE, 3);
        GPIO_Write(GPIOB, 0b11111110);
        delay(10);
        digit(FALSE, 3);

        //
        digit(TRUE, 3);
        GPIO_Write(GPIOB, 0b11111101);
        delay(10);
        digit(FALSE, 3);

        digit(TRUE, 3);
        GPIO_Write(GPIOB, 0b11111011);
        delay(10);
        digit(FALSE, 3);

        //
        digit(TRUE, 3);
        GPIO_Write(GPIOB, 0b11110111);
        delay(10);
        digit(FALSE, 3);

        digit(TRUE, 2);
        GPIO_Write(GPIOB, 0b11110111);
        delay(10);
        digit(FALSE, 2);

        digit(TRUE, 1);
        GPIO_Write(GPIOB, 0b11110111);
        delay(10);
        digit(FALSE, 1);

        digit(TRUE, 0);
        GPIO_Write(GPIOB, 0b11110111);
        delay(10);
        digit(FALSE, 0);
        //

        digit(TRUE, 0);
        GPIO_Write(GPIOB, 0b11101111);
        delay(10);
        digit(FALSE, 0);

        digit(TRUE, 0);
        GPIO_Write(GPIOB, 0b11011111);
        delay(10);
        digit(FALSE, 0);
    }
}

/* Other programs ------------------------------------------------------------------*/

// test connection
/*
digit(TRUE, 0);
GPIO_Write(GPIOB, numbers[7]);
delay(900);
*/

/*
// snake
digit(TRUE, 0);
GPIO_Write(GPIOB, 0b11111110);
delay(10);
digit(FALSE, 0);

digit(TRUE, 1);
GPIO_Write(GPIOB, 0b11111110);
delay(10);
digit(FALSE, 1);

digit(TRUE, 2);
GPIO_Write(GPIOB, 0b11111110);
delay(10);
digit(FALSE, 2);

digit(TRUE, 3);
GPIO_Write(GPIOB, 0b11111110);
delay(10);
digit(FALSE, 3);

//
digit(TRUE, 3);
GPIO_Write(GPIOB, 0b11111101);
delay(10);
digit(FALSE, 3);

digit(TRUE, 3);
GPIO_Write(GPIOB, 0b11111011);
delay(10);
digit(FALSE, 3);

//
digit(TRUE, 3);
GPIO_Write(GPIOB, 0b11110111);
delay(10);
digit(FALSE, 3);

digit(TRUE, 2);
GPIO_Write(GPIOB, 0b11110111);
delay(10);
digit(FALSE, 2);

digit(TRUE, 1);
GPIO_Write(GPIOB, 0b11110111);
delay(10);
digit(FALSE, 1);

digit(TRUE, 0);
GPIO_Write(GPIOB, 0b11110111);
delay(10);
digit(FALSE, 0);
//

digit(TRUE, 0);
GPIO_Write(GPIOB, 0b11101111);
delay(10);
digit(FALSE, 0);

digit(TRUE, 0);
GPIO_Write(GPIOB, 0b11011111);
delay(10);
digit(FALSE, 0); */

// test numbers
/*
digit(TRUE, 1);
for (uint8_t i = 0; i < 12; i++)
{
    GPIO_Write(GPIOB, letters[i]);
    delay(100);
}*/

/*
// count from 0 to 9999 and back 9999 - 0
    uint32_t c = 0;
    for (c = 0; c < 9999; c++)
    {
        segment_write(c);
        delay(1);
        if (c == 9998)
        {
            for (uint32_t d = 9999; d > 0; d--)
            {
                segment_write(d);
                delay(1);
            }
            c = 0;
        }
    }
*/

/*
// button add numbers on 1 digit
digit(TRUE, 0);
if (read1 == RESET)
{
    aktualni_stav = 1;
}
else
{
    aktualni_stav = 0;
}

if (aktualni_stav == 1 && minuly_stav == 0) // change
{
    unit += 1;
}

if (aktualni_stav == 0 && minuly_stav == 1) // button was unpressed
{
    GPIO_Write(GPIOB, numbers[unit]);
}
minuly_stav = aktualni_stav;

if (unit >= 9) // overfloat
{
    unit = 0;
    tens += 1;
    GPIO_Write(GPIOB, numbers[tens]);
    digit(TRUE, 1);
}
*/

/*--------------------------------------------------------------------------------*/