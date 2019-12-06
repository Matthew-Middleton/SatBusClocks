/*
 * SatCLKS.cpp
 *
 *  Created on: Nov 18, 2019
 *  Updated on: Dec 5, 2019
 *      Author: Matthew Middleton
 */

#include "SatCLKS.h"

SatCLKS::SatCLKS()
{
    /*Assigns each of the CS registers to a pointer within the class*/
    this->CSCTL0_H_ = (volatile unsigned int *) &CSCTL0_H;
    this->CSCTL1_ = (volatile unsigned int *) &CSCTL1;
    this->CSCTL2_ = (volatile unsigned int *) &CSCTL2;
    this->CSCTL3_ = (volatile unsigned int *) &CSCTL3;
    this->CSCTL4_ = (volatile unsigned int *) &CSCTL4;
    this->CSCTL5_ = (volatile unsigned int *) &CSCTL5;
    this->CSCTL6_ = (volatile unsigned int *) &CSCTL6;
}

void SatCLKS::configClks(unsigned int clk, unsigned int dco_range, unsigned int freq, unsigned int clk_div)
{
    unsigned int cycles = 0;
    //calculation for cycle delays. k cycles = 20 cycles buffer + (10us / (1/n MHz))
    cycles = 20 + (10 / lookup_frequency(dco_range, freq));

    *CSCTL0_H_ = CSKEY_H; //Unlocks register
    *CSCTL1_ = 0;
    *CSCTL2_ = clk;   //Sets the clock to DCO
    //Sets the divider according to the clocks provided
    *CSCTL3_ = clk_div;

    if(dco_range&DCORSEL)   //The DCO range may exceed 8MHz.
    {

#ifdef FRCTL0
        /*Determine how many waitstates are required for the fram
         * switches based on the bit pattern(see family guide) specified by freq
         */
        switch(freq & (BIT3|BIT2|BIT1))
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
    else if(freq & (BIT6|BIT3|BIT2))        //The frequency for DCORSEL=0 is 8MHz
    {
#ifdef FRCTL0
        //8MHz frequency
        FRCTL0 = FRCTLPW | NWAITS1;
#endif
    }
    *CSCTL1_ = dco_range | freq;  //Sets the DCO to the given frequency

    /*Delay by ~10us to let DCO settle, per device errata. */
    __delay_cycles(30);//per specs

    *CSCTL0_H_ = 0;                  //lock the CS registers
}

/*Looks up the frequency needed to process the cycle delays based off the dco_range, upper or lower (DCORSEL), and freq, the frequency (DCOFSEL)
 */
unsigned int SatCLKS::lookup_frequency(unsigned int dco_range, unsigned int freq)
{
    unsigned int lookup = 1;
    if(!(dco_range&BIT6))//lower frequency range; DCORSEL value = 0
    {
        switch(freq)//DCOFSEL value passed in
        {
        //values rounded up per CSCTL1 register DCOFSEL values
            case 0:         //1MHz
                lookup = 1;
                break;
            case 1:         //2.67MHz
                lookup = 3;
                break;
            case 2:         //3.5MHz
            case 3:         //4MHz
                lookup = 4;
                break;
            case 4:         //5.33MHz
                lookup = 6;
                break;
            case 5:         //7MHz
                lookup = 7;
                break;
            case 6:         //8MHz
                lookup = 8;
                break;
            default:        //1MHz
                lookup = 1;
                break;
        }
    }
    else//upper frequency range; DCORSEL value = 1
    {
        switch(freq)//DCORSEL value passed in
        {
            case 0:         //1MHz
                lookup = 1;
                break;
            case 1:         //5.33MHz
                lookup = 6;
                break;
            case 2:         //7MHz
                lookup = 7;
                break;
            case 3:         //8MHz
                lookup = 8;
                break;
            case 4:         //16MHz
                lookup = 16;
                break;
            case 5:         //21MHz
                lookup = 21;
                break;
            case 6:         //24MHz
                lookup = 24;
                break;
            default:        //1MHz
                lookup = 1;
                break;
        }
    }
    return lookup;
}

void SatCLKS::configPreScalarClk()
{

}

void SatCLKS::configTimerA(unsigned int delayCycles)
{

    TA0CCTL0 |= CCIE;                           /*Enable Interrupts on Timer*/
    TA0CCR0 = delayCycles;                      /*Number of cycles in the timer*/
    TA0CTL = TASSEL__SMCLK | MC__UP | ID__1;    /*Use SMCLK as source for timer in UP mode*/
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void timer_A0(void)
{
    P1OUT ^= BIT1|BIT0;
    TA0CCTL0 &= ~TAIFG;
}
//https://e2e.ti.com/support/microcontrollers/msp430/f/166/t/618978?MSP430FR5994-LPM3-using-timer-interrupt-
//http://e2e.ti.com/support/microcontrollers/msp430/f/166/t/609971?MSP430FR5994-Can-t-get-system-to-work-on-16-MHz


