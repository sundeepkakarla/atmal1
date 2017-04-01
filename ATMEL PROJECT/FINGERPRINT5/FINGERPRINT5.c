/*
 * FINGERPRINT5.c
 *
 * Created: 2/5/2015 1:06:02 PM
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
unsigned char generate_ch[13]={0xEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X04,0x02,0X01,0X00,0X08};
unsigned char store[15]={0xEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X06,0X06,0X01,0x00,0x01,0x00,0x0F};
unsigned char identify[12]={0xEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X03,0X11,0X00,0X15};
unsigned char b1=0,b2=0,i1=0,i2=0,id=0,id1=0,id2=0,cnt=0,eeprom_add=0,aa,n=0;
//void clearfp(void);
//void convert (unsigned char temp_value);
unsigned char part_A=0,part_B=0,part_C=0,part_D=0,check=0,count=0,enroll_check=0,en_byte;
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
   _delay_ms(50);
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
	

	while( !( USARTC0_STATUS & USART_RXCIF_bm) );

	c=USARTC0_DATA;
}
void clearfp()
{
	unsigned char cl=0;
	while(cl<20)
	{
		fp[cl]=' ';
		cl++;
	}
	j=0;
}
void convert(unsigned char temp_value)
{
	unsigned char value,d1,d2,d3,k=0;
	temp_value = temp_value;

	value=temp_value/10;
	d3=temp_value%10;
	d1=value/10;
	d2=value%10;
	d1=d1+30;
	// lcddata(d1);
	_delay_ms(10);
	d2=d2+30;
	//LCD_cmd(d2);
	_delay_ms(50);
	k++;
	// msgdisplay(".");
	d3=d3+0x30;
	//LCD_cmd(d3);
	_delay_ms(10);
	k=0;
}

/*-------------------------------------------------------------------------24.
main program
--------------------------------------------------------------------------*/
int main (void )
{
	
	
	//Init_LCD();
	//uart_init();
	again:
	//LCD_cmd(0x01);
	//_delay_ms(50);
	gfx_mono_init();
	gfx_mono_draw_string(" welcome ",0, 0, &sysfont);
	_delay_ms(50);
	//LCD_cmd(0x01);
	//LCD_cmd(0xc0);
	gfx_mono_draw_string("EVM with FP",0, 0, &sysfont);
	_delay_ms(50);
	i=0;
	while(1)
	{
		if((PORTA_IN & 0x00)==0x00)//checking for sw1
		{
			while(PORTA_IN==0x00);
			//LCD_cmd(0x01);
			gfx_mono_draw_string("Enrolling....",0, 0, &sysfont);
			_delay_ms(50);
			i=0;
			while(i<12)
			{
				send_char(enroll[i]);
				i++;
			}
			rec= receive_data();
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
			if(!rec)//{LCD_cmd(0x01);
				gfx_mono_draw_string("Enrolling ok",0, 0, &sysfont);
			}
			else
			{
				gfx_mono_init();
				//LCD_cmd(0x01);
				gfx_mono_draw_string("Enrolling not ok",0, 0, &sysfont);
				goto again;
			}
			i=0;
			while(i<13)
			{
				send_char(generate_ch[i]);
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
			if(!rec)
			{
				gfx_mono_init();
				//LCD_cmd(0x01);
				gfx_mono_draw_string("genchar ok",0, 0, &sysfont);
			}
			else
			{
				//LCD_cmd(0x01);
				gfx_mono_draw_string("genchar not ok",0, 0, &sysfont);
				goto again;
			}

			i=0;
			while(i<15)
			{
				send_char(store[i]);
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
			if(!rec)
			{
				//LCD_cmd(0x01);
				gfx_mono_draw_string("store ok",0, 0, &sysfont);
			}
			else
			{
				//LCD_cmd(0x01);
				("store not ok");
				goto again;
			}
			//check=1;129.
			//enroll_check =0;130.
			//en_byte=12;131.

			/***********133.
			identifying your ID134.
			*******************/
			if(PORTA_IN=0x00)//checking for sw2136.
			{
				while(PORTA_IN=0x00);
				//LCD_cmd(0x01);
				gfx_mono_draw_string("Identfying...",0, 0, &sysfont);
				i=0;
				while(i<12)
				{
					send_char(identify[i]);
					i++;
				}
				check=1;
				enroll_check =1;
				en_byte=16;
			}
			/***********151.
			final result152.
			*******************/
			if(PORTA_IN==0x00)//checking for sw3154.
			{
				while(PORTA_IN==0x00);
				//LCD_cmd(0x01);
				gfx_mono_draw_string("A B C D",0, 0, &sysfont);
				//LCD_cmd(0xc0);
				convert(part_A);
				//LCD_cmd(0xc4);
				convert(part_B);
				//LCD_cmd(0xc7);
				convert(part_C);
				//LCD_cmd(0xcc);
				convert(part_D);
				_delay_ms(10);
				//_delay_ms(10);
				part_A=part_B=part_C=part_D=0;
				goto again;
			}
			/********* receiving data from FP *********/
			if(check==1)
			{
				count=0;
				check=0;
				while(count<en_byte)
				{
					fp[count]=receive_data();;
					count++;
				}
				_delay_ms(10);
				n=0;
				while(n<20)
				{
					if((fp[n]==0xEF)&&(fp[n+1]==0x01)&&(fp[n+9]==0x00))
					{
						//LCD_cmd(0x01);
						//LCD_cmd(0x80);
						gfx_mono_draw_string(" Successfully ",0, 0, &sysfont);
						//LCD_cmd(0xc0);
						gfx_mono_draw_string(" completed",0, 0, &sysfont);
						b1=0;
						b2=0;
						i1=0;
						i2=0;
						id=0;
						b1=fp[n+11]/10;
						b2=fp[n+11]%10;
					//	_delay_ms(10);
						//_delay_ms(10);
						//_delay_ms(10);
						// LCD_cmd(0xc0,0);199.
						// message(0xc0,"Id:");200.
						// LCD_cmd(0xC3,0);201.
						// LCD_cmd(b1+0x30,1);202.
						// LCD_cmd(b2+0x30,1);203.
						i1=b1-48;
						i1=i1*10;
						i2=b2-48;
						i2=i2*1;
						id=i1+i2;
						_delay_ms(10);
						if(enroll_check ==1)
						{
							//LCD_cmd(0x01);
							gfx_mono_draw_string(" Plz poll ur vote ",0, 0, &sysfont);
							// clearfp();214.
							enroll_check=0;
							goto poll;
						}
								goto again;
					}
					else
					{
						//LCD_cmd(0x01);
						gfx_mono_draw_string(" Failed",0, 0, &sysfont);
						_delay_ms(10);
						goto again;
					}
				}
			}


			/*-----------------------------------------------------230.
			polling your vote231.
			----------------------------------------------------*/
			poll:
			while(1)
			{
				if(PORTA_IN==0x00) //sw1236.
				{
					part_A++;
					//LCD_cmd(0x01);
					gfx_mono_draw_string("Vote for part A",0, 0, &sysfont);
					_delay_ms(10);
					goto again;
				}
				if(PORTA_IN==0x00) //sw2243.
				{
					part_B++;
					//LCD_cmd(0x01);
					gfx_mono_draw_string("Vote for part B",0, 0, &sysfont);
					_delay_ms(10);
					goto again;
					// goto again;
				}
				if(PORTA_IN==0x00) //sw3

				{
					part_C++;
					//LCD_cmd(0x01);
					gfx_mono_draw_string("voteforc",0, 0, &sysfont);
					_delay_ms(10);
					goto again;
					// goto again;259.
				}
				if(PORTA_IN==0x00)
				{
					part_D++;
				//	LCD_cmd(0x01);
					gfx_mono_draw_string("Vote for part D",0, 0, &sysfont);
					_delay_ms(10);
					goto again;
					//goto again;
				}
			}
		}
	}

