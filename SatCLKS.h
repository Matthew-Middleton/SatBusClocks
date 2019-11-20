/*
 * SatCLKS.h
 *
 *  Created on: Nov 18, 2019
 *      Author: Matthew Middleton
 */
#pragma once
#include <stdint.h>
#include <msp430.h>

class SatCLKS
{
    /*----------------------------------------------------------------------------------------------------------------*/
    /*CLK registers*/

    volatile unsigned int *CSCTL0_H;    /*Unlocks/locks the CS registers for manipulation. CSKEY_H to unlock, 0 to lock*/
    volatile unsigned int *CSCTL1;      /*Sets the frequency for the DCO*/
    volatile unsigned int *CSCTL2;      /*Sets the CLK to the desired oscillator*/
    volatile unsigned int *CSCTL3;      /*Sets the CLK divider*/
    volatile unsigned int *CSCTL4;      /*Sets/adusts the drive strength or turns the specified CLKS off*/
    volatile unsigned int *CSCTL5;      /*Sets the fault flags for the specified oscillators*/
    volatile unsigned int *CSCTL6;      /*Sets the given CLKS to be available for conditional module requests*/
    /*-----------------------------------------------------------------------------------------------------------------*/
    public:

        SatCLKS();
        /*Sets a clock
         *
         */
        void configClks(unsigned int clk, unsigned int dco_range, unsigned int freq, unsigned int div);

        void configPreScalarClk();

        void configTimerA(unsigned int delayCycles);
};
