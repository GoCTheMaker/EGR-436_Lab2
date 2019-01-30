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
    //while(EUSCI_A3->STATW & UCBUSY);
    for(i = 0; i <4; i++)
    {
        SPI_SendByte(CLOCK_IN);
        SPI_ReadByte(&temp);
        *id |= (temp<<(i*8));
        SPI_RXFlag = NO;
    }
    P9->OUT |= BIT4;
    return 0;
}
//---------------------------------------------------------------------
int Flash_WriteData(uint16_t addr, uint8_t * data, uint16_t length)
{
    uint16_t i;
    uint8_t status;

    //Set TX flag low
    SPI_TXFlag = 0;
    //STE low
    P9->OUT &= ~BIT4;

    //WREN or write enable
    SPI_SendByte(WREN);
    P9->OUT |= BIT4;

    //Seems to need this deselect between commands
    P9->OUT &= ~BIT4;

    //Send Write Command
    SPI_SendByte(WRITE);

    //Mask and send address
    SPI_SendByte((addr & 0xFF00)>>8); //Bit 15 to 8
    SPI_SendByte(addr & 0xFF); //Bit 7 to 0

    //Write all data in array
    for(i = 0; i < length; i++)
    {
        SPI_SendByte(*data);
        data++;
    }
    SPI_SendByte(WRDI); //Reset Write Enable Latch
    P9->OUT |= BIT4;

    Flash_GetStatus(&status);
    P9->OUT &= ~BIT4;

    P9->OUT |= BIT4; //STE High
    return 0;
}
//-------------------------------------------
int Flash_ReadData(uint16_t addr, uint8_t * data, uint16_t length)
{
    uint16_t i;
    SPI_TXFlag = 0;
    SPI_RXFlag = 0;

    //STE low
    P9->OUT &= ~BIT4;

    //Send Read Command
    SPI_SendByte(READ);

    //Mask and send address
    SPI_SendByte((addr & 0xFF00)>>8); //Bit 15 to 8
    SPI_SendByte(addr & 0xFF); //Bit 7 to 0

    for(i = 0; i < length; i++)
    {
        SPI_SendByte(CLOCK_IN);
        SPI_ReadByte(data);
        data++;
    }

    P9->OUT |= BIT4;
    return 0;
}
//------------------------------------------------------
int Flash_GetStatus(uint8_t * status)
{
    //Set TX flag low
    SPI_TXFlag = 0;
    //STE low
    P9->OUT &= ~BIT4;

    //WREN or write enable
    SPI_SendByte(RDSR);

    //Read in
    SPI_SendByte(CLOCK_IN);
    SPI_ReadByte(status);
    P9->OUT |= BIT4;
}
//-------------------------------------
int Flash_FormatDevice()
{
    uint8_t index[INDEX_SIZE];
    uint8_t i = 0, j = 0, ret;
    uint16_t tempAddr;

    while(j < FILE_SLOTS)
    {
        tempAddr = 100 + (400*j);
        index[i++] = (tempAddr & 0xFF00)>>8;      //top bits of start addr
        index[i++] = (tempAddr & 0x00FF);       //bottom bits of start addr

        tempAddr = 499 + (400*j);
        index[i++] = (tempAddr & 0xFF00)>>8;    //Top of stop addr
        index[i++] = (tempAddr & 0x00FF);       //Bottom of stop addr
        index[i++] = NO;        //Address not full
        j++;
    }

    ret = Flash_WriteData(INDEX_START_ADDR, index, INDEX_SIZE);
    return ret;
}
//---------------------------------------------------
int Flash_ReadIndex()
{
    uint8_t index[INDEX_SIZE];
    int i;
    Flash_ReadData(INDEX_START_ADDR, index, INDEX_SIZE);

    for(i = 4; i < INDEX_SIZE; i = i + 5)
    {
        if(index[i])
        {
            //Go and get file name
            //Print file name to screen with index
        }
        //printf("Index %d: Addresses: %X - %X Activated %d\n", i / 5 , (index[i-4]<<8) | index[i-3], (index[i-2]<<8) | index[i-1], index[i]);
    }
}
//--------------------------------------------------
int Flash_FindOpenIndex()
{

}

