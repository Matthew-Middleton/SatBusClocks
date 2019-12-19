/*
 * main.cpp
 *
 *  Created on: Nov 18, 2019
 *      Author: Matthew Middleton
 */

#include "./SatBusClocks/SatCLKS.h"
//#include <msp430.h>

int main()
{

    WDTCTL = WDTPW | WDTHOLD;
    SatCLKS clock_contr;
    clock_contr.configClks(SELS__DCOCLK | SELM__DCOCLK, 0, DCOFSEL_0, DIVS__1 | DIVM__1);

    P1DIR = BIT1 | BIT0;
    P1OUT = BIT1;
    P5DIR = BIT7;
    P5SEL1 = BIT1|BIT2;
    clock_contr.configTimerA(60500);

    PM5CTL0 &= ~LOCKLPM5;
    _bis_SR_register(GIE);

    P5SEL1 = 0x00;
    P1OUT &= ~(BIT1 | BIT0);
    P1DIR = 0x0000;

    return 0;
}
