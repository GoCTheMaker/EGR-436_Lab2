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
	uint8_t* command;
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


	while (1)   //Main program loop
	    {

	        if(UARTFlag)    //Characters have been received but not full command
	        {

	            while(!CheckFullCommand());  //Look for newline character in the buffer

	                ReadFromBuffer();   //Get full command

                    command = calloc(MAX_CMD_SIZE,sizeof(char));
                    strncpy(command,RxRead,RxReadTo);

	                if (strstr(command, "STORE") != NULL)
	                {
	                   RxReadIndex++;
	                   UART_ParseTitle(RxRead);
	                   printf("%s",RxRead);

	                }

	                if (strstr(command, "CLEAR") != NULL)
                    {
	                    Flash_FormatDevice();

                    }

                    if (strstr(command, "DIR") != NULL)
                    {
                        Flash_DisplayIndex(data);
                    }
                    if (strstr(command, "MEM") != NULL)
                    {
                        Flash_GetMemSize(&free, &total);
                    }

                    if (strstr(command, "DELETE") != NULL)
                    {
                        Flash_DeleteFile(2);
                    }






	                if(1)
	                {
	                    EchoCommand();
	                    //Echo Back BPM
	                }
	                else
	                {
	                    //Echo back error message
	                }

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
