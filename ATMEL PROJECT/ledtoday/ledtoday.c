/*
 * ledtoday.c
 *
 * Created: 2/5/2015 11:51:55 AM
 *  Author: naga baswanth
 */ 


#include <avr/io.h>
#define F_CPU 32000000ul
#include <util/delay.h>
void Set_Clk_32MHz(void);
void delay(void);

int main(void)
{
	PORTA_DIRCLR=0x0F;
	PORTA_DIRSET=0x0F;
	
    while(1)
    {
		if((PORTD_IN & 0X01)==0X01)
		{
			PORTA_OUTSET=0X01;
			_delay_ms(50);

	PORTA_OUTCLR=0X01;
		_delay_ms(50);
        //TODO:: Please write your application code 
    
		
		}
	}
	
}
