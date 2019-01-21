/*
 * globals.h
 *
 *  Created on: Jan 19, 2019
 *      Author: colli
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

//Macros
#define WREN    0b00000110  //Set write enable latch
#define WRDI    0b00000100  //Reset write enable latch
#define RDSR    0b00000101  //Read status register
#define WRSR    0b00000001  //Write status register
#define READ    0b00000011  //Read memory code
#define WRITE   0b00000010  //Write memory code
#define RDID    0b10011111  //Read device ID


#endif /* GLOBALS_H_ */
