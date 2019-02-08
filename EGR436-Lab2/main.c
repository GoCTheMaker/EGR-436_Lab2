#include "msp.h"
#include "globals.h"
#include "spi.h"
#include "flash.h"
#include "poems.h"
#include <stdio.h>


/**
 * main.c
 */

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    InitHardware();

    //-------------------------------------------
	uint8_t data[MAX_ADDR];
	uint16_t len;
	uint16_t addr, free, total;
	uint8_t index;
	//-------------------------------------------

	//Tom, these functions below define the 6 functional requirements of the lab
	//Documentation on use can be found in the header file
	//TODO Create these functions
	//  Receive command from computer
	//  Parse and call relevant function
	//  Implement UART from lab 1 in general

	//Clear
	Flash_FormatDevice();

	//Store

	Flash_StoreFile(Poem1, sizeof(Poem1));
	Flash_StoreFile(Poem2, sizeof(Poem2));
	Flash_StoreFile(Poem3, sizeof(Poem3));
	Flash_StoreFile(Poem4, sizeof(Poem4));

	Flash_ReadData(0x0000, data, MAX_ADDR); //Reads all bytes for debugging

	//mem
	Flash_GetMemSize(&free, &total);

	//Delete
	Flash_DeleteFile(2);

	//Read
	Flash_ReadFile(2, data, &len);

	//Dir
	Flash_DisplayIndex(data);


	for(;;)
	{

	}

}

void InitHardware()
{
    __disable_irq();
    SPI_PortInit();
    __enable_irq();
}
/************************************
 * Operation of IRQ:
 *  TX flag should be re-enabled if another
 *  byte is ready to be sent. This will be useful for low
 *  power mode later.
 *
 *  Ideally writing etc will be handled in
 *  individual functions as needed.
 *
 *  Process should be:
 *      Set Flags to 0
 *      Write data to register;
 *      Wait (Sleep eventually) for flag
 *      Clear flag
 *      Write more data
 *      Repeat until process complete
 *
 ************************************/

void EUSCIA3_IRQHandler(void)//Does not operate as intended at the moment
{
    if(EUSCI_A3->IFG & UCTXIFG) //Set flag if data transfer is still operating
    {
        SPI_TXFlag = 1; //TX successful
        EUSCI_A3->IFG &= ~UCTXIFG;
    }
    if(EUSCI_A3->IFG & UCRXIFG)
    {
        SPI_RXFlag = 1; //RX successful
        EUSCI_A3->IFG &= ~(UCTXIFG|UCRXIFG);
    }
    return;

}
