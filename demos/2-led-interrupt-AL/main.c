//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"
/*
int main(void) {
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;
  P1OUT |= LED_RED;

  configureClocks();		// setup master oscillator, CPU & peripheral clocks 
  enableWDTInterrupts();	// enable periodic interrupt 
  
  or_sr(0x18);		 CPU off, GIE on 
}


void
__interrupt_vec(WDT_VECTOR) WDT()	// 250 interrupts/sec
{
  P1OUT |= LED_GREEN;
} 

*/
;
main:
    BIS #LEDS, &P1DIR
    BIC #LED_GREEN, &P1OUT
    BIS #LED_RED, &P1OUT
    CALL #configureClocks
    CALL #enableWDTInterrupts
    OR #0X18, &SR
    jmp $

interrupt_vector:
  .sect ".text"
  .global _interrupt_vec
  .type _interrupt_vec, @function

_interrupt_vec
  BIS #LED_GREEN, &P1OUT
  RETI

  
