#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"

void GlobalIT_enable(bool on_off);
void SerialIT_enable(bool on_off);

#endif