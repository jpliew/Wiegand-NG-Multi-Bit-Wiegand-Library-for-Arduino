#include <WiegandNG.h>

volatile unsigned long 	WiegandNG::_lastPulseTime;	// time last bit pulse received
volatile int 			WiegandNG::_bitCounted;		// number of bits arrived at Interrupt pins
volatile unsigned char *WiegandNG::_buffer;			// buffer for data retention
int						WiegandNG::_bufferSize;		// memory (bytes) allocated for buffer

void shift_left(volatile unsigned char *ar, int size, int shift)
{
	while (shift--) {                           	// For each bit to shift ...
		int carry = 0;                          	// Clear the initial carry bit.
		int lastElement = size-1;
		for (int i = 0; i < size; i++) {   			// For each element of the array, from low byte to high byte
			if (i!=lastElement) {
				// condition ? valueIfTrue : valueIfFalse
				carry = (ar[i+1] & 0x80) ? 1 : 0;
				ar[i] = carry | (ar[i]<<1);
			}
			else {
				ar[i] <<=1;
			}
		}   
	}
}  

void WiegandNG::clear() {							// reset variables to start new capture
	_bitCounted=0;
	_lastPulseTime = millis();
	memset((unsigned char *)_buffer,0,_bufferSize);	
	interrupts();									// allow interrupt
}

void WiegandNG::pause() {
	noInterrupts();									// disable interrupt so that user can process data 
}

volatile unsigned char * WiegandNG::getRawData() {
	return _buffer;									// return pointer of the buffer
}

int WiegandNG::getPacketGap() {
	return _packetGap;	
}

int WiegandNG::getBitAllocated() {
	return _bitAllocated;
}

int WiegandNG::getBitCounted() {
	return _bitCounted;
}

int WiegandNG::getBufferSize() {
	return _bufferSize;
}

bool WiegandNG::available() {
	bool ret=false;
	unsigned long sysTick = millis();
	if ((sysTick - _lastPulseTime) > _packetGap) {	// _packetGap (ms) laps
		if(_bitCounted>0) {							// bits found, must have data, return true
			ret=true;
		}
		else
		{
			_lastPulseTime = millis();
		}
	}
	return ret;
}

void WiegandNG::ReadD0 () {
	_bitCounted++;									// Increment bit count for Interrupt connected to D0
	shift_left(_buffer,_bufferSize,1);				// shift 0 into buffer
	_lastPulseTime = millis();						// Keep track of time last wiegand bit received
}

void WiegandNG::ReadD1() {
	_bitCounted++;									// Increment bit count for Interrupt connected to D1
	shift_left(_buffer,_bufferSize,1);				// shift 1 into buffer
	_buffer[_bufferSize-1] |=1;						// set last bit 1
	_lastPulseTime = millis();						// Keep track of time last wiegand bit received
}

bool WiegandNG::begin(int allocateBits, int packetGap) {
#ifdef digitalPinToInterrupt
	// newer versions of Arduino provide pin to interrupt mapping
	begin(2,digitalPinToInterrupt(2),3,digitalPinToInterrupt(3), allocateBits, packetGap);
#else
	begin(2,0,3,1, allocateBits, packetGap);
#endif
}

bool WiegandNG::begin(int pinD0, int pinIntD0, int pinD1, int pinIntD1, int allocateBits, int packetGap) {
	if (_buffer != NULL) {
		delete [] _buffer;
	}
	_packetGap = packetGap;
	_bitAllocated = allocateBits;
	
	_bufferSize=(_bitAllocated/8);						// calculate the number of bytes required to store wiegand bits
	if((_bitAllocated % 8) >0) _bufferSize++;			// add 1 extra byte to cater for bits that are not divisible by 8
	_buffer = new unsigned char [_bufferSize];			// allocate memory for buffer
	if(_buffer == NULL) return false;					// not enough memory, return false

	clear();
	
	pinMode(pinD0, INPUT);								// Set D0 pin as input
	pinMode(pinD1, INPUT);								// Set D1 pin as input
	attachInterrupt(pinIntD0, ReadD0, FALLING);			// Hardware interrupt - high to low pulse
	attachInterrupt(pinIntD1, ReadD1, FALLING);			// Hardware interrupt - high to low pulse
	return true;
}

WiegandNG::WiegandNG() {

}

WiegandNG::~WiegandNG() {
	if (_buffer != NULL) {
		delete [] _buffer;
	}
}


