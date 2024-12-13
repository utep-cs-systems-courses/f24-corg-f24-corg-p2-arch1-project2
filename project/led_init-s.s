	.arch msp430g2553

	.p2align 1,0

	.text





	.global led_init
	.extern P1OUT
	.extern P1DIR
	.extern LEDS

led_init:

	bis #65, &P1DIR
	and #~64, r12
	bis #1, r12
	pop r0
