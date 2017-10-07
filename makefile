SOURCES := irremote.c
ARCHIVE := $(addprefix lib,$(SOURCES:.c=.a))
AVR_MMCU = atmega328p
AVR_CPU_SPEED = 16000000UL
LIB_PATH = AVR-LIBRARY-COMMON_DEFINES AVR-LIBRARY-COMMON_TIMER

CROSS_COMPILE := avr-
CC := gcc
AR := ar

INCLUDES = $(addprefix -I,$(LIB_PATH))

AVR_CFLAGS := -Wall -g2 -gstabs -O1 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=$(AVR_MMCU) -DF_CPU=$(AVR_CPU_SPEED)
AVR_AFLAGS := -r
AVR_OBJECTS := $(SOURCES:.c=.o)

all: AVR_BUILD

AVR_BUILD: $(ARCHIVE)

$(ARCHIVE) : $(AVR_OBJECTS)
	$(CROSS_COMPILE)$(AR) $(AVR_AFLAGS) $@ $<
	rm -f $<

%.o: %.c
	$(CROSS_COMPILE)$(CC) $(INCLUDES) $(AVR_CFLAGS) -c $<

clean:
	rm -f $(AVR_OBJECTS) $(ARCHIVE)
