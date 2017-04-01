/*
 * fake.c
 *
 * Created: 2/7/2015 2:13:12 PM
 *  Author: naga baswanth
 */ 


#include <avr/io.h>
#include <string.h>


sbit enrol    = P3^4;
sbit identifi = P3^5;
sbit over 	  = P3^6;

sbit m1a = P0^0;
sbit m1b = P0^1;

sbit row0 = P1^0;
sbit row1 = P1^1;
sbit row2 = P1^2;
sbit row3 = P1^3;
sbit col0 = P1^4;
sbit col1 = P1^5;
sbit col2 = P1^6;


unsigned char i=0,fp[20],j=0;
unsigned char rec=0x00,dummy=0x0f,id,ckey,r,msg[10];

void enroll_finger(void);
void identify_finger(void);

code unsigned char enroll[12]={0xEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X03,0X01,0X00,0X05};   // ok

code unsigned char generate_ch1[13]={0xEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X04,0x02,0X02,0X00,0X09};	 //ok
code unsigned char un_cmd[12]={0xef,0x01,0xff,0xff,0xff,0xff, 0x01,0x00,0x03,0x05,0x00,0x09 };
code unsigned char store[12]={0xEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X06,0X06,0X02,0x00};			//ok
code unsigned char identify[17]={0xef,0x01,0xff,0xff,0xff,0xff,0x01,0x00,0x08,0x1b,0x01,0x00,0x00,0x01,0x01,0x00,0x27};

unsigned char psw1[]="1234",psw2[]="2345",psw3[]="3456",psw4[]="4567",psw5[]="5678";
unsigned char key[4][3] = { '1', '2', '3',
	'4', '5', '6',
	'7', '8', '9',
'*', '0', '#'};





unsigned char  keypad()
{
	unsigned char c0,c1,c2,k=0;



	
	while(1)
	{
		row0 = 0;						//make rows as o/p's
		row1 = 0;
		row2 = 0;
		row3 = 0;
		col0 = 1;						//make columns as i/p's
		col1 = 1;
		col2 = 1;
		
		do
		{
			c0 = col0;
			c1 = col1;
			c2 = col2;
		}while((c0!=1)||(c1!=1)||(c2!=1));		  //check all keys are released
		
		do
		{
			c0 = col0;
			c1 = col1;
			c2 = col2;
		}while((c0==1)&&(c1==1)&&(c2==1));		 //check any key is pressed

		while(1)
		{
			row0 = 0;
			row1 = 1;
			row2 = 1;
			row3 = 1;
			c0 = col0;
			c1 = col1;
			c2 = col2;

			if((c0!=1)||(c1!=1)||(c2!=1))
			{
				r = 0;
				break;
			}

			row0 = 1;
			row1 = 0;
			row2 = 1;
			row3 = 1;
			c0 = col0;
			c1 = col1;
			c2 = col2;

			if((c0!=1)||(c1!=1)||(c2!=1))
			{
				r = 1;
				break;
			}

			row0 = 1;
			row1 = 1;
			row2 = 0;
			row3 = 1;
			c0 = col0;
			c1 = col1;
			c2 = col2;

			if((c0!=1)||(c1!=1)||(c2!=1))
			{
				r = 2;
				break;
			}

			row0 = 1;
			row1 = 1;
			row2 = 1;
			row3 = 0;
			c0 = col0;
			c1 = col1;
			c2 = col2;

			if((c0!=1)||(c1!=1)||(c2!=1))
			{
				r = 3;
				break;
			}
		}
		
		if((c0==0)&&(c1==1)&&(c2==1))
		{
			k = key[r][0];
			return (k);
		}

		else if((c0==1)&&(c1==0)&&(c2==1))
		{
			k = key[r][1];
			return (k);
		}

		else //if((c0==1)&&(c1==1)&&(c2==0))
		{
			k = key[r][2];
			return (k);
		}
	}
}





void serial() interrupt 4
{
	if(RI==1)
	{
		fp[j]= SBUF;
		j++;
		RI=0;
	}
}



void long_delay (void )
{
	unsigned int g,k;
	for(g=0;g<50;g++)
	for(k=0;k<500;k++);
}


void main()
{
	m1a=m1b=0;
	lcd_init();
	delay(50);
	uart_init();
	delay(50);

	lcdcmd(0x01);
	msgdisplay("Home Security ");
	lcdcmd(0xc0);
	msgdisplay("Using Biometric");

	i=0;
	while(1)
	{
		ckey = keypad();
		delay(200);
		if(ckey == '#')
		{
			
			msg[i] = '\0';
			i=0;
			break;
		}
		
	}

	
	lcdcmd(0x01);
	msgdisplay("***WELCOME***");
	delay(500);

	lcdcmd(0x01);
	msgdisplay("Home Security ");
	lcdcmd(0xc0);
	msgdisplay("Using Biometric");

	delay(1000);

	again:
	long_delay();
	lcdcmd(0x01);
	msgdisplay("PUT UR FINGER");
	
	EA=1;
	ES=1;

	while(1)
	{
		
		if(enrol == 0)		//checking for enrolling
		{
			while(enrol == 0);
			
			enroll_finger();
			goto again;
		}

		if(identifi == 0)			//identify
		{
			while(identifi == 0);
			
			identify_finger();
			goto again;
		}
	}
}

