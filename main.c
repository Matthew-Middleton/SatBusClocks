#include <msp430.h> 

void configTimerA(unsigned int delayCycles);

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
    CSCTL1 = DCOFSEL_0;     /*Sets the DCO to 1 MHz: Default frequency*/
    CSCTL2 = SELM__DCOCLK | SELS__DCOCLK;  /*Selects MCLK and SMCLK as the desired clock*/
    CSCTL3 = DIVM__1 | DIVS__1;        /*Sets the divider for MCLK and SMCLK to 1*/

    /*Delay by ~10us to let DCO settle. 30 cycles = 20 cycles buffer + (10us / (1/1MHz))*/
    __delay_cycles(30);
    CSCTL0_H = 0;           /*Lock CS registers*/

    configTimerA(50000);
    /*while(1)
    {
        _bis_SR_register(LPM3_bits+GIE);//what?
    }*/
	return 0;
}

//should configure to check for unsigned overflow
void configTimerA(unsigned int delayCycles)
{

    TA0CCTL0 |= CCIE;       /*Enable Interrupts on Timer*/
    TA0CCR0 = delayCycles;  /*Number of cycles in the timer*/
    TA0CTL |= TASSEL__SMCLK;/*Use SMCLK as source for timer*/
    TA0CTL |= MC_1;         /*Use UP mode timer*/
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void timer_A0(void)
{
    P1SEL0 &= 0x0;
    P1SEL2 &= 0x0;
}
//https://e2e.ti.com/support/microcontrollers/msp430/f/166/t/618978?MSP430FR5994-LPM3-using-timer-interrupt-
