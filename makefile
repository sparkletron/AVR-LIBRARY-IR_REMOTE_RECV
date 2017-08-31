SOURCES = irremote.c
ARCHIVE = libIRremoteRecv.a
AVR_CC = avr-gcc
AVR_ARCHIVE = avr-ar
AVR_CFLAGS = -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL
AVR_INCLUDE = -I AVR-LIBRARY-COMMON_DEFINES -I AVR-LIBRARY-COMMON_TIMER
AVR_AFLAGS = -r
AVR_OBJECTS = $(SOURCES:.c=.o)

all: AVR_BUILD
	
AVR_BUILD: $(ARCHIVE)

$(ARCHIVE) : $(AVR_OBJECTS)
	$(AVR_ARCHIVE) $(AVR_AFLAGS) $@ $<
	rm -f $<

%.o: %.c
	$(AVR_CC) $(AVR_INCLUDE) $(AVR_CFLAGS) -c $<

clean:
	rm -f $(AVR_OBJECTS) $(PROGRAM) $(ARCHIVE)
	