

#include "SatCLKS.h"

void configTimerA(unsigned int delayCycles);

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

    setCLK(SELM_DCOCLK | SELS_DCOCLK, DCOFSEL_2, DIVM__1 | DIVS__1)

    configTimerA(500);
    while(1)
    {
        _bis_SR_register(GIE);
    }
    return 0;
}

void setCLK(unsigned int clk, unsigned int freq, unsigned int div)
{
    CLKREGISTERS.(*CSCTL0_H) = CSKEY_H; //Unlocks register
#ifdef FRCTL0
    FRCTL0 = FRCTLPW | NWAITS1;
#endif
    CLKREGISTERS.(*CSCTL1) = freq;  //Sets the DCO to the given frequency
    CLKREGISTERS.(*CSCTL2) = clk;   //Sets the clock to DCO
    CLKREGISTERS.(*CSCTL3) = div;   //Sets the divider according to the clocks provided

    /*Delay by ~10us to let DCO settle. 30 cycles = 20 cycles buffer + (10us / (1/1MHz))*/
    __delay_cycles(4);//per specs
    *CSCTL0_H = 0;                      //lock the CS registers
}

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
//http://e2e.ti.com/support/microcontrollers/msp430/f/166/t/609971?MSP430FR5994-Can-t-get-system-to-work-on-16-MHz
