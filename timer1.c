#include "timer1.h"

uint8_t brojacPrekida = 0;
uint8_t prosla1sekunda = 0;
uint8_t brojPonavljanja = 0;
uint8_t tick5ms;

void timer1_init(uint8_t TMOD_reg, uint8_t TL1_reg, uint8_t TH1_reg, uint8_t omogucenPrekidTajmera1)
{
    TMOD |= (TMOD_reg & 0x0F) << 4;   // GORNJA 4 bita za Timer1
    TH1 = TH1_reg;
    TL1 = TL1_reg;
    ET1 = omogucenPrekidTajmera1 & 1;
}

void timer1_rst(uint8_t TL1_reg, uint8_t TH1_reg)
{
    TH1 = TH1_reg;
    TL1 = TL1_reg;
}

void timer1_start(void)
{
    TR1 = 1;
}

void timer1_stop(void)
{
    TR1 = 0;
}

void timer1_int(void) interrupt 3
{
    timer1_rst(0x00, 0xEE); // 5ms

    if (++brojacPrekida >= 200) {
        brojacPrekida = 0;
        prosla1sekunda = 1;
    }

    tick5ms = 1;   // NOVI FLAG
}

