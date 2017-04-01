/*
 * fingerprint3.c
 *
 * Created: 03-02-2015 10:18:00
 *  Author: a.naga baswanth
 */ 
#include <avr/io.h>
#include "gfx_mono.h"
#include "gfx_mono_text.h"
#include "sysfont.h"
#include <util/delay.h>
//#include <util/delay_basic.h>
//void delay(void);
//#define LCD_DATA PORTC
//#define ctrl PORTC
//#define RS PORTB_1
//#define EN PORTB_0 
#define F_CPU 32000000ul
void Set_Clk_32MHz(void);
void uart_init(void);
//void delay(void);
void send_char(char data);
void send_string(char *str);
//void receive_data(void);
//void LCD_cmd(unsigned char cmd);
//void Init_LCD(void);
//void LCD_write(unsigned char data);
//void LCD_write_string(unsigned char *str);
void Set_Clk_32MHz(void);
//void line_change (unsigned char lc);
// Function to set up UART
unsigned char i=0,fp[20],j=0,str[4],s=0,rec,dummy;
unsigned char enroll[12]={0xEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X03,0X01,0X00,0X05};
//unsigned char generate_ch[13]={0xEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X04,0x02,0X01,0X00,0X08};
unsigned char generate_ch[13]={0xEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X04,0x02,0X02,0X00,0X09};

//unsigned char store[15]={0xEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X06,0X06,0X01,0x00,0x01,0x00,0x0F};
//unsigned char identify[17]={0xEF,0X01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x08,0x1B,0x01,0x00,0x00,0x01,0x01,0x00,0x27};
unsigned char identify[17]={0XEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X08,0X04,0X02,0X00,0X00,0X00,0X04,0X00,0X13};
//unsigned char identify[17]={0XEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X08,0X04,0X02,0X00,0X00,0X00,0X08,0X00,0X17};
 char receive_data();


/***************************************************************
Code for configuring Sys Clk at 32MHz
***************************************************************/

void Set_Clk_32MHz(void)
{
	OSC_CTRL |= OSC_RC32MEN_bm;    //Setup 32Mhz crystal
	
	while(!(OSC_STATUS & OSC_RC32MRDY_bm));
	
	CCP = CCP_IOREG_gc;                    //Trigger protection mechanism
	CLK_CTRL = CLK_SCLKSEL_RC32M_gc;                //Enable internal  32Mhz crystal
	
}
/*****************************************************************************
Code for Initialize USART
*****************************************************************************/
void uart_init(void)
{
	PORTC_OUTSET = PIN3_bm; //Let's make PC7 as TX
	PORTC_DIRSET = PIN3_bm; //TX pin as output

	USARTC0_BAUDCTRLA = 207;
	USARTC0_BAUDCTRLB = 0;
	USARTC0_CTRLA = 0;
	USARTC0_CTRLC = 0x03;
	USARTC0_CTRLB = USART_TXEN_bm | USART_RXEN_bm;
}

/***************************************************************************
code for Sending a single char
*************************************************************************/
void send_char(char data)
{    USARTC0_DATA = data;
	while ( !( USARTC0_STATUS & USART_DREIF_bm) );
    //_delay_ms(50);
	//USARTC0_STATUS |= USART_DREIF_bm;  // Added by Mukesh
}



/**************************************************
code for sending a string
**************************************************/
void  send_string(char *str)
{  while(1)
   {  
		 if( *str == '\0' ) break;
      send_char(*str++);	    
   }
}

char receive_data(void)
{
	unsigned char c;
	
	//_delay_ms(100);
	while( !( USARTC0_STATUS & USART_RXCIF_bm) );
	c=USARTC0_DATA;
	//USARTC0_STATUS |= USART_RXCIF_bm;   // Added by Mukesh

}



