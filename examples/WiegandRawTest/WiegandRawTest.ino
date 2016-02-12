#include <WiegandNG.h>

WiegandNG wg;

void PrintBinary(WiegandNG tempwg) {
	volatile unsigned char *buffer=tempwg.getRawData();
	unsigned int bufferSize = tempwg.getBufferSize();
	unsigned int countedBits = tempwg.getBitCounted();

	unsigned int countedBytes = (countedBits/8);
	if ((countedBytes % 8)>0) countedBytes++;
	unsigned int bitsUsed = countedBytes * 8;
	
	for (int i=bufferSize-countedBytes; i< bufferSize;i++) {
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

	// initialize Wiegand ND for 34 bit data, every 8 bits take up 1 byte of Arduino memory
	// as long as there is still memory, user can even capture 1024 bit Wiegand by calling begin(1024)
	if(!wg.begin(1024)) {										
		Serial.println("Out of memory!");
	}
	Serial.println("Ready...");
}

void loop() {
	if(wg.available()) {
		wg.pause();							// pause Wiegand pin interrupts
		Serial.print("Bits=");
		Serial.println(wg.getBitCounted());	// display the number of bits counted
		Serial.print("RAW Binary=");
		PrintBinary(wg);					// display raw data in binary form, raw data inclusive of PARITY
		wg.clear();							// compulsory to call clear() to enable interrupts for subsequent data
	}

}