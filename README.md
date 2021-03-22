# Wiegand NG - Multi Bit Wiegand Library for Arduino

This library is re-written from [Wiegand 24 and Wiegand 36 Protocol Library for Arduino](https://github.com/monkeyboard/Wiegand-Protocol-Library-for-Arduino) because the original library was coded to only detect 8, 26 and 34 bit Wiegand based on this [Wiegand RFID Reader](http://www.monkeyboard.org/products/85-developmentboard/84-rfid-wiegand-protocol-development-kit).

Wiegand NG supports unlimited bits (maximum memory allocatable by Arduino) and also support third party decoding scheme to be added. The original suggestion came from [digitalentropy](https://github.com/digitalentropy) after submitting an issue on the original library's github page.

## Features

* unlimited bits (maximum memory allocatable by Arduino)
* third party decoding support
* customizable pulseGap
* 3 Wiegand Readers when using Arduino Mega (untested)

# Different Wiegand libraries comparison

| Library | Description |
| ---| --- |
| [Wiegand library](https://github.com/monkeyboard/Wiegand-Protocol-Library-for-Arduino) | This is the easiest and cleanest library to use when only a single wiegand reader is needed. I strongly suggest to use this version if you only need one reader support |
| [Wiegand NG library](https://github.com/jpliew/Wiegand-NG-Multi-Bit-Wiegand-Library-for-Arduino) | This is another single reader library, however it uses dynamic memory to store the raw wiegand thus allowing unlimited bit length to be stored as long as your Arduino board has the memory to store it. I won't recommend to use this library unless you are facing a non-standard wiegand reader that sends out data that will not be decoded by the original Wiegand library. |
| [Multi Wiegand library](https://github.com/jpliew/Multi-Reader-Wiegand-Protocol-Library-for-Arduino) | This library uses pin change interrupt this allowing all the pin change interrupt supported pin to be used. With this, multiple readers can also be supported. Due to a workaround (hack) used to overcome the limitation with `attachInterrupt` not being able to be attached to class method directly, the initialisation of the sketch is more ugly and complicated. |


## Requirements

The following are needed 

* [Arduino](http://www.arduino.cc) Any ATMEGA328 compatible board should work.
* [Wiegand RFID Reader](http://www.monkeyboard.org/products/85-developmentboard/84-rfid-wiegand-protocol-development-kit) The code was written for this reader however customers reported working with [HID] (http://www.hidglobal.com/products/cards-and-credentials) compatible readers.
* DATA0 of Wiegand connects to Arduino PIN 2 and DATA1 of Wiegand connects to Arduino PIN 3

## Installation 

Create a folder named Wiegand in Arduino's libraries folder.  You will have the following folder structure:

	cd arduino/libraries
	mkdir WiegandNG
	cd WiegandNG
	git clone https://github.com/jpliew/Wiegand-NG-Multi-Bit-Wiegand-Library-for-Arduino.git .

## Arduino Sketch

![alt text](http://www.monkeyboard.org/images/tutorials/wiegand/wiegand_arduino.png "RFID Reader to Arduino connection diagram")


Execute Arduino IDE, select Example-->WiegandNG-->WiegandRawTest

### Example
```c
#include <WiegandNG.h>

WiegandNG wg;

void PrintBinary(WiegandNG &tempwg) {
	volatile unsigned char *buffer=tempwg.getRawData();
	unsigned int bufferSize = tempwg.getBufferSize();
	unsigned int countedBits = tempwg.getBitCounted();

	unsigned int countedBytes = (countedBits/8);
	if ((countedBits % 8)>0) countedBytes++;
	// unsigned int bitsUsed = countedBytes * 8;
	
	for (unsigned int i=bufferSize-countedBytes; i< bufferSize;i++) {
		unsigned char bufByte=buffer[i];
		for(int x=0; x<8;x++) {
			if ( (((bufferSize-i) *8)-x) <= countedBits) {
				if((bufByte & 0x80)) {
					Serial.print("1");
				}
				else {
					Serial.print("0");
				}
			}
			bufByte<<=1;
		}
	}
	Serial.println();
}

void setup() {
	Serial.begin(9600);

	// for UNO just use wg.begin(), will default to Pin 2 and Pin 3 
	// connected to D0 and D1 respectively
	// initialize Wiegand ND for 48 bit data, every 8 bits take 
	// up 1 byte of Arduino memory
	// as long as there is still memory, user can even capture 
	// 1024 bit Wiegand by calling wg.begin(1024)

	unsigned int pinD0 = 2;
	unsigned int pinD1 = 3;
	unsigned int wiegandbits = 48;
	unsigned int packetGap = 15;			// 25 ms between packet
	
	if(!wg.begin(pinD0, pinD1, wiegandbits, packetGap)) {
		Serial.println("Out of memory!");
	}
	Serial.println("Ready...");
}

void loop() {
	if(wg.available()) {
		wg.pause();			// pause Wiegand pin interrupts
		Serial.print("Bits=");
		Serial.println(wg.getBitCounted()); // display the number of bits counted
		Serial.print("RAW Binary=");
		PrintBinary(wg); // display raw data in binary form, raw data inclusive of PARITY
		wg.clear();	// compulsory to call clear() to enable interrupts for subsequent data
	}
}
```

## Acknowledgement
The library is based on the [Wiegand 24 and Wiegand 36 Protocol Library for Arduino](https://github.com/monkeyboard/Wiegand-Protocol-Library-for-Arduino) and contributions from community listed on the page.

## Contributors
[zanhecht](https://github.com/zanhecht) Fix handling of partial bytes.

Written by [JP Liew](http://jpliew.com)

Project home: http://www.monkeyboard.org/tutorials/82-protocol/24-wiegand-converter

*This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.*

*This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.*
