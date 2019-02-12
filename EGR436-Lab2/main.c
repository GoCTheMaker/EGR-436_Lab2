#include "msp.h"
#include "globals.h"
#include "spi.h"
#include "flash.h"
#include "poems.h"
#include <stdio.h>
#include "hardware.h"
#include "serial.h"
#include "strings.h"




/**
 * main.c
 */
//Local Prototypes
void InitHardware();
void InitSoftware();

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    InitHardware();
    InitSoftware();

    //-------------------------------------------
	uint8_t data[MAX_ADDR];
	uint16_t len;
	uint16_t addr, free, total;
	uint8_t index;
	uint8_t command[100];
	uint8_t* fileName;
	uint8_t* fileTxt;
	uint16_t* txtTemp;
	//-------------------------------------------

	//Tom, these functions below define the 6 functional requirements of the lab
	//Documentation on use can be found in the header file
	//TODO Create these functions
	//  Receive command from computer
	//  Parse and call relevant function
	//  Implement UART from lab 1 in general

	/*
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
    */

	while (1)   //Main program loop
	{

	    if(UARTFlag)    //Characters have been received but not full command
	    {

	        while(!CheckFullCommand());  //Look for newline character in the buffer

	        ReadFromBuffer();   //Get full command

	        UART_ParseCommand(RxRead, sizeof(RxRead), command);

	        if (strstr(command, "STORE") != NULL)
	        {
	            UART_ParseFile(RxRead, sizeof(RxRead), data, &len);
	            Flash_StoreFile(data, len);

	            sprintf(data, "Received, File size: %dB\n", len);
	            UART_ReturnData(data, strlen(data));
	        }

	        if (strstr(command, "CLEAR") != NULL)
	        {
	            Flash_FormatDevice();

	            sprintf(data, "Formatted\n", len);
	            UART_ReturnData(data, strlen(data));
	        }

	        if (strstr(command, "DIR") != NULL)
	        {
	            Flash_DisplayIndex(data);

	            UART_ReturnData(data, strlen(data));
	        }
	        if (strstr(command, "MEM") != NULL)
	        {
	            Flash_GetMemSize(&free, &total);

	            sprintf(data, "Storage: %dB Free of %dB\n", free, total);
	            UART_ReturnData(data, strlen(data));

	        }
	        if (strstr(command, "DELETE") != NULL)
	        {
	            UART_ParseIndex(RxRead, sizeof(RxRead), &index);
	            Flash_DeleteFile(index);

	            sprintf(data, "Deleted %d\n", index);
	            UART_ReturnData(data, strlen(data));
	        }
	        if (strstr(command, "READ") != NULL)
	        {
	            UART_ParseIndex(RxRead, sizeof(RxRead), &index);
	            Flash_ReadFile(index, data, &len);

	            UART_ReturnData(data, strlen(data));
	        }


	        memset(command, 0, sizeof(command));
	        memset(data, 0, sizeof(data));
	    }
	}
}

void InitHardware()
{
    __disable_irq();
    SPI_PortInit();
    UART0_init();
    LedOutput_Init();
    __enable_irq();
}


void InitSoftware()
{
    UARTFlag = 0;

    //Define pointer index
    RxWriteIndex = 0;
    RxReadIndex = 0;

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

void EUSCIA0_IRQHandler(void)
{

    RxBuffer[RxWriteIndex] = EUSCI_A0->RXBUF;   //Takes char from buffer and puts writes to serial buffer
    RxWriteIndex = (RxWriteIndex + 1) % BUFFER_SIZE;    //Increments the circular buffer write index
    P2->OUT ^= 2;   //Toggles led for debug
    UARTFlag = 1;   //Set UART flag to begin parsing of the buffer
}
