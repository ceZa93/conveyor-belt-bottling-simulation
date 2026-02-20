#ifndef SERIAL_H
#define SERIAL_H

#include "interrupts.h"

void Serial_Init(uint8_t SMOD1, uint8_t SPD, uint8_t brl);
void send(uint8_t* msg);

#endif