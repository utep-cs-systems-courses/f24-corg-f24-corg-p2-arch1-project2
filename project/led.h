#ifndef led_included

#define led_included



#define LED_RED BIT0               // P1.0

#define LED_GREEN BIT6             // P1.6

#define LEDS (BIT0 | BIT6)

void buzzer_init();

void buzzer_set_period(short cycles); 

void initButtons();

void led_init();
#endif // included
