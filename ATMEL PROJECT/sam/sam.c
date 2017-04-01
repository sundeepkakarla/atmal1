//Program for gesture control robotic wheel chair.

#include <avr/io.h>
#include "gfx_mono.h"
#include "gfx_mono_text.h"
#include "sysfont.h"

uint16_t adc_result,adc_result1,adc_result2;
void Disp_Sensval(unsigned int prs);  //for displying ADCB0 result on internal LCD
void Disp_Sensval1(unsigned int prs); //for displying ADCB1 result on internal LCD
void Disp_Sensval2(unsigned int prs); //for displying ADCB2 result on internal LCD
//here main loop starts
int main(void)
{
	PORTA_DIRSET=0x03;  // setting 0th and 1st pin of PORTA as output
	PORTE_DIRSET=0x03;  // setting 0th and 1st pin of PORTE as output
	PORTC_DIRSET=0xFF;  // setting all the pins of PORTC as output
	OSC.CTRL |= OSC_RC32MEN_bm; // setting oscilator frequency to 32 MHz
	while(!(OSC.STATUS & OSC_RC32MRDY_bm));
	CCP = CCP_IOREG_gc;  // for protected IO registers
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc; // setting system clock to 32 MHz frequency
	gfx_mono_init();
	PORTE.OUTSET = PIN4_bm;
	gfx_mono_draw_string("FLEX SENOR OUTPUT:", 0, 0, &sysfont); // for displaying words " flex sensor" on LCD
	ADCB.CTRLB = ADC_RESOLUTION_12BIT_gc;  // setting ADCB to 12 bit resolution
	ADCB.REFCTRL = ADC_REFSEL_INTVCC_gc;   // setting ADCB refercence voltage to Vcc/1.6
	ADCB.PRESCALER = ADC_PRESCALER_DIV64_gc;  //prescaling the clock pulse by dividing it by 64
	ADCB.CH0.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc;//setting channel input mode as single-ended positive input signal
	ADCB.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;  //setting CHANNEL0 of ADCB as MUXPOSITIVE
	ADCB.CH1.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc;//setting channel input mode as single-ended positive input signal
	ADCB.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN1_gc;   //setting CHANNEL1 of ADCB as MUXPOSITIVE
	ADCB.CH2.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc;//setting channel input mode as single-ended positive input signal
	ADCB.CH2.MUXCTRL = ADC_CH_MUXPOS_PIN2_gc;   //setting CHANNEL2 of ADCB as MUXPOSITIVE
	ADCB.CTRLA = ADC_ENABLE_bm;  //Setting this bit enables the ADC.
	ADCB.CTRLA |= ADC_FLUSH_bm;  //Setting this bit will flush the ADC pipeline
	while(1)
	{
		ADCB.CH0.CTRL |= ADC_CH_START_bm;
		while(!(ADCB.CH0.INTFLAGS & ADC_CH_CHIF_bm));
		ADCB.CH0.INTFLAGS = ADC_CH_CHIF_bm;
		adc_result  = ADCB.CH0RES;  //storing the result of ADCB0 in adc_result
		ADCB.CH1.CTRL |= ADC_CH_START_bm;
		while(!(ADCB.CH1.INTFLAGS & ADC_CH_CHIF_bm));
		ADCB.CH1.INTFLAGS = ADC_CH_CHIF_bm;
		adc_result1 = ADCB.CH1RES;  //storing the result of ADCB1 in adc_result1
		ADCB.CH2.CTRL |= ADC_CH_START_bm;
		while(!(ADCB.CH2.INTFLAGS & ADC_CH_CHIF_bm));
		ADCB.CH2.INTFLAGS = ADC_CH_CHIF_bm;
		adc_result2 = ADCB.CH2RES;  //storing the result of ADCB2 in adc_result2
		Disp_Sensval(adc_result);// display  adc_result on LCD
		Disp_Sensval1(adc_result1);// display adc_result1 on LCD
		Disp_Sensval2(adc_result2);// display adc_result2 on LCD

		if ((adc_result < 2000)&(adc_result1 < 2000)&(adc_result2 < 2000))
		{
			PORTA_OUTSET=0x01; // setting PORTA PIN0 to HIGH
			PORTA_OUTCLR=0x02;  // setting PORTA PIN1 to LOW
			PORTE_OUTSET=0x01;  // setting PORTE PIN0 to HIGH
			PORTE_OUTCLR=0x02;  // setting PORTE PIN1 to LOW
			PORTC_OUTCLR=0x01;  // setting PORTC PIN0 to LOW
			PORTC_OUTSET=0xFE;   // setting PORTC rest pins to HIGH
		}
		else if ((adc_result < 2000)&(adc_result1 < 2000)&(adc_result2 >= 2000))
		{
			PORTA_OUTCLR=0x03;  // setting PORTA PIN0 & PIN1 to LOW
			PORTE_OUTSET=0x01; // setting PORTE PIN0 to HIGH
			PORTE_OUTCLR=0x02; // setting PORTE PIN1 to LOW
			PORTC_OUTCLR=0x02; // setting PORTC PIN1 to LOW
			PORTC_OUTSET=0xFD; // setting PORTC rest pins to HIGH
		}
		else if (( adc_result < 2000)&( adc_result1 >= 2000)&(adc_result2 < 2000))
		{
			PORTA_OUTSET=0x01;// setting PORTA PIN0 to HIGH
			PORTA_OUTCLR=0x02;// setting PORTA PIN to LOW
			PORTE_OUTCLR=0x03; // setting PORTE PIN0 & PIN1 to LOW
			PORTC_OUTCLR=0x04;  // setting PORTC PIN2 to LOW
			PORTC_OUTSET=0xFB;  // setting PORTC rest pins to HIGH
		}
		else if ((adc_result >= 2000)&(adc_result1 < 2000)&(adc_result2 < 2000))
		{
			PORTA_OUTSET=0x02;// setting PORTA PIN1 to HIGH
			PORTA_OUTCLR=0x01;// setting PORTA PIN0 to LOW
			PORTE_OUTSET=0x02;// setting PORTE PIN1 to HIGH
			PORTE_OUTCLR=0x01;// setting PORTE PIN0 to LOW
			PORTC_OUTCLR=0x08;// setting PORTC PIN3 to LOW
			PORTC_OUTSET=0xF7; // setting PORTC rest pins to HIGH
		}
		else if ((adc_result >= 2000)&(adc_result1 >= 2000)&(adc_result2 < 2000))
		{
			PORTA_OUTCLR=0x03; //setting PORTA PIN0 & PIN1 to LOW
			PORTE_OUTCLR=0x03; //setting PORTE PIN0 & PIN1 to LOW
			PORTC_OUTCLR=0x10;// setting PORTC PIN4 to LOW
			PORTC_OUTSET=0xEF;// setting PORTC rest pins to HIGH
		}
		else if ((adc_result >= 2000)&(adc_result1 < 2000)&(adc_result2 >= 2000))
		{
			PORTA_OUTCLR=0x03; //setting PORTA PIN0 & PIN1 to LOW
			PORTE_OUTCLR=0x03; //setting PORTE PIN0 & PIN1 to LOW
			PORTC_OUTCLR=0x20;// setting PORTC PIN6 to LOW
			PORTC_OUTSET=0xDF;// setting PORTC rest pins to HIGH
			
		}
		else if ((adc_result < 2000)&(adc_result1 >= 2000)&(adc_result2 >= 2000))
		{
			PORTA_OUTCLR=0x03;  //setting PORTA PIN0 & PIN1 to LOW
			PORTE_OUTCLR=0x03;  //setting PORTE PIN0 & PIN1 to LOW
			PORTC_OUTCLR=0x40; // setting PORTC PIN6 to LOW
			PORTC_OUTSET=0xBF; // setting PORTC rest pins to HIGH

		}
		else if ((adc_result >= 2000)&(adc_result1 >= 2000)&(adc_result2 >= 2000))
		{
			PORTA_OUTCLR=0x03;  //setting PORTA PIN0 & PIN1 to LOW
			PORTE_OUTCLR=0x03;  //setting PORTE PIN0 & PIN1 to LOW
			PORTC_OUTSET=0xFF;  //setting PORTC ALL PINS to LOW

		}
		
	}
}


