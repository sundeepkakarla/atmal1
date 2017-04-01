/*
 * GccApplication3.c
 *
 * Created: 2/5/2015 11:32:13 AM
 *  Author: naga baswanth
 */ 


#include <avr/io.h>
#define F_CPU 32000000ul
#include <util/delay.h>
void Set_Clk_32MHz(void);
void uart_init(void);
void delay(void);
void send_char(char data);
void send_string(char *str);
void receive_data(void);
void print_data(void);
int main(void)
{
    
char msg[10];
unsigned char rx;
void receive_data(void)
{
	//char c;
  for(rx=0;rx<=4;rx++)

	while( !( USARTC0_STATUS & 0x80) );

	msg[rx]=USARTC0_DATA;
	
if (msg[3]==1 && msg[4]==5 )
{   
	send_string("\r\n");
	send_string("password matched_1\r\n");
	print_data();
	//main();
}
else if (msg[0]==1 && msg[1]==5 && msg[2]==7)
{
	send_string("three\r\n");
	send_string("password matched_2\r\n");
	print_data();
	//main();
}


else
{
	send_string("Password didn't matched\r\n");
	//main();
}
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

/************************************************************************************
main ()
*************************************************************************************/
int main(void)
{ 
	
Set_Clk_32MHz();
uart_init();
send_string("UART Rx\r\n");
while(1)
{
receive_data();
//print_data();
}
}

void print_data (void)
{
send_string(msg);
send_string("\r\n");
send_string("BENZ\r\n");
send_string(" PRAVEEN BORN TO ROCK \n\r");
send_string("The Data is being transmitted through UART\n\r");
send_string("Using Xmega_a3bu Microcontroller\r\n");
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
        //TODO:: Please write your application code 
    }
