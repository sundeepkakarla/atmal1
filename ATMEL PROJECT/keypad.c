//G:\RAHUL IMP_DATA\M.Tech_Project\My xmega usart running code\KAYPAD\KEYPAD\KEYPAD\KEYPAD.c 1
//1 /*
// * KEYPAD.c
//3 *
//4 * Created: 17/11/2014 13:06:51
//5 * Author: rahul k
//6 */
//789
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
 unsigned char pwd[5];
 unsigned char rx=0;

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
	 send_string("Enter Password\r\n");
	
	 while(rx <=4)
	 {
		 scan_keypad();
	 }
	 send_char(pwd[0]);
	 send_char(pwd[1]);
	 
	 send_char(pwd[2]);
	 send_char(pwd[3]);
	// send_char(pwd[4]);
	 send_char('\r');
	 send_char('\n');
	 if ((pwd[0]=='1') && (pwd[1]=='2') && (pwd[2]=='3') && (pwd[3]=='4'))
	 {
		 send_string("Password matched for person_1\r\n");
		 send_string("PRAVEEN BORN TO ROCK\r\n");
		 send_string("BENZ\r\n");
		 rx=0;
		 Password();
	 }
	 if ((pwd[0]=='2') && (pwd[1]=='3') && (pwd[2]=='4') && (pwd[3]=='5'))
	 {
		 send_string("Password Matched for Person_2\r\n");
		 send_string("PRAVEEN BORN TO ROCK\r\n");
		 send_string("BENZ\r\n");
		 rx=0;
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

int main(void)
{
	
	 PORTE_DIRSET=0x07; // PORTE as output port
	 PORTD_DIR =0x00; // PORTD as input port
	
	
	 Set_Clk_32MHz();
	 uart_init();
	 send_string("testing keypad\r\n");
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