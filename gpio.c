#include "gpio.h"

void init_gpio(void)
{
	P0 = 0xFF; //sve jedinice, dugmici nisu stisnuti
	P2 = 0x00; //sve diode iskljucene	
}