void Disp_Sensval1(unsigned int prs) //for displaying ADCB0 results on internal lcd
{
	
	unsigned char u=0,tn=0,h=0,th=0;
	u = prs%10;
	prs = prs/10;

	tn = prs%10;
	prs = prs/10;

	h = prs%10;
	prs = prs/10;

	th = prs;
	
	gfx_mono_draw_char(th | 0x30, 9, 18, &sysfont);
	gfx_mono_draw_char(h | 0x30, 18, 18, &sysfont);
	gfx_mono_draw_char(tn | 0x30, 27, 18, &sysfont);
	gfx_mono_draw_char(u | 0x30, 36, 18, &sysfont);
}
void Disp_Sensval(unsigned int prs) //for displaying ADCB1 results on internal lcd
{
	
	unsigned char u=0,tn=0,h=0,th=0;
	u = prs%10;
	prs = prs/10;

	tn = prs%10;
	prs = prs/10;

	h = prs%10;
	prs = prs/10;

	th = prs;
	
	gfx_mono_draw_char(th | 0x30, 9, 9, &sysfont);
	gfx_mono_draw_char(h | 0x30, 18, 9, &sysfont);
	gfx_mono_draw_char(tn | 0x30, 27, 9, &sysfont);
	gfx_mono_draw_char(u | 0x30, 36, 9, &sysfont);
}
void Disp_Sensval2(unsigned int prs) //for displaying ADCB2 results on internal lcd
{
	
	unsigned char u=0,tn=0,h=0,th=0;
	u = prs%10;
	prs = prs/10;

	tn = prs%10;
	prs = prs/10;

	h = prs%10;
	prs = prs/10;

	th = prs;
	
	gfx_mono_draw_char(th | 0x30, 9, 27, &sysfont);
	gfx_mono_draw_char(h | 0x30, 18, 27, &sysfont);
	gfx_mono_draw_char(tn | 0x30, 27, 27, &sysfont);
	gfx_mono_draw_char(u | 0x30, 36, 27, &sysfont);
}