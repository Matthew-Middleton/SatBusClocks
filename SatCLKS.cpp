
#include "SatCLKS.h"

void configClks(unsigned int clk, unsigned int dco_range, unsigned int freq, unsigned int clk_div)
{
    uint16_t tempCSCTL3 = 0;
    tempCSCTL3 = CSCTL3;        //Store CSCTL3 values temporarily

    CLKREGISTERS.(*CSCTL0_H) = CSKEY_H; //Unlocks register
    CLKREGISTERS.(*CSCTL2) = clk;   //Sets the clock to DCO
    //Sets the divider according to the clocks provided
    CLKREGISTERS.(*CSCTL3) = clk_div;

    if(dco_range&DCORSEL)   //The DCO range may exceed 8MHz.
    {

#ifdef FRCTL0
        /*Determine how many waitstates are required for the fram
         * switches based on the bit pattern(see family guide) specified by freq
         */
        switch(freq)
        {
        //If the frequency is 8 MHz: 8MHz/8MHz = 1
        case 3:
            FRCTL0 = FRCTLPW | NWAITS1;
            break;
        /*If the frequency is 16 MHz: 16MHz / 8MHz = 2
         *or the frequency is 21 MHz: 21MHz / 8MHz = 2.625
         *2 wait states are needed
         */
        case 4:
        case 5:
            FRCTL0 = FRCTLPW | NWAITS2;
            break;
        //If the frequency is 24 MHz: 24MHz / 8MHz = 3 wait states are needed
        case 6:
            FRCTL0 = FRCTLPW | NWAITS3;
            break;
        default:
            break;
        }
#endif

    }
    CLKREGISTERS.(*CSCTL1) = dco_range | freq;  //Sets the DCO to the given frequency

    /*Delay by ~10us to let DCO settle, per device errata. k cycles = 20 cycles buffer + (10us / (1/n MHz))*/
    //is this needed or do the wait states handle the delay?
    //__delay_cycles(30);//per specs

    CLKREGISTERS.(*CSCTL3) = tempCSCTL3;    //Restore clock settings. Is this necessary?
    *CSCTL0_H = 0;                  //lock the CS registers
}

void configPreScalarClk()
{

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


