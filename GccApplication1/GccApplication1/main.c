#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB = 0b00000000;
	DDRD = 0b11111111;

	int ls1, rs1,fl,fr,bl,br;
	while (1)
	{
		ls1 = PINB & 0b00000001;
		rs1 = PINB & 0b00000010;
		fl  = PINB & 0b00000100;
		fr  = PINB & 0b00010000;
		bl  = PINB & 0b00100000;
		br  = PINB & 0b01000000;
		
		
		if ((ls1 == 0b00000001)  && (rs1 == 0b00000010))
		{
			PORTD = 0b00001001;
			_delay_ms(10);
			PORTD = 0b00000000;
			
		}
		
		else if ((ls1 == 0b00000000)  && (rs1 == 0b00000000))
		{
			PORTD = 0b00000000;
			_delay_ms(10);
			PORTD = 0b00000000;
		}
		
		else if ((ls1 == 0b00000000)  && (rs1 == 0b00000010))
		{
			PORTD = 0b0000001;
			_delay_ms(10);
			PORTD = 0b00000000;
		}
		
		else if ((ls1 == 0b00000001)  && (rs1 == 0b00000000))
		{
			PORTD = 0b00001000;
			_delay_ms(10);
			PORTD = 0b00000000;
		}
		
	}
}

