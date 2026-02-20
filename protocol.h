#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#define MSG_START '<'
#define MSG_END	  '>'
#define BUFFER_SIZE 10

extern volatile uint8_t program;
extern volatile uint8_t vreme_punjenja;

void parse(char* buffer, uint8_t start, uint8_t len);

#endif