# makfile configuration
COMMON_OBJECTS  = main-c.o 
CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -I../h
LDFLAGS		= -L/opt/ti/msp430_gcc/include 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-as

all: led-c.elf led-s.elf

#additional rules for files
led-c.elf: ${COMMON_OBJECTS} main-c.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ ../lib/libTimer.a

led-s.elf: ${COMMON_OBJECTS} main-s.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ ../lib/libTimer.a

load: led-c.elf
	msp430loader.sh led-c.elf

load-s: led-s.elf
	msp430loader.sh led-s.elf
clean:
	rm -f *.o *.elf

#main.o: led.h
main-c.o: led.h

wdInterruptHandler.o:  led.h
