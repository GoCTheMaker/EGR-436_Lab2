/*
 * globals.h
 *
 *  Created on: Jan 19, 2019
 *      Author: Collin
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

//Macros

#define YES 1
#define NO  0

#define BUFFER_SIZE 1024

//Globals
uint8_t SPI_TXFlag; //Flag set to alert program that data is ready to be sent
uint8_t SPI_RXFlag; //Flag set to alert program that data is ready to be parsed

uint8_t SPI_TXBuff [BUFFER_SIZE]; //Buffers for both received and sent data
uint8_t SPI_RXBuff [BUFFER_SIZE];

uint16_t SPI_TXWrtIndex; //Indexes for buffer operation
uint16_t SPI_TXReadIndex;
uint16_t SPI_RXWrtIndex;
uint16_t SPI_RXReadIndex;

#endif /* GLOBALS_H_ */