void enroll_finger(void)
{
	
	i=0;

	while(i<12)
	{
		ch_send_to_modem(enroll[i]);
		i++;
	}
	j=0;
	fp[9]=0x01;
	while(j==0);
	long_delay();
	if(fp[9]==0x00)
	{
	}
	else
	{
		lcdcmd(0xc0);
		msgdisplay("ENROLL FAILED");
		long_delay();
		long_delay();
		goto end;
	}

	
	j=0;
	i=0;
	while(i<13)
	{

		i++;
	}
	j=0;
	fp[9]=0x01;
	while(j==0);
	long_delay();
	if(fp[9]==0x00)
	{
	}
	else
	{
		lcdcmd(0xc0);
		msgdisplay("ENROLL FAILED");
		long_delay();
		long_delay();
		goto end;
	}

	i=0;
	
	while(i<12)
	{
		ch_send_to_modem(enroll[i]);
		i++;
	}
	j=0;
	fp[9]=0x01;
	while(j==0);
	long_delay();
	if(fp[9]==0x00)
	{
	}
	else
	{
		lcdcmd(0xc0);
		msgdisplay("ENROLL FAILED");
		long_delay();
		long_delay();
		goto end;
	}

	
	j=0;
	i=0;
	while(i<13)
	{
		ch_send_to_modem(generate_ch1[i]);
		i++;
	}
	j=0;
	fp[9]=0x01;
	while(j==0);
	long_delay();
	if(fp[9]==0x00)
	{
	}
	else
	{
		lcdcmd(0xc0);
		msgdisplay("ENROLL FAILED");
		long_delay();
		long_delay();
		goto end;
	}

	
	j=0;
	i=0;
	while(i<12)
	{
		ch_send_to_modem(un_cmd[i]);
		i++;
	}
	j=0;
	fp[9]=0x01;
	while(j==0);
	long_delay();
	if(fp[9]==0x00)
	{
	}
	else
	{
		lcdcmd(0xc0);
		msgdisplay("ENROLL FAILED");

		long_delay();
		long_delay();
		goto end;
	}

	
	j=0;
	i=0;
	while(i<12)
	{
		ch_send_to_modem(store[i]);
		i++;
	}
	ch_send_to_modem(rec);
	ch_send_to_modem(0x00);
	ch_send_to_modem(dummy);
	j=0;
	fp[9]=0x01;
	while(j==0);
	long_delay();
	
	if(fp[9]==0x00)
	{
		lcdcmd(0x01);
		msgdisplay("successfully ");
		lcdcmd(0xc0);
		msgdisplay("Enrolled");
		
		msgdisplay("ID:");
		lcddata(rec);
		

		dummy++;
		long_delay();
		long_delay();
		goto end;
	}
	else
	{
		lcdcmd(0xc0);
		msgdisplay("ENROLL FAILED");
		long_delay();
		long_delay();
		goto end;
	}
	end:
	delay(10);
}

