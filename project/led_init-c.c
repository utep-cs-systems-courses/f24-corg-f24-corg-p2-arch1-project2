#include "led.h"
#include <msp430.h>

void led_init(){
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;
  P1OUT |= LED_RED;
}
