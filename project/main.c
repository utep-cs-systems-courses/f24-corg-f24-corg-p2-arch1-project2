#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include <stdio.h>


#define SW1 BIT3
volatile int lcdCount = 0;
volatile int soundCount = 0;

int main(void){
  
  while(1){
  
    P1DIR |= LEDS;

    P1OUT &= ~LED_GREEN;
    P1OUT |= LED_RED;
    
    configureClocks();
    buzzer_init();
    enableWDTInterrupts();

    
    or_sr(0x18);
  }
}
void  buzzer_init(){
  timerAUpmode();
  P2SEL2 &= ~(BIT6 | BIT7);
  P2SEL &= ~BIT7;
  P2SEL |= BIT6;
  P2DIR = BIT6;
}
#pragma vector = PORT1_VECTOR
void __interrupt_port1_ISR(int lcdCount){
  if(lcdCount >= 0 && lcdCount < 4){
    lcdCount++;
  }else{
    lcdCount = 0;
  }
}
#pragma vector = PORT2_VECTOR
void __interrupt_port2_ISR(int lcdCount){
  if(soundCount >= 0 && soundCount < 4){
    soundCount++;
  }else{
    soundCount = 0;
  }
}
void __interrupt_vec(WDT_VECTOR) WDT()
{
  static int blink_count = 0;
  blink_count++;
  static int buzzerActive = 0;
  switch(lcdCount)
  {
  case 0:
    //blink_count++;

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
    //blink_count++;

    if(blink_count%125 == 0){
      P1OUT ^= LED_RED;
    }

    if(blink_count%500  == 0){
      P1OUT |= LED_RED;
    }

    break;
  }/*
  switch(soundCount){
  case 0:
    buzzer_set_period(10000);
    break;
  case 1:
    buzzer_set_period(5000);
    break;
  case 2:
    buzzer_set_period(2000);
    break;
  case 3:
    buzzer_set_period(750);
    break;
  }*/
  if(blink_count % (250-(45*soundCount)) == 0 && !(buzzerActive==1)){
    buzzer_set_period(764);
    buzzerActive = 1;
  }
  if(blink_count- (250-(45*soundCount) ) == 50 && (buzzerActive==1)){
    buzzer_set_period(0);
    buzzerActive = 0;
  }
  if(blink_count == 500){
    blink_count = 0;
  }
}
void buzzer_set_period(short cycles){
  CCR0 = cycles;
  CCR1 = cycles >> 1;
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
