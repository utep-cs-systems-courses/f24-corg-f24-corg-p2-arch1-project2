#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include <stdio.h>


#define SW1 BIT1
#define SW2 BIT2
#define SW3 BIT3
volatile int lcdCount = 0;
volatile int soundCount = 0;

void initButtons() {
  // Configure first button (SW1)
  P2DIR &= ~SW1;       // Set as input
  P2REN |= SW1;        // Enable pull-up/down resistor
  P2OUT |= SW1;        // Select pull-up

  P2IES |= SW1;        // High-to-low edge
  P2IFG &= ~SW1;       // Clear interrupt flag
  P2IE |= SW1;         // Enable interrupt

  // Configure second button (SW2)
  P2DIR &= ~SW2;       // Set as input
  P2REN |= SW2;        // Enable pull-up/down resistor
  P2OUT |= SW2;        // Select pull-up

  P2IES |= SW2;        // High-to-low edge
  P2IFG &= ~SW2;       // Clear interrupt flag
  P2IE |= SW2;         // Enable interrupt
  //configure third button (SW3)
  P2DIR &= ~SW2;       // Set as input
  P2REN |= SW2;        // Enable pull-up/down resistor
  P2OUT |= SW2;        // Select pull-up

  P2IES |= SW2;        // High-to-low edge
  P2IFG &= ~SW2;       // Clear interrupt flag
  P2IE |= SW2;         // Enable interrupt

}
int main(void){
  
  
    WDTCTL = WDTPW | WDTHOLD; 
    
    configureClocks();
    led_init();
    buzzer_init();
    initButtons();
    buzzer_set_period(764);
    enableWDTInterrupts();
    __enable_interrupt();
    or_sr(0x18);
}
/*
void led_init(){
    P1DIR |= LEDS;

    P1OUT &= ~LED_GREEN;
    P1OUT |= LED_RED;
    
} 
*/  
void  buzzer_init(){
  timerAUpmode();
  P2SEL2 &= ~(BIT6 | BIT7);
  P2SEL &= ~BIT7;
  P2SEL |= BIT6;
  P2DIR = BIT6;
}

void __interrupt_vec(PORT2_VECTOR) __interrupt_port2_ISR(){
  //P1OUT &= ~LEDS;
  buzzer_set_period(764);
  if(P2IFG & SW3){
    P1OUT &= ~LEDS;
    P2IFG &= 1;
    and_sr(~0x18);
  }
  if(P2IFG & SW1){

    if(lcdCount >= 0 && lcdCount < 4){
      lcdCount++;
    }else{
      lcdCount = 0;
    }
    P2IFG &= ~SW1;
  }
  if(P2IFG & SW2){

    if(soundCount >= 0 && soundCount < 4){
      soundCount++;
    }else{
      soundCount = 0;
    }
    P2IFG &= ~SW2;
    buzzer_set_period(0);
  }
}

void __interrupt_vec(WDT_VECTOR) WDT()
{
  //IFG1 &= ~WDTIFG;
  
  static int blink_count = 0;
  static int buzzerActive = 0;
  if(blink_count == 250 & SW3){
    or_sr(0x10);
  }  
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
    if(blink_count%50){
      P1OUT &= ~LED_RED;
      P1OUT ^= LED_GREEN;
    }
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
  default:
    P1OUT |= LEDS;
  }
 
  if( (blink_count % (240/(soundCount+1))==0)  ){
    buzzer_set_period(2000-((soundCount+1)*200) );
    buzzerActive = 1;
  }
  if( (blink_count%(240/(soundCount+1)) >=20)  ){
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


