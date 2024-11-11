#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void){
  while(1){
  
    P1DIR |= LEDS;

    P1OUT &= ~LED_GREEN;
    P1OUT |= LED_RED;

    configureClocks();
    enableWDTInterrupts();

    or_sr(0x18);
  }
}


void __interrupt_vec(WDT_VECTOR) WDT()
{
  static int blink_count = 0;
  blink_count++;

  if(blink_count%125){
    P1OUT ^= LED_RED;
  }

  if(blink_count%500){
    P1OUT ^= LED_GREEN;
  }
  
}
