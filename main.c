#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	/*Stop watchdog timer*/

	P1SEL0 |= 0x1;          /*Sets LED1 on the MSP*/
	P1SEL1 &= 0x0;          /*Clears LED2 on the MSP*/

    /*Disable the GPIO power-on default high-impedance mode to activate
    *previously configured port settings
    */
    PM5CTL0 &= ~LOCKLPM5;

    CSCTL0_H = CSKEY_H;     /*Unlocks the CS registers for manipulation*/
    CSCTL1 = DCOFSEL_0;     /*Sets the DCO to 1 MHz*/
    CSCTL2 = SELM__DCOCLK;  /*Selects MCLK as the desired clock*/
    CSCTL3 = DIVM_0;        /*Sets the divider for MCLK to 1*/

    /*Delay by ~10us to let DCO settle. 30 cycles = 20 cycles buffer + (10us / (1/1MHz))*/
    __delay_cycles(30);

    CSCTL0_H = 0;           /*Lock CS registers*/


	return 0;
}
