#include "msp.h"
#include "globals.h"


/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
}




void InitHardware()
{
    __disable_irq();
    EUSCI_A3->CTLW0 |= 0x01; //Disable UCA1 during config
    EUSCI_A3->CTLW0 = 0x2D82;      //bit1 0, bit6,7 10, bit8 1, bit9,10 10, bit11 1, bit12 0, bit13 1, bit14,15 00
                                   //STE used for enable, SMCLK, Synchronous, 4-pin SPI active low,
                                   //Master mode, 8-bit data, MSB first, SPI mode 0
    EUSCI_A3->BRW = 1;      //3MHz
    EUSCI_A3->CTLW0 &= ~0x01;   //Re-enable UCA3 after config

    P9->SEL0 &= ~0xF0;
    P9->SEL1 |=  0xF0;//Configure UCA3 pins in SPI3 for SPI communication
    //Need to configure pins for rest of chip control
    EUSCI_A3->IE |= 0x01;

    __enable_irq();
}




void EUSCIA3_IRQHandler(void)
{

}
