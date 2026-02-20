#include "serial.h"
#include "protocol.h"

uint8_t *tx_buffer;
char rx_buffer[BUFFER_SIZE];
uint8_t rxw_index = 0;
uint8_t rxr_index = 0;
uint8_t rx_byte;

void Serial_Init(uint8_t SMOD1, uint8_t SPD, uint8_t brl){
	// PCON.SMOD0=0 dozvoljava konfiguraciju
	PCON &= ~((1 << 6) | (1 << 7));
	// PCON.SMOD1=1 veca brzina baudratea
	PCON |= (SMOD1 << 7);

	// SCON.SM0=0 i SCON.SM1=1 definisu 8bit UART rezim rada
	SCON &= ~(1 << 7);
	SCON |= (1 << 6); 
	// SCON.REN=1 dozvoljava primanje podataka
	SCON |= (1 << 4);

	// BDRCON.TBCK/RBCK omogucava interni baudrate generator za Tx i Rx
	// BDRCON.BRR pokrece interni baudrate generator
	BDRCON |= ((1 << 2) | (1 << 3) | (1 << 4));

	// 	BDRCON.SPD=1 veca brzina baudrate-a
	BDRCON &= ~(1 << 1);
	BDRCON |= (SPD << 1);

	// BRL odredjuje brzinu internog baudrate generatora
	BRL = brl; //256 - (uint8_t)(2^SMOD1 * 11059200/2)/(6^(1-SPD)*32*baudrate);
}

void send(uint8_t* msg){
	tx_buffer = msg;

	// Prvi bajt tx_buffer se stavlja u SBUF i salje
 	SBUF = *tx_buffer;
}

void SerialIT(void) interrupt 4{
 	if (TI){
		// Prelazi se na sledeci bajt koji jos nije poslat
		++tx_buffer;

		if (*tx_buffer != '\0'){
		  	// upis u SBUF pokrece slanje
		 	SBUF = *tx_buffer;
		}

		// Omogucava sledeci prekid ako je nesto upisano u SBUF
		TI = 0;
	} else if (RI){
		// Citanje iz SBUF preuzima jedan primljeni bajt
		rx_byte = SBUF;

		if (rx_byte == MSG_START){
			rxr_index = rxw_index;
		}

		rx_buffer[rxw_index] = rx_byte;
					   
		// Ako je jedna cela poruka stigla mozemo da je parsiramo
		if (rx_byte == MSG_END){
			uint8_t len = 0;

			if (rxw_index < rxr_index){
				len = BUFFER_SIZE - rxr_index + rxw_index - 1;
			} else {
				len = rxw_index - rxr_index - 1;
			}

			parse(rx_buffer, rxr_index, len);
		}

		// Stari bajtovi ce biti prepisani kada pristignu novi
		rxw_index = (rxw_index + 1) % BUFFER_SIZE;

		// Omogucava primanje sledeceg bajta
		RI = 0;
	}
}

