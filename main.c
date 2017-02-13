#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#define PIN_RC_PULSE 3
#define PIN_GLOW_PLUG 4

unsigned int RCpulseLen(unsigned char PIN_NR){
	while(PINB & (1<<PIN_NR)); 							//Wait as long as PIN_NR is high
	TCCR0B=0x04;														//Set timer/counter0 prescaler to divide by 256 => 8,162 ms to overflow
	GTCCR = GTCCR & 0x7B; 									//Start timer/counter0
	while(! (PINB & (1<<PIN_NR))); 					//Wait as long as PIN_NR is low
	TCNT0 = 0; 															//Reset Timer/Counter 0
	while(PINB & (1<<PIN_NR)); 							//Wait as long as PIN_NR is high
	return TCNT0*32;												//Return pulse length in microseconds (resolution 32 microseconds)
}

int main(void) {
	DDRB = DDRB & ~(1<<PIN_RC_PULSE);				//Set PIN_RC_PULSE as input
	DDRB = DDRB | (1<<PIN_GLOW_PLUG);				//Set PIN_GLOW_PLUG as output
	_delay_ms(1000); 												//Wait 1 second to let things stabilize
	while(1){
		while(RCpulseLen(PIN_RC_PULSE)<1500);	//Wait while Pulse < 1500
		PORTB = PORTB | (1<<PIN_GLOW_PLUG);		//Turn glow plug on
		_delay_ms(15000);											//wait 15 seconds
		PORTB = PORTB & ~(1<<PIN_GLOW_PLUG);	//turn glow plug off
		while(RCpulseLen(PIN_RC_PULSE)>1500);	//Wait while Pulse > 1500
	}
}
