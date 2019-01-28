#include "msp.h"
#include "globals.h"
#include "spi.h"
#include "flash.h"


/**
 * main.c
 */
void main(void)
{
    uint32_t test;
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	InitHardware();

	while(1)
	{
	    Flash_GetDeviceID(test);

	}

}


void InitHardware()
{
    __disable_irq();
    SPI_PortInit();
    __enable_irq();
}




void EUSCIA3_IRQHandler(void)
{
    EUSCI_A3->IFG &= ~(EUSCI_A_IFG_TXIFG | EUSCI_A_IFG_RXIFG);
    if(SPI_TXFlag)  //Check for which register flag was set instead? That could cause false positives I think
    {
        SPI_SendByte(SPI_TXBuff[SPI_TXReadIndex]);
        SPI_TXReadIndex = (SPI_TXReadIndex + 1) % BUFFER_SIZE;
        if(SPI_TXReadIndex == SPI_TXWrtIndex)
        {
            SPI_TXFlag = NO;
        }
        return;
    }

    else if(SPI_RXFlag)
    {
        SPI_ReadByte(&SPI_RXBuff[SPI_RXWrtIndex]);
        SPI_RXWrtIndex = (SPI_RXWrtIndex + 1) % BUFFER_SIZE;
        if(SPI_RXReadIndex == SPI_RXWrtIndex)
        {
            SPI_RXFlag = NO;
        }
        //return;

    }

}
