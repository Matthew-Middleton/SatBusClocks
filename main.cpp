/*
 * main.cpp
 *
 *  Created on: Nov 18, 2019
 *      Author: Matthew Middleton
 */

#include "SatCLKS.h"

int main()
{
    WDTCTL = WDTPW | WDTHOLD;

    PM5CTL0 &= ~LOCKLPM5;

    SatCLKS clock_contr;
    volatile unsigned int i = 50000;
    P1DIR = BIT1;
    P1OUT |= BIT1;
    clock_contr.configClks(SELS__DCOCLK | SELM__DCOCLK, DCORSEL, DCOFSEL_0, DIVS__1 | DIVM__1);

    clock_contr.configTimerA(50000);

        _bis_SR_register(GIE);
    return 0;
}


