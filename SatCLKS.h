 #pragma once
 #include <msp430.h>
 
 namespace SatLib
 {
	class SatCLKs
	{
	/*----------------------------------------------------------------------------------------------------------------*/
	/*CLK registers*/
 
	volatile unsigned int *CSCTL0_H; 	/*Unlocks/locks the CS registers for manipulation. CSKEY_H to unlock, 0 to lock*/
	volatile unsigned int *CSCTL1;		/*Sets the frequency for the DCO*/
	volatile unsigned int *CSCTL2;		/*Sets the CLK to the desired oscillator*/
	volatile unsigned int *CSCTL3;		/*Sets the CLK divider*/
	volatile unsigned int *CSCTL4;		/*Sets/adusts the drive strength or turns the specified CLKS off*/
	volatile unsigned int *CSCTL5;		/*Sets the fault flags for the specified oscillators*/
	volatile unsigned int *CSCTL6;		/*Sets the given CLKS to be available for conditional module requests*/
	/*-----------------------------------------------------------------------------------------------------------------*/
	};
 }
 
