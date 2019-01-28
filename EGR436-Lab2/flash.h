/*
 * flash.h
 *
 *  Created on: Jan 21, 2019
 *      Author: Collin
 */

#ifndef FLASH_H_
#define FLASH_H_

//Macros
//Flash memory commands
#define WREN    0b00000110  //Set write enable latch
#define WRDI    0b00000100  //Reset write enable latch
#define RDSR    0b00000101  //Read status register
#define WRSR    0b00000001  //Write status register
#define READ    0b00000011  //Read memory code
#define WRITE   0b00000010  //Write memory code
#define RDID    0b10011111  //Read device ID

Flash_GetDeviceID(uint32_t * id);

//Need to have all of the adresses mapped.



#endif /* FLASH_H_ */
