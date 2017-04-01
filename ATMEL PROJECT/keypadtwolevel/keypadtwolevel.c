/*
 * keypadtwolevel.c
 *
 * Created: 2/6/2015 12:12:06 PM
 *  Author: naga baswanth
 */ 
#include <avr/io.h>
#define F_CPU 32000000ul
#include <util/delay.h>

void Set_Clk_32MHz(void);
void uart_init(void);
// void delay(void);
void send_char(char data);
void send_string(char *str);
void scan_keypad(void);
void Password (void);
unsigned char pwd[3];
unsigned char rx=0;
unsigned char u0=0, u1=0,u2=0,u3=0;
void int_to_char(unsigned char int_val);
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
23 Code for configuring Sys Clk at 32MHz
24 ***************************************************************/

void Set_Clk_32MHz(void)
{
	OSC_CTRL |= OSC_RC32MEN_bm; //Setup 32Mhz crystal
	
	while(!(OSC_STATUS & OSC_RC32MRDY_bm));
	
	CCP = CCP_IOREG_gc; //Trigger protection mechanism
	CLK_CTRL = CLK_SCLKSEL_RC32M_gc; //Enable internal 32Mhz crystal
	
	
}

/*****************************************************************************
39 Code for Initialize USART
40 *****************************************************************************/
void uart_init(void)
{
	PORTC_OUTSET = PIN3_bm; //Let's make PC7 as TX
	PORTC_DIRSET = PIN3_bm; //TX pin as output
	
	USARTC0_BAUDCTRLA = 207;
	USARTC0_BAUDCTRLB = 0;
	USARTC0_CTRLA = 0;
	USARTC0_CTRLC = 0x03;
	USARTC0_CTRLB = USART_TXEN_bm;
}

/***************************************************************************
54 code for Sending a single char
55 *************************************************************************/
void send_char(char data)
{ USARTC0_DATA = data;
	//while ( !( USARTC0_STATUS & USART_DREIF_bm) );
	_delay_ms(100);
}
/**************************************************
62 code for sending a string
63 **************************************************/
void send_string(char *str)
{ while(1)
	{
		if( *str == '\0' ) break;
		send_char(*str++);
	}
}



//G:\RAHUL IMP_DATA\M.Tech_Project\My xmega usart running code\KAYPAD\KEYPAD\KEYPAD\KEYPAD.c 2

void Password(void)
{
	send_string("Enter key: \r\n");
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
	if ((pwd[0]=='1') && (pwd[1]=='2') && (pwd[2]=='3') && (pwd[3]=='4'))
	{
		send_string("Matched key\r\n");
		rx=0;
		u0++;
		int_to_char(u0);
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
			send_string("Matched first person password\r\n");
			rx=0;
			u0++;
			int_to_char(u0);
			Password();

		}
		else if ((pwd[0]=='2') && (pwd[1]=='3') && (pwd[2]=='4') && (pwd[3]=='7'))
		{
			send_string("Password second Matched\r\n");
			rx=0;
			u2++;
			int_to_char(u2);
			Password();
		}
		
		else if ((pwd[0]=='2') && (pwd[1]=='3') && (pwd[2]=='4') && (pwd[3]=='8'))
		{
			send_string("Password Matched for Person_3\r\n");
			rx=0;
			u3++;
			int_to_char(u3);
			Password();
		}
		else
		{
			send_string("Password Didn't Matched\r\n");
			send_string("Try Again!\n\r");
			rx=0;
			Password();
		}
	}
	else
	{
		send_string("key did'nt matched\r\n");
		send_string("Try again!\n\r");
		rx=0;
		Password();
	}
}


int main(void)
{
	
	PORTE_DIRSET=0x07; // PORTE as output port
	PORTD_DIR =0x00; // PORTD as input port
    PORTB_DIR = 0x0F;//port b as input
    PORTC_OUT = 0x01;
	
	Set_Clk_32MHz();
	uart_init();
	send_string("testing keypad\r\n");
	if(PORTE_IN = 0x38)
	{
		send_string("The user_1 entered=");
		int_to_char(u1);
		send_string("\r\n");
		send_string("The user_2 entered=");
		int_to_char(u2);
		send_string("\r\n");
		send_string("The user_3 entered=");
		int_to_char(u3);
		send_string("\r\n");
		Password();
	}
	else
	Password();
	
	
}

void scan_keypad(void)
{
	PORTE_OUT = 0x02;
	PORTD_DIR =0x00; // PORTD as input port
	// Scan A[1x2]
	if ((PORTE_OUT == 0x02) && (PORTD_IN ==0x31)) // if 0th pin of PORTD is receiving logic high
	{
		send_string("2\r\n");
		pwd[rx]='2';
		rx++;
		
		_delay_ms(1000);
	}
	// Scan A[2x2]
	if((PORTE_OUT == 0x02) && (PORTD_IN == 0x38))
	{
		
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
		send_string("8\r\n");
		_delay_ms(1000);
		//G:\RAHUL IMP_DATA\M.Tech_Project\My xmega usart running code\KAYPAD\KEYPAD\KEYPAD\KEYPAD.c 3
	}
	// Scan A[4x2]
	if((PORTE_OUT == 0x02) && (PORTD_IN == 0x32))
	{
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
		send_string("4\r\n");
		_delay_ms(1000);
	}
	// Scan A[3x1]
	if((PORTE_OUT == 0x01) && (PORTD_IN == 0x34))
	{
		pwd[rx]='7';
		rx++;
		send_string("7\r\n");
		_delay_ms(1000);
	}
	// Scan A[4x1]
	if((PORTE_OUT == 0x01) && (PORTD_IN == 0x32))
	{
		pwd[rx]='*';
		rx++;
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
		send_string("3\r\n");
		_delay_ms(1000);
	}
	// Scan A[2x3]
	if((PORTE_OUT == 0x04) && (PORTD_IN == 0x38))
	{
		pwd[rx]='6';
		rx++;
		send_string("6\r\n");
		_delay_ms(1000);
	}
	// Scan A[3x3]
	if((PORTE_OUT == 0x04) && (PORTD_IN == 0x34))
	{
		pwd[rx]='9';
		rx++;
		send_string("9\r\n");
		_delay_ms(1000);
	}
	//G:\RAHUL IMP_DATA\M.Tech_Project\My xmega usart running code\KAYPAD\KEYPAD\KEYPAD\KEYPAD.c 4
	// Scan A[4x3]
	if((PORTE_OUT == 0x04) && (PORTD_IN == 0x32))
	{
		send_string("#\r\n");
		pwd[rx]='#';
		rx++;
		_delay_ms(1000);
	}
}