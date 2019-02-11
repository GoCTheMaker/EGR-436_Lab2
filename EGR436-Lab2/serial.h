/*
 * serial.h
 *
 *  Created on: Jan 14, 2019
 *      Author: colli
 */

#ifndef SERIAL_H_
#define SERIAL_H_

//Prototypes
void UART0_init();
int CheckFullCommand();
void ReadFromBuffer();
void EchoCommand();
int UART_ParseTitle(char * inString);
char* UART_ParsePoem (char * inString);


#endif /* SERIAL_H_ */