/*-------------------------------------------------------------------------24.
main program
--------------------------------------------------------------------------*/
int main (void )
{
	PORTE_DIRSET=0xFF;
	Set_Clk_32MHz();
	//Init_LCD();
	uart_init();
	//receive_data();
	//_delay_ms(5000);
	//LCD_cmd(0x01);
	//_delay_ms(50);
	gfx_mono_init();
	PORTA_DIRCLR=0x01;
//	PORTE_OUTSET=(1<<4);

	again:
	gfx_mono_draw_string(" welcome ",0, 0, &sysfont);
	_delay_ms(50);
	//LCD_cmd(0x01);
	//LCD_cmd(0xc0);
	gfx_mono_draw_string("SCAN YOUR FINGER",0, 0, &sysfont);
	_delay_ms(50);
	i=0;
//	PORTD_DIRSET=0x00;
	while(1)
	{
		//send_char(0x01);
		//_delay_ms(2000);
		
		if((PORTA_IN & 0x01)==0x00)//checking for sw1
		{
			//PORTA_OUTSET=0x01;
		    while((PORTA_IN & 0x01)==0x00);
			//LCD_cmd(0x01);
			gfx_mono_init();
			gfx_mono_draw_string("scanning new image",0, 0, &sysfont);
			//_delay_ms(1000);
			//send_char(0x01);
			_delay_ms(5000);
			
			i=0;
			while(i<12)
			{
				send_char(enroll[i]);
				i++;
			}
			//gfx_mono_init();
		    //gfx_mono_draw_string("Command sent....",0, 0, &sysfont);
			//_delay_ms(50);
		
			rec=receive_data();
			rec=receive_data();
			rec=receive_data();
			rec=receive_data();
			rec=receive_data();
			//gfx_mono_draw_string("Receiving data....",0, 0, &sysfont);
			rec=receive_data();
			rec=receive_data();
			rec=receive_data();
			rec=receive_data();
			rec=receive_data();
			dummy=receive_data();
			dummy=receive_data();
			if(!rec)//{LCD_cmd(0x01);
			{
				gfx_mono_init();
				gfx_mono_draw_string("Enrolling ok",0, 0, &sysfont);
				_delay_ms(5000);
			}
			else
			{
				gfx_mono_init();
				//LCD_cmd(0x01);
				gfx_mono_draw_string("1: Enrolling not ok",0, 0, &sysfont);
				_delay_ms(2000);
				goto again;
			}
		}
		else
		{
				gfx_mono_init();
				//LCD_cmd(0x01);
				gfx_mono_draw_string("2: Enrolling not ok",0, 0, &sysfont);
				_delay_ms(2000);
				goto again;
		}

			i=0;
			while(i<13)
			{
				send_char(generate_ch[i]);
				i++;
			}
			//_delay_ms(1);
			rec=receive_data();
			rec=receive_data();
			rec=receive_data();
			rec=receive_data();
			rec=receive_data();
			rec=receive_data();
			rec=receive_data();
			rec=receive_data();
			rec=receive_data();
			rec=receive_data();
			dummy=receive_data();
			dummy=receive_data();
			if(!rec)
			{
				//gfx_mono_init();
				//LCD_cmd(0x01);
				gfx_mono_init();
				gfx_mono_draw_string("genchar  ok",0, 0, &sysfont);
				_delay_ms(2000);
				
				//gfx_mono_init();
				gfx_mono_draw_string("Entering to Identify",0, 0, &sysfont);
				_delay_ms(2000);
				
				//	while((PORTA_IN & 0x04)==0x00);
				//LCD_cmd(0x01);
				//	gfx_mono_draw_string("Identfying...",0, 0, &sysfont);
				//_delay_ms(1000);
				i=0;
				while(i<17)
				{
					send_char(identify[i]);
					i++;
				}
				rec=receive_data();
				rec=receive_data();
				rec=receive_data();
				rec=receive_data();
				rec=receive_data();
				rec=receive_data();
				rec=receive_data();
				rec=receive_data();
				rec=receive_data();
				rec=receive_data();
				dummy=receive_data();
				dummy=receive_data();
				dummy=receive_data();
				dummy=receive_data();
				dummy=receive_data();
				dummy=receive_data();
				if(!rec)
				{
					//LCD_cmd(0x01);
					//				gfx_mono_draw_string("store ok",0, 0, &sysfont);
					gfx_mono_init();
					gfx_mono_draw_string("Matched: Open the door",0, 0, &sysfont);
					_delay_ms(5000);
				}
				else
				{
					//gfx_mono_init();
					//LCD_cmd(0x01);
					gfx_mono_init();
					gfx_mono_draw_string("Not Matched",0, 0, &sysfont);
					_delay_ms(5000);
					goto again;
				}
			}
			else
			{
				//LCD_cmd(0x01);
				gfx_mono_init();
				gfx_mono_draw_string("genchar not ok",0, 0, &sysfont);
				gfx_mono_draw_char(rec,9,18, &sysfont);
				_delay_ms(3000);
				goto again;
			}

			/*
			//if((PORTA_IN & 0x04)==0x00)//checking for sw2136.
			//{
				//gfx_mono_init();
				gfx_mono_init();
				gfx_mono_draw_string("Entering to Identify...",0, 0, &sysfont);
				_delay_ms(2000);
				
			//	while((PORTA_IN & 0x04)==0x00);
				//LCD_cmd(0x01);
			//	gfx_mono_draw_string("Identfying...",0, 0, &sysfont);
				//_delay_ms(1000);
			i=0;
			while(i<17)
			{
				send_char(identify[i]);
				i++;
			}
			rec=receive_data();
			rec=receive_data();
			rec=receive_data();
			rec=receive_data();
			rec=receive_data();
			rec=receive_data();
			rec=receive_data();
			rec=receive_data();
			rec=receive_data();
			rec=receive_data();
			dummy=receive_data();
			dummy=receive_data();
			dummy=receive_data();
			dummy=receive_data();
			dummy=receive_data();
			dummy=receive_data();
			if(!rec)
			{
				//LCD_cmd(0x01);
//				gfx_mono_draw_string("store ok",0, 0, &sysfont);
				gfx_mono_init();
				gfx_mono_draw_string("Not matched",0, 0, &sysfont);
			}
			else
			{
				//gfx_mono_init();
				//LCD_cmd(0x01);
				gfx_mono_init();
				gfx_mono_draw_string("Matched: Open the door",0, 0, &sysfont);
				_delay_ms(5000);
				goto again;
			}
			//}
			*/
		}
	}