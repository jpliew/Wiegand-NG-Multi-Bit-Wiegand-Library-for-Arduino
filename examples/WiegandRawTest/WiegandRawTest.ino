#include <WiegandNG.h>

WiegandNG wg;

void PrintBinary(volatile unsigned char *buffer, int size) {
	for (int i=0; i< size;i++) {
		unsigned char bufByte=buffer[i];
		for(int x=0; x<8;x++) {
		if((bufByte & 0x80)) {
				Serial.print("1");
			}
			else {
				Serial.print("0");
			}
			bufByte<<=1;
		}
	}
	Serial.println();
}

void setup() {
	Serial.begin(9600);

	// initialize Wiegand ND for 34 bit data, every 8 bits take up 1 byte of Arduino memory
	// as long as there is still memory, user can even capture 1024 bit Wiegand by calling begin(1024)
	if(!wg.begin(34)) {										
		Serial.println("Out of memory!");
	}
	Serial.println("Ready...");
}

void loop() {
	if(wg.available()) {
		wg.pause();											// pause Wiegand pin interrupts
		Serial.print("Bits=");
		Serial.println(wg.getBitCounted());					// display the number of bits counted
		Serial.print("RAW Binary=");
		PrintBinary(wg.getRawData(), wg.getBufferSize());	// display raw data in binary form, raw data inclusive of PARITY
		wg.clear();											// compulsory to call clear() to enable interrupts for subsequent data
	}

}