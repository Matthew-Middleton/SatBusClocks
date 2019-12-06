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

    PM5CTL0 &= ~LOCKLPM5;
    volatile unsigned int val = 0;
    SatCLKS clock_contr;
    P1DIR = 0;
    P1OUT &= 0x0000;
    P5DIR = BIT7;
    P5SEL1 = BIT1|BIT2;
    /*CSCTL0_H = CSKEY_H;
    CSCTL2 = SELM__DCOCLK;
    CSCTL3 = DIVM__1;
    CSCTL1 = DCOFSEL_1;*/
    clock_contr.configClks(SELM__DCOCLK, 0, DCOFSEL_2, DIVM__1);

    //clock_contr.configTimerA(50320);
    //_bis_SR_register(GIE);
    P5SEL1 = 0x00;
    //P1DIR = 0x0000;
    //P1OUT = 0x0000;
    return 0;
}


