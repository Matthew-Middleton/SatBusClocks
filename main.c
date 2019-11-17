
#include "SatCLKS.h"
void configTimerA(unsigned int delayCycles);

void setCLKPreScalar(unsigned int clk, unsigned int Hz, unsigned int divisor)
{
    *CSCTL0_H = CSKEY_H;//Unlocks register
    *CSCTL1 = Hz;       //Sets the DCO to the given frequency
    *CSCTL2 = clk;      //Selects the given CLKs
    *CSCTL3 = divisor;  //Sets the divider according to the clocks provided

    //need to determine how to set cycle delay to let the DCO settle based of user input
    /*Delay by ~10us to let DCO settle. 30 cycles = 20 cycles buffer + (10us / (1/1MHz))*/
    __delay_cycles(30);
    *CSCTL0_H = 0;
}
/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   /*Stop watchdog timer*/

    P1DIR |= BIT1;
    P1OUT |= BIT1;

    /*Disable the GPIO power-on default high-impedance mode to activate
    *previously configured port settings
    */
    PM5CTL0 &= ~LOCKLPM5;

    CSCTL0_H = CSKEY_H;     /*Unlocks the CS registers for manipulation*/
    CSCTL1 = DCOFSEL_0;     /*Sets the DCO to 1 MHz: Default frequency*/
    CSCTL2 = SELM__DCOCLK | SELS__DCOCLK;  /*Selects MCLK and SMCLK as the desired clock*/
    CSCTL3 = DIVM__1 | DIVS__1;        /*Sets the divider for MCLK and SMCLK to 1*/

    /*Delay by ~10us to let DCO settle. 30 cycles = 20 cycles buffer + (10us / (1/1MHz))*/
    __delay_cycles(30);
    CSCTL0_H = 0;           /*Lock CS registers*/

    configTimerA(500);
    while(1)
    {
        _bis_SR_register(GIE);
    }
    return 0;
}

//should configure to check for unsigned overflow
void configTimerA(unsigned int delayCycles)
{

    TA0CCTL0 |= CCIE;                           /*Enable Interrupts on Timer*/
    TA0CCR0 = delayCycles;                      /*Number of cycles in the timer*/
    TA0CTL = TASSEL__SMCLK | MC__UP | ID__1;    /*Use SMCLK as source for timer in UP mode*/
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void timer_A0(void)
{
    P1OUT ^= BIT1;
    TA0CCTL0 &= ~TAIFG;
}
//https://e2e.ti.com/support/microcontrollers/msp430/f/166/t/618978?MSP430FR5994-LPM3-using-timer-interrupt-
