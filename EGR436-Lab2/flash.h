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

#define INDEX_SIZE 100
#define FILE_SLOTS 20
#define INDEX_START_ADDR 0x0000

int Flash_GetDeviceID(uint32_t * id);
int Flash_WriteData(uint16_t addr, uint8_t * data, uint16_t length);
int Flash_ReadData(uint16_t addr, uint8_t * data, uint16_t length);
int Flash_GetStatus(uint8_t * status);
int Flash_ReadIndex();
int Flash_FormatDevice();

//Need to have all of the adresses mapped.



#endif /* FLASH_H_ */
