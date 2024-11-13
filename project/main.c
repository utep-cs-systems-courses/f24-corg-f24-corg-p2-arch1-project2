#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include <stdio.h>


#define SW1 BIT3
volatile int lcdCount = 0;

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

#pragma vector = PORT1_VECTOR
void __interrupt_port1_ISR(int lcdCount){
  if(lcdCount >= 0 && lcdCount < 4){
    lcdCount++;
  }else{
    lcdCount = 0;
  }
}

void __interrupt_vec(WDT_VECTOR) WDT()
{
  static int blink_count = 0;
  
  switch(lcdCount)
  {
  case 0:
    blink_count++;

    if(blink_count%125 == 0){
      P1OUT ^= LED_RED;
    }

    if(blink_count%500  == 0){
      P1OUT ^= LED_GREEN;
    }

    break;
  case 1:
    P1OUT &= ~LED_GREEN;
    break;
  case 2:
    P1OUT &= ~LED_RED;
    P1OUT ^= LED_GREEN;
    break;
  case 3:
    blink_count++;

    if(blink_count%125 == 0){
      P1OUT ^= LED_RED;
    }

    if(blink_count%500  == 0){
      P1OUT |= LED_RED;
    }

    break;
  }
  if(blink_count == 500){
    blink_count = 0;
  }
}
/*
void lcdStateHandler(int choice){
  switch(choice)
  {
  case 1:
    printf("hey");
    break;
  case 2:
    printf("what");
    break;
  case 3:
    printf("is");
    break;
  case 4:
    printf("up");
    break;
  }

}
*/
