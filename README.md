# ATMEGA328P-LIBRARY-IR_Remote_Receiver
Library to create NEC IR Remote Receiver (designed for atmega329P). This allows any NEC remote to send commands to a IR sensor attached to port c pin 5 (bit 5).

This library requires the Common_Timer and Common_Defines library, available on my github. It specifically uses the following:

void init100usTimer2(uint64_t speed)

The above is a 100us timer, the variable is e_100microseconds that the library uses for time.

Functions:

initializeIR() : Pass a 16 or 8 bit address and a struct from the defined remotes in remotes.h. Make sure to call this function first.

getIRCommandRec() : Will return a defined commandAction that matches the data captured by the IR sensor interupt routine. 

Remotes:

Adafruit Mini Remote Control PRODUCT ID: 389

