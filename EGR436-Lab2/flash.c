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

//Function returns device ID of Flash
//Not decoded, mostly used for debugging SPI
int Flash_GetDeviceID(uint32_t * id)
{
    uint8_t i;
    uint8_t temp;
    *id = 0;

    SPI_TXFlag = NO;
    SPI_RXFlag = NO; //Expecting data

    P9->OUT &= ~BIT4; //Pull STE Low

    SPI_SendByte(RDID); //Send command
    while(!SPI_TXFlag); //Wait for interrupt
    //while(EUSCI_A3->STATW & UCBUSY);
    for(i = 0; i <4; i++)
    {
        SPI_SendByte(CLOCK_IN);
        while(!SPI_RXFlag);
        //while(EUSCI_A3->STATW & UCBUSY);
        SPI_ReadByte(&temp);
        *id |= (temp<<(i*8));
        SPI_RXFlag = NO;
    }
    P9->OUT |= BIT4;
    return 0;
}
//

int Flash_WriteData(uint16_t addr, uint8_t data[])
{
    //Set TX flag low
    //STE low
    //WREN or write enable
    //Wait flag set
    //Reset flag
    //Send Write Command
    //Wait for flag set
    //Reset flag
    //Write all data in array
    //Wait for flag
    //Reset flag
    //loop
    //WRDI reset write enable latch
    //STE High
}

int Flash_ReadData(uint16_t addr, uint8_t data[])
{

}
/*
int Flash_ReadIndex
{

}

int Flash_ParseIndex
*/
