#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include <stdio.h>


#define SW1 BIT3
#define SW2 BIT4
volatile int lcdCount = 0;
volatile int soundCount = 0;
void initButtons() {
  // Configure first button (SW1)
  P1DIR &= ~SW1;       // Set as input
  P1REN |= SW1;        // Enable pull-up/down resistor
  P1OUT |= SW1;        // Select pull-up

  P1IES |= SW1;        // High-to-low edge
  P1IFG &= ~SW1;       // Clear interrupt flag
  P1IE |= SW1;         // Enable interrupt

  // Configure second button (SW2)
  P1DIR &= ~SW2;       // Set as input
  P1REN |= SW2;        // Enable pull-up/down resistor
  P1OUT |= SW2;        // Select pull-up

  P1IES |= SW2;        // High-to-low edge
  P1IFG &= ~SW2;       // Clear interrupt flag
  P1IE |= SW2;         // Enable interrupt
}
int main(void){
  
  
    WDTCTL = WDTPW | WDTHOLD; 
    
    configureClocks();
    led_init();
    buzzer_init();
    initButtons();
    buzzer_set_period(440);
    enableWDTInterrupts();
    __enable_interrupt();
      
    or_sr(0x18);
}
void led_init(){
    P1DIR |= LEDS;

    P1OUT &= ~LED_GREEN;
    P1OUT |= LED_RED;
    
} 
    
void  buzzer_init(){
  timerAUpmode();
  P2SEL2 &= ~(BIT6 | BIT7);
  P2SEL &= ~BIT7;
  P2SEL |= BIT6;
  P2DIR = BIT6;
}
#pragma vector = PORT1_VECTOR
void __interrupt_port1_ISR(){
  P1OUT &= ~LEDS;
  buzzer_set_period(440);
  if(P1IFG & SW1){

    if(lcdCount >= 0 && lcdCount < 4){
      lcdCount++;
    }else{
      lcdCount = 0;
    }
    P1IFG &= ~SW1;
  }
  if(P1IFG & SW2){

    if(soundCount >= 0 && soundCount < 4){
      soundCount++;
    }else{
      soundCount = 0;
    }
    P1IFG &= ~SW2;
  }
}

void __interrupt_vec(WDT_VECTOR) WDT()
{
  IFG1 &= ~WDTIFG;
  
  static int blink_count = 0;
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
      P1OUT ^= LED_GREEN;
    }

    if(blink_count%500  == 0){
      P1OUT |= LED_RED;
    }

    break;
  }
 
  if(blink_count % (250-(45*soundCount)) == 0 && !(buzzerActive==1)){
    buzzer_set_period(1024+((soundCount+1)*0.3) );
    buzzerActive = 1;
  }
  if(blink_count- (250-(45*soundCount) ) == 50 && (buzzerActive==1)){
    buzzer_set_period(0);
    buzzerActive = 0;
  }
  if(blink_count == 500){
    blink_count = 0;
  }
  blink_count++;
}
void buzzer_set_period(short cycles){
  CCR0 = cycles;
  CCR1 = cycles >> 1;
}


