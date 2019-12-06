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
    //P1DIR = BIT1;
    //P1OUT |= BIT1;
    P5DIR = BIT7;
    P5SEL1 = BIT1|BIT2;
    clock_contr.configClks(SELM__DCOCLK, 0, DCOFSEL_0, DIVM__1);

    //clock_contr.configTimerA(50320);
    //_bis_SR_register(GIE);
    P5SEL1 = 0x00;
    return 0;
}


