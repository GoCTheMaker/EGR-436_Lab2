/*
 * flash.c
 *
 *  Created on: Jan 21, 2019
 *      Author: Collin
 */

//Includes
#include "msp.h"
#include "globals.h"
#include "spi.h"
#include "flash.h"


int Flash_GetDeviceID(uint32_t * id)
{
    uint8_t temp[BUFFER_SIZE];
    while(SPI_SendCommand(RDID)); //Wait until we can send command to the device
                                   //This is a bad idea
    SPI_RXFlag = YES; //Expecting data

    while(SPI_ReadData(temp));
    P1->OUT ^= BIT0; //Toggle LED for debugging
    *id = temp[0] | temp[1]<<8 | temp[2]<<16 | temp[3]<<24;

    return 0;

}

