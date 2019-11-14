 #pragma once
 #include <msp430.h>
 
 
 volatile unsigned int *CSCTL0_H; /*Unlocks/Locks the CS registers for manipulation. CSKEY_H to unlock, 0 to lock*/
 volatile unsigned int *CSCTL1;
 volatile unsigned int *CSCTL2;
 volatile unsigned int *CSCTL3;
