# IR REMOTE LIBRARY EXAMPLE #

To test this example you have to setup some features before
compiling and flashing. The easiest device to flash is an arduino UNO or NANO
board. To flash other devices you need to pay attention to the pin configuration
and to adjust the makefile.

### Build and Flash and arduino UNO - NANO ###

Here are the steps to compile and flash and **Arduino UNO** or an **Arduino NANO** board.

### Setup ##

First you need to choose a timer for the 100us timer in `commonTimer.h`.
Make sur to have :

`#define USE100USTIMER   1`

And choose the timer. For example, if you want to use TIMER0 define :

`#define __100USTIMER    0`

At this step the timer configuration is done. The last thing to do is the pin
configuration.
In `irremote.h` mark the port on which the IR sensor signal pin is connected with the following macro :

`#define PORT_TO_USE     'B'`

You can choose **PORTB**, **PORTC** or **PORTD** by adjusting the letter. Here we'll use a pin on PORTB.
Then once you call the `initIR()` function in your code, you just need to specify the pin number (for example **PB3**) of the port you have choosed to finish the setup. 

### Build and Flash ###

Before building the project, make sure that `Makefile` options are properly set. depending on your target board/device, you'll have to adjust some parameters - `baudrate` `F_CPU` `MCU`. Open `Makefile` get more information.

Once you've done the adjustments you can build the project by typing `make`

Then you'll find the **.hex** file in the `bin/` directory. Connect your board to the computer via USB, mark the port on which the OS attached your board and then enter

`make flash port=yourport`

For example, to flash on `/dev/ttyUSB0` enter 

`make flash port=/dev/ttyUSB0`

And you've done :)




