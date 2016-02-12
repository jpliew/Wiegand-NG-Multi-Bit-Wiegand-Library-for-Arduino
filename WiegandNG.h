#ifndef _WIEGAND_NG_H
#define _WIEGAND_NG_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class WiegandNG {

private:
	static void ReadD0();
	static void ReadD1();
	static volatile unsigned long 	_lastPulseTime;		// time last bits received
	static volatile int 			_bitCounted;		// number of bits arrived at Interrupt pins
//	static volatile int				_bufferIndex;		// current index in the buffer
	static int						_bufferSize;		// memory (bytes) allocated for buffer
	int								_bitAllocated;		// wiegand bits required
	int								_packetGap;			// gap between wiegand packet in millisecond
	static volatile unsigned char *	_buffer;			// buffer for data retention
	
public:
	bool begin(int bits, int packetGap=25);				// default packetGap is 25ms
	bool begin(int pinD0, int pinIntD0, int pinD1, int pinIntD1, int bits, int packetGap);
	bool available();
	void clear();
	void pause();
	int getBitCounted();
	int getBitAllocated();
	int getBufferSize();
	int getPacketGap();
	volatile unsigned char *getRawData();
	WiegandNG();
	~WiegandNG();
};

#endif
