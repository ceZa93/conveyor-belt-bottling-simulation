#include "protocol.h"
#include "serial.h"

uint8_t* err_msg = "ERROR\r\n";
//uint8_t* ok_msg = "OK\r\n";
uint8_t index;

uint8_t cmd_index = 0;
uint8_t i;

volatile uint8_t program = 0;//-------------------------------------//
volatile uint8_t vreme_punjenja = 3;



void parse(char* buffer, uint8_t start, uint8_t len){
	if (buffer[start] == MSG_START){
		char cmd[BUFFER_SIZE];
		index = start + 1;
		cmd_index = 0;

		while (cmd_index < len){
			cmd[cmd_index++] = buffer[index];
			index = (index + 1) % BUFFER_SIZE;			
		}

		if (cmd_index == 4 && 
			cmd[0] == 'S' && 
			cmd[1] == 'T' && 
			cmd[2] == 'O' &&
			cmd[3] == 'P'){
			program = 11;  //-------------------------------------//
			send("Zaustavi motor\r\n");
		}
		else if (cmd_index == 5 && 
			cmd[0] == 'S' && 
			cmd[1] == 'T' && 
			cmd[2] == 'A' &&
			cmd[3] == 'R' &&
			cmd[4] == 'T'){
			program = 10;  //-------------------------------------//
			send("Pokreni motor\r\n");
		}else if (cmd_index >= 5 && 
			cmd[0] == 'T' && 
			cmd[1] == 'I' && 
			cmd[2] == 'M' &&
			cmd[3] == 'E'){
			vreme_punjenja = 0;

		    for (i = 4; i < cmd_index; i++) {
		        vreme_punjenja = vreme_punjenja * 10 + (cmd[i] - '0');
		    }
		
		    send("Vreme punjenja\r\n");
		}
		else {
		 	send(err_msg);
		}
	} else {
		send(err_msg);	
	}

}

