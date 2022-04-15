#include "stm8s.h"
#include "__assert__.h"

/*
uint8_t minuly_stav = 1;   // zde si budeme ukládat minulý stav tlačítka (1=tlačítko stisknuté, 0=tlačítko uvolněné)
uint8_t aktualni_stav = 1; // zde si budeme ukládat aktuální stav tlačítka (1=tlačítko stisknuté, 0=tlačítko uvolněné)
// u funkce GPIO_ReadInputPin, hodnota RESET znamená log.0 a tedy stisknuté tlačítko

void main(void)
{
    GPIO_Init(GPIOB, PIN_ALL, GPIO_MODE_OUT_PP_LOW_SLOW); // nastavíme port PB jako výstup typu push-pull (LEDka)
    GPIO_Init(GPIOE, PIN_4, GPIO_MODE_IN_FL_NO_IT);       // nastavíme PE4 jako vstup (tlačítko)

    while (1)
    { // stále dokola
        // načteme aktuální stav tlačítka
        if (GPIO_ReadInputPin(GPIOE, PIN_4) == RESET)
        {                      // zjisti jestli je tlačítko stisknuté
            aktualni_stav = 1; // pokud ano ulož že je stisknuté
        }
        else
        {
            aktualni_stav = 0; // jinak ulož že je uvolněné
        }

        // teď budeme kontrolovat jestli nenastal "okamžik stisku" nebo "okamžik uvolnění"

        if (minuly_stav == 1 && aktualni_stav == 0)
        {                                  // je to okamžik stisku ?
            GPIO_Write(GPIOB, 0b00000000); // pokud ano rozsvítíme LEDku
        }

        if (minuly_stav == 0 && aktualni_stav == 1)
        {
            GPIO_Write(GPIOB, 0b11111111); // je to okamžik uvolně              // pokud ano rozsvítíme LEDku
        }

        minuly_stav = aktualni_stav; // aktuální stav tlačítka už se stal "minulostí"
        // teď je z něj minulý stav tlačítka a my si jdeme přečíst nový aktuální stav
    }
}
*/

// držení tlačítka

uint8_t minuly_stav = 0;   // zde si budeme ukládat minulý stav tlačítka (1=tlačítko stisknuté, 0=tlačítko uvolněné)
uint8_t aktualni_stav = 0; // zde si budeme ukládat aktuální stav tlačítka (1=tlačítko stisknuté, 0=tlačítko uvolněné)
uint8_t stav_ledky = 0;    // tady si pamatujeme jestli LEDka svítí nebo je zhasnutá (1=svítí, 0=nesvítí)

void main(void)
{
    GPIO_Init(GPIOB, PIN_ALL, GPIO_MODE_OUT_PP_LOW_SLOW); // nastavíme PC5 jako výstup typu push-pull (LEDka)
    GPIO_Init(GPIOE, PIN_4, GPIO_MODE_IN_FL_NO_IT);       // nastavíme PE4 jako vstup (tlačítko)
    GPIO_Init(GPIOG, PIN_4, GPIO_MODE_IN_FL_NO_

    while (1)
    { // stále dokola
        // načteme aktuální stav tlačítka
        if (GPIO_ReadInputPin(GPIOE, PIN_4) == RESET)
        {                      // zjisti jestli je tlačítko stisknuté
            aktualni_stav = 1; // pokud ano ulož že je stisknuté
        }
        else
        {
            aktualni_stav = 0; // jinak ulož že je uvolněné
        }

        if (minuly_stav == 0 && aktualni_stav == 1)
        { // je to okamžik stisku ?
            // přepneme stav LEDky
            if (stav_ledky == 1)
            {                                  // pokud je LEDka rozsvícená
                GPIO_Write(GPIOB, 0b00000000); // zhasneme ji...
                stav_ledky = 0;                // ...a zapamatujeme si že je zhasnutá
            }
            else
            {                                   // jinak je LEDka zhasnutá, takže...
                GPIO_Write(GPIOB, 0b111111111); // ...ji rozsvítíme ...
                stav_ledky = 1;                 // ... a zapamatujeme si že je rozsvícená
            }
        }

        minuly_stav = aktualni_stav; // aktuální stav tlačítka se stal minulým
    }
}