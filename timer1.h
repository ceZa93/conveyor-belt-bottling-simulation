#ifndef TIMER1_H
#define TIMER1_H

#include "gpio.h"
#include "interrupts.h"

extern uint8_t brojacPrekida;
extern volatile  uint8_t prosla1sekunda;
extern volatile uint8_t tick5ms;

void timer1_init(uint8_t TMOD_reg, uint8_t TL1_reg, uint8_t TH1_reg, uint8_t omogucenPrekid);
void timer1_rst(uint8_t TL0_reg, uint8_t TH1_reg);
void timer1_start(void);
void timer1_stop(void);
void timer1_int(void);

#endif