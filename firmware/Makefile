# Name: Makefile
# Author: <insert your name here>
# Copyright: <insert your copyright message here>
# License: <insert your license reference here>

# This is a prototype Makefile. Modify it according to your needs.
# You should at least check the settings for
# DEVICE ....... The AVR device you compile for
# CLOCK ........ Target AVR clock rate in Hertz
# OBJECTS ...... The object files created from your source files. This list is
#                usually the same as the list of source files with suffix ".o".
# PROGRAMMER ... Options to avrdude which define the hardware you use for
#                uploading to the AVR and the interface where this hardware
#                is connected. We recommend that you leave it undefined and
#                add settings like this to your ~/.avrduderc file:
#                   default_programmer = "stk500v2"
#                   default_serial = "avrdoper"
# FUSES ........ Parameters for avrdude to flash the fuses appropriately.


DEVICE     = atmega328p
CLOCK      = 8000000
PROGRAMMER = -c avrisp -P /dev/tty.usbserial-AH01KQD3 -b 19200
OBJECTS    = build/utils.o build/timer.o
LIBRARY    = build/libgb7avr.a
FUSES      = -U lfuse:w:0xc2:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m

# DEVICE     = atmega168p
# CLOCK      = 8000000
# PROGRAMMER = -c stk500 -P /dev/tty.usbserial-1420
# FUSES      = -U lfuse:w:0x42:m -U hfuse:w:0xdf:m -U efuse:w:0xf9:m
# Fuse high byte:
# 0xd9 = 1 1 0 1   1 0 0 1 <-- BOOTRST (boot reset vector at 0x0000)
#        ^ ^ ^ ^   ^ ^ ^------ BOOTSZ0
#        | | | |   | +-------- BOOTSZ1
#        | | | |   +---------- EESAVE (set to 0 to preserve EEPROM over chip erase)
#        | | | +-------------- CKOPT (clock option, depends on oscillator type)
#        | | +---------------- SPIEN (if set to 1, serial programming is disabled)
#        | +------------------ WDTON (if set to 0, watchdog is always on)
#        +-------------------- RSTDISBL (if set to 0, RESET pin is disabled)
# Fuse low byte:
# 0x24 = 0 0 1 0   0 1 0 0
#        ^ ^ \ /   \--+--/
#        | |  |       +------- CKSEL 3..0 (8M internal RC)
#        | |  +--------------- SUT 1..0 (slowly rising power)
#        | +------------------ BODEN (if 0, brown-out detector is enabled)
#        +-------------------- BODLEVEL (if 0: 4V, if 1: 2.7V)
#
# For computing fuse byte values for other devices and options see
# the fuse bit calculator at http://www.engbedded.com/fusecalc/


# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-g++ -std=c++2a -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -fconcepts
ARCHIVER = avr-ar rcs
SIMULATE = simavr -f $(CLOCK) -m $(DEVICE)

# symbolic targets:

build/%.o: src/%.cpp
	$(COMPILE) -c $< -o $@

build/%.o: %.S
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

%.s: src/%.cpp
	$(COMPILE) -S $< -o $@

archive: $(OBJECTS)
	$(ARCHIVER) $(LIBRARY) $(OBJECTS)

clean:
	rm -f $(OBJECTS)
