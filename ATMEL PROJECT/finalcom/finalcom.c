/*
 * finalcom.c
 *
 * Created: 2/11/2015 7:48:35 PM
 *  Author: naga baswanth
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
void scan_keypad(void);
void Password (void);
unsigned char pwd[3];
unsigned char rx=0;
unsigned char u0=0, u1=0,u2=0,u3=0;
void int_to_char(unsigned char int_val);

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
 void int_to_char(unsigned char int_val)
 {
	 
	 unsigned char u,tn,h;
	 u = int_val%10;
	 int_val = int_val/10;
	 tn = int_val%10;
	 int_val = int_val/10;
	 h = int_val;
	 send_char(h|0x30);
	 
	 send_char(tn|0x30);
	 send_char(u|0x30);
	 send_char('\r');
	 send_char('\n');

 }
 


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
	gfx_mono_init();
	PORTE_OUTSET=(1<<4);
	PORTE_DIRSET=0x0F; // PORTE as output port
	PORTD_DIR =0x00; // PORTD as input port
	PORTB_DIR = 0x0F;//port b as input
	PORTC_OUT = 0x01;
	
//	PORTE_OUTSET=(1<<4);

	again:
	gfx_mono_init();
	PORTE_OUTSET=(1<<4);
	gfx_mono_draw_string(" welcome ",0, 0, &sysfont);
	_delay_ms(1000);
	//LCD_cmd(0x01);
	//LCD_cmd(0xc0);
	gfx_mono_draw_string("SCAN YOUR FINGER",0, 0, &sysfont);
	_delay_ms(1000);
	i=0;
	   //PORTC_OUT = 0x01;
	   
	  

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
			//else if
			

			
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
			 if((PORTA_IN & 0x04)==0x00)
				{
					while((PORTA_IN & 0x04)==0x00);	
					Set_Clk_32MHz();
					uart_init();
					gfx_mono_init();
					PORTE.OUTSET = PIN4_bm;
					gfx_mono_draw_string("testing keypad\r\n", 0, 0, &sysfont);
					_delay_ms(50);
					send_string("testing keypad\r\n");
					Password();
					goto again;
															
					
					
				}
				else
					{

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
			}
				
			}
			
		

	void Password(void)
	{
		
		OSC.CTRL |= OSC_RC32MEN_bm;
		while(!(OSC.STATUS & OSC_RC32MRDY_bm));
		CCP = CCP_IOREG_gc;
		CLK.CTRL = CLK_SCLKSEL_RC32M_gc;
		gfx_mono_init();
		PORTE.OUTSET = PIN4_bm;
		_delay_ms(1000);
		gfx_mono_draw_string("Enter key:", 0, 0, &sysfont);
		//  _delay_ms(50);
		send_string("Enter key: \r\n");
		scan_keypad();
		while(rx <= 3)
		{
			scan_keypad();
		}

		for(rx=0;rx<=3;rx++)
		{
			send_char(pwd[rx]);
		}
		send_char('\r');
		send_char('\n');
		_delay_ms(1000);
		if ((pwd[0]=='1') && (pwd[1]=='2') && (pwd[2]=='3') && (pwd[3]=='4'))
		{
			gfx_mono_init();
			PORTE.OUTSET = PIN4_bm;
			gfx_mono_draw_string("Matched key", 0, 0, &sysfont);
			
			
			send_string("Matched key\r\n");
			rx=0;
			u0++;
			int_to_char(u0);
			PORTE.OUTSET = PIN4_bm;
			gfx_mono_draw_string("enter password", 5, 18, &sysfont);
			send_string("enter password\r\n");
			while(rx <= 3)
			{
				scan_keypad();
			}
			for(rx=0;rx<=3;rx++)
			{
				send_char(pwd[rx]);
			}
			send_char('\r');
			send_char('\n');
			if ((pwd[0]=='2') && (pwd[1]=='3') && (pwd[2]=='4') && (pwd[3]=='5'))
			{
				
				gfx_mono_init();
				PORTE.OUTSET = PIN4_bm;
				gfx_mono_draw_string("1st person ", 5, 18, &sysfont);
				_delay_ms(100);
				send_string("Matched first person password\r\n");
				_delay_ms(100);
				rx=0;
				u0++;
				int_to_char(u0);
				_delay_ms(50);
				Password();

			}
			else if ((pwd[0]=='2') && (pwd[1]=='3') && (pwd[2]=='4') && (pwd[3]=='7'))
			{
				gfx_mono_init();
				PORTE.OUTSET = PIN4_bm;
				gfx_mono_draw_string("2nd Matched", 5, 18, &sysfont);
				_delay_ms(100);
				send_string("Password second Matched\r\n");
				rx=0;
				u2++;
				int_to_char(u2);
				Password();
			}
			
			else if ((pwd[0]=='2') && (pwd[1]=='3') && (pwd[2]=='4') && (pwd[3]=='8'))
			{
				gfx_mono_init();
				PORTE.OUTSET = PIN4_bm;
				gfx_mono_draw_string("3rd matched", 5, 18, &sysfont);
				_delay_ms(100);
				send_string("Password Matched for Person_3\r\n");
				rx=0;
				u3++;
				int_to_char(u3);
				Password();
			}
			else
			{
				gfx_mono_init();
				PORTE.OUTSET = PIN4_bm;
				gfx_mono_draw_string("Password Didn't Matched\r\n", 0, 0, &sysfont);
				_delay_ms(100);
				send_string("Password Didn't Matched\r\n");
				gfx_mono_init();
				PORTE.OUTSET = PIN4_bm;
				gfx_mono_draw_string("Try Again1!\n\r", 5, 18, &sysfont);
				_delay_ms(100);
				send_string("Try Again!\n\r");
				rx=0;
				Password();
			}
		}
		else
		{   gfx_mono_init();
			PORTE.OUTSET = PIN4_bm;
			gfx_mono_draw_string("key did'nt matched\r\n", 0, 0, &sysfont);
			_delay_ms(2000);
			gfx_mono_init();
			PORTE.OUTSET = PIN4_bm;
			gfx_mono_draw_string("Try again!\n\r", 5, 18, &sysfont);
			_delay_ms(1000);
			send_string("key did'nt matched\r\n");
			send_string("Try again!\n\r");
			rx=0;
			Password();
		}
	
	}

      //int main(void)
	
		 // gfx_mono_init();
		  //PORTE_OUTSET=(1<<4);
		  //PORTE_DIRSET=0x0F; // PORTE as output port
		  //PORTD_DIR =0x00; // PORTD as input port
		  //PORTB_DIR = 0x0F;//port b as input
	      //PORTC_OUT = 0x01;
		//{
			
		
		//Set_Clk_32MHz();
		//uart_init();
	//	gfx_mono_init();
	//	PORTE.OUTSET = PIN4_bm;
		//gfx_mono_draw_string("testing keypad\r\n", 0, 0, &sysfont);
		//_delay_ms(50);
		//send_string("testing keypad\r\n");
		//if(PORTE_IN = 0x38)
		//{
			//gfx_mono_init();
			//PORTE.OUTSET = PIN4_bm;
			//gfx_mono_draw_string("The user_1 entered=", 0, 0, &sysfont);
			//send_string("The user_1 entered=");
			//int_to_char(u1);
		//	gfx_mono_draw_string("\r\n", 0, 0, &sysfont);
			//send_string("\r\n");
			//gfx_mono_draw_string("The user_2 entered=", 0, 0, &sysfont);
			//send_string("The user_2 entered=");
			
			//int_to_char(u2);
			//gfx_mono_draw_string("\r\n", 0, 0, &sysfont);
			//send_string("\r\n");
			//gfx_mono_draw_string("The user_3 entered=", 0, 0, &sysfont);
			//send_string("The user_3 entered=");
		//	int_to_char(u3);
			//gfx_mono_draw_string("\r\n", 0, 0, &sysfont);
			//send_string("\r\n");
			//Password();
		//}
		//else
	//	Password();
		
		
	//}


	void scan_keypad(void)
	{
		PORTE_OUT = 0x02;
		PORTD_DIR =0x00; // PORTD as input port
		// Scan A[1x2]
		if ((PORTE_OUT == 0x02) && (PORTD_IN ==0x31)) // if 0th pin of PORTD is receiving logic high
		{
			gfx_mono_init();
			PORTE.OUTSET = PIN4_bm;
			gfx_mono_draw_string("2\r\n", 0, 0, &sysfont);
			send_string("2\r\n");
			pwd[rx]='2';
			rx++;
			
			_delay_ms(1000);
		}
		// Scan A[2x2]
		if((PORTE_OUT == 0x02) && (PORTD_IN == 0x38))
		{
			gfx_mono_init();
			PORTE.OUTSET = PIN4_bm;
			gfx_mono_draw_string("5\r\n", 0, 0, &sysfont);
			send_string("5\r\n");
			pwd[rx]='5';
			rx++;
			_delay_ms(1000);
		}
		// Scan A[3x2]
		if((PORTE_OUT == 0x02) && (PORTD_IN == 0x34))
		{
			pwd[rx]='8';
			rx++;
			gfx_mono_init();
			PORTE.OUTSET = PIN4_bm;
			gfx_mono_draw_string("8\r\n", 0, 0, &sysfont);
			send_string("8\r\n");
			_delay_ms(1000);
			//G:\RAHUL IMP_DATA\M.Tech_Project\My xmega usart running code\KAYPAD\KEYPAD\KEYPAD\KEYPAD.c 3
		}
		// Scan A[4x2]
		if((PORTE_OUT == 0x02) && (PORTD_IN == 0x32))
		{
			gfx_mono_init();
			PORTE.OUTSET = PIN4_bm;
			gfx_mono_draw_string("0\r\n", 0, 0, &sysfont);
			send_string("0\r\n");
			pwd[rx]='0';
			rx++;
			_delay_ms(1000);
		}
		
		PORTE_OUT = 0x01;
		PORTD_DIR =0x00; // PORTD as input port
		// Scan A[1x1]
		if ((PORTE_OUT == 0x01) && (PORTD_IN == 0x31)) // if 0th pin of PORTD is receiving logic high
		{
			gfx_mono_init();
			PORTE.OUTSET = PIN4_bm;
			gfx_mono_draw_string("1\r\n", 0, 0, &sysfont);
			send_string("1\r\n");
			pwd[rx]='1';
			rx++;
			_delay_ms(1000);
		}
		// Scan A[2x1]
		if((PORTE_OUT == 0x01) && (PORTD_IN == 0x38))
		{
			
			pwd[rx]='4';
			rx++;
			gfx_mono_init();
			PORTE.OUTSET = PIN4_bm;
			gfx_mono_draw_string("4\r\n", 0, 0, &sysfont);
			send_string("4\r\n");
			_delay_ms(1000);
		}
		// Scan A[3x1]
		if((PORTE_OUT == 0x01) && (PORTD_IN == 0x34))
		{
			pwd[rx]='7';
			rx++;
			gfx_mono_init();
			PORTE.OUTSET = PIN4_bm;
			gfx_mono_draw_string("7\r\n", 0, 0, &sysfont);
			send_string("7\r\n");
			_delay_ms(1000);
		}
		// Scan A[4x1]
		if((PORTE_OUT == 0x01) && (PORTD_IN == 0x32))
		{
			pwd[rx]='*';
			rx++;
			gfx_mono_init();
			PORTE.OUTSET = PIN4_bm;
			gfx_mono_draw_string("*\r\n", 0, 0, &sysfont);
			send_string("*\r\n");
			_delay_ms(1000);
		}
		
		PORTE_OUT = 0x04;
		PORTD_DIR =0x00; // PORTD as input port
		// Scan A[1x3]
		if ((PORTE_OUT == 0x04) &&(PORTD_IN ==0x31)) // if 0th pin of PORTD is receiving logic high
		{
			pwd[rx]='3';
			rx++;
			gfx_mono_init();
			PORTE.OUTSET = PIN4_bm;
			gfx_mono_draw_string("3\r\n", 0, 0, &sysfont);
			send_string("3\r\n");
			_delay_ms(1000);
		}
		// Scan A[2x3]
		if((PORTE_OUT == 0x04) && (PORTD_IN == 0x38))
		{
			pwd[rx]='6';
			rx++;
			gfx_mono_init();
			PORTE.OUTSET = PIN4_bm;
			gfx_mono_draw_string("6\r\n", 0, 0, &sysfont);
			send_string("6\r\n");
			_delay_ms(1000);
		}
		// Scan A[3x3]
		if((PORTE_OUT == 0x04) && (PORTD_IN == 0x34))
		{
			pwd[rx]='9';
			rx++;
			gfx_mono_init();
			PORTE.OUTSET = PIN4_bm;
			gfx_mono_draw_string("9\r\n", 0, 0, &sysfont);
			send_string("9\r\n");
			_delay_ms(1000);
		}
		//G:\RAHUL IMP_DATA\M.Tech_Project\My xmega usart running code\KAYPAD\KEYPAD\KEYPAD\KEYPAD.c 4
		// Scan A[4x3]
		if((PORTE_OUT == 0x04) && (PORTD_IN == 0x32))
		{
			gfx_mono_init();
			PORTE.OUTSET = PIN4_bm;
			gfx_mono_draw_string("#\r\n", 0, 0, &sysfont);
			send_string("#\r\n");
			pwd[rx]='#';
			rx++;
			_delay_ms(1000);
		}
		
	
	}
			
	
		
	
		
		
	
	