void identify_finger(void)
{


	i=0;
	
	while(i<12)
	{
		ch_send_to_modem(enroll[i]);
		i++;
	}
	j=0;
	fp[9]=0x01;
	while(j==0);
	long_delay();
	if(fp[9]==0x00)
	{
	}
	else
	{
		lcdcmd(0xc0);
		msgdisplay("UnAuthorized....");
		ES=0;

		goto end;
	}

	
	j=0;
	i=0;
	while(i<13)
	{

		i++;
	}
	j=0;
	fp[9]=0x01;
	while(j==0);
	long_delay();
	if(fp[9]==0x00)
	{
	}
	else
	{
		lcdcmd(0xc0);
		msgdisplay("UnAuthorized....");
		ES=0;

		goto end;
	}
	

	
	i=0;
	j=0;
	while(i<17)
	{
		ch_send_to_modem(identify[i]);
		i++;
	}
	j=0;
	while(j==0);
	long_delay();
	
	if(fp[9]==0x00)
	{
		lcdcmd(0x01);
		msgdisplay("Successfully ");
		lcdcmd(0xc0);
		msgdisplay("Authorized:");
		
		id=fp[11];
		lcddata(id);
		
		long_delay();
		long_delay();

		lcdcmd(0x01);
		msgdisplay("ENTER PSWRD");
		lcdcmd(0XC0);

		i=0;
		while(1)
		{
			ckey = keypad();
			delay(200);
			if(ckey == '#')
			{
				
				msg[i] = '\0';
				i=0;
				break;
			}
			
		}

		if(id==0)
		{
			if(!strcmp(psw1,msg))
			{
				lcdcmd(0x01);
				msgdisplay("PSWD AUTHORIZED");
				lcdcmd(0XC0);
				m1a=1;
				m1b=0;
				lcdcmd(0x01);
				msgdisplay("Door Opening");
				delay(2000);
				m1a=0;
				m1b=0;
				lcdcmd(0x01);
				msgdisplay("Door Opened  ");
				delay(2000);
				m1a=0;
				m1b=1;
				lcdcmd(0x01);
				msgdisplay("Door Closing");
				delay(2000);
				m1a=0;
				m1b=0;
				lcdcmd(0x01);
				msgdisplay("Door Closed  ");
				delay(2000);
			}
			else
			goto end1;
		}

		else
		if(id == 1)
		{
			if(!strcmp(psw2,msg))
			{
				
				lcdcmd(0x01);
				msgdisplay("PSWD AUTHORIZED");
				lcdcmd(0XC0);
				m1a=1;
				m1b=0;
				lcdcmd(0x01);
				msgdisplay("Door Opening");
				delay(2000);
				m1a=0;
				m1b=0;
				lcdcmd(0x01);
				msgdisplay("Door Opened  ");
				delay(2000);
				m1a=0;
				m1b=1;
				lcdcmd(0x01);
				msgdisplay("Door Closing");
				delay(2000);
				m1a=0;
				m1b=0;
				lcdcmd(0x01);
				msgdisplay("Door Closed  ");
			delay(2000);	}
			else
			goto end1;

			
		}

		else
		if(id == 2)
		{
			if(!strcmp(psw3,msg))
			{
				
				lcdcmd(0x01);
				msgdisplay("PSWD AUTHORIZED");
				lcdcmd(0XC0);
				m1a=1;
				m1b=0;
				lcdcmd(0x01);
				msgdisplay("Door Opening");
				delay(2000);
				m1a=0;
				m1b=0;
				lcdcmd(0x01);
				msgdisplay("Door Opened  ");
				delay(2000);
				m1a=0;
				m1b=1;
				lcdcmd(0x01);
				msgdisplay("Door Closing");
				delay(2000);
				m1a=0;
				m1b=0;
				lcdcmd(0x01);
				msgdisplay("Door Closed  ");
				delay(2000);
			}
			else
			goto end1;

			
		}
		
		else
		if(id == 3)
		{
			if(!strcmp(psw4,msg))
			{
				
				lcdcmd(0x01);
				msgdisplay("PSWD AUTHORIZED");
				lcdcmd(0XC0);
				m1a=1;
				m1b=0;
				lcdcmd(0x01);
				msgdisplay("Door Opening");
				delay(2000);
				m1a=0;
				m1b=0;
				lcdcmd(0x01);
				msgdisplay("Door Opened  ");
				delay(2000);
				m1a=0;
				m1b=1;
				lcdcmd(0x01);
				msgdisplay("Door Closing");
				delay(2000);
				m1a=0;
				m1b=0;
				lcdcmd(0x01);
				msgdisplay("Door Closed  ");
				delay(2000);
			}
			else
			goto end1;

			
		}

		else
		if(id == 4)
		{
			if(!strcmp(psw5,msg))
			{
				
				lcdcmd(0x01);
				msgdisplay("PSWD AUTHORIZED");
				lcdcmd(0XC0);
				m1a=1;
				m1b=0;
				lcdcmd(0x01);
				msgdisplay("Door Opening");
				delay(2000);
				m1a=0;
				m1b=0;
				lcdcmd(0x01);
				msgdisplay("Door Opened  ");
				delay(2000);
				m1a=0;
				m1b=1;
				lcdcmd(0x01);
				msgdisplay("Door Closing");
				delay(2000);
				m1a=0;
				m1b=0;
				lcdcmd(0x01);
				msgdisplay("Door Closed  ");
				delay(2000);
			}
			else
			goto end1;
			
		}
		else
		{
			end1:
			lcdcmd(0x01);
			msgdisplay("PSWD ");
			lcdcmd(0XC0);
			msgdisplay("UNAUTHORIZED");

			delay(2000);
			
			
			goto end;
		}

		while(over == 1);

		lcdcmd(0x01);
		msgdisplay("THANQ FOR");
		lcdcmd(0XC0);
		msgdisplay("USING System");
		delay(1000);
		
		lcdcmd(0x01);
		msgdisplay("PSWD AUTHORIZED");
		lcdcmd(0XC0);
		m1a=1;
		m1b=0;
		lcdcmd(0x01);
		msgdisplay("Door Opening");
		delay(2000);
		m1a=0;
		m1b=0;
		lcdcmd(0x01);
		msgdisplay("Door Opened  ");
		delay(2000);
		m1a=0;
		m1b=1;
		lcdcmd(0x01);
		msgdisplay("Door Closing");
		delay(2000);
		m1a=0;
		m1b=0;
		lcdcmd(0x01);
		msgdisplay("Door Closed  ");
		delay(2000);
		
		goto end;
	}
	else
	{
		
		lcdcmd(0xc0);
		msgdisplay("UnAuthorized....");
		ES=0;
		long_delay();
		
		goto end;
	}
	end:
	delay(10);
}