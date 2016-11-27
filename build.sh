#!/bin/sh

mkdir build
avr-gcc -mmcu=atmega328p -std=c99 -o ./build/temp.o *.c 
avr-objcopy -j .text -j .data -O ihex ./build/temp.o ./build/temp.hex
avrdude -v -v -v -v -V -F -C /home/bic/Development/Arduino/arduino-1.6.12/hardware/arduino/avr/bootloaders/gemma/avrdude.conf -p atmega328p -P /dev/ttyUSB0 -c stk500v1 -b 115200 -U flash:w:./build/temp.hex:i
ls -lah build | grep 'hex'
#rm -r build
