/***********************************************************/
/* Author      : Mohamed samy                              */
/* Date        : 1 FEB 2023                                */
/* Version     : V01                                       */
/* Description : Implementation file                       */
/***********************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Project.h"
#define F_CPU 1000000
/**********************************************************/
/****************** Global Variables **********************/
/**********************************************************/
unsigned char SSD1_value =0;
unsigned char SSD2_value =0;
unsigned char SSD3_value =0;
unsigned char SSD4_value =0;
unsigned char SSD5_value =0;
unsigned char SSD6_value =0;
/**********************************************************/
/**********************************************************/
void GPIO_Inti(void)
{
	/*
	 *  First 4 BINS at PortC As Output for Display on Seven Segment
	 *  First 6 BINS at PortA As Output for select which one of the 6 Seven Segment will display
	 *  Make PB2 As Input (Internal Pull up) for INT2
	 *  Make PD2 As Input  (Internal Pull up)  , PD3 As Input (External Pull Down) for INT0 , INT1
	 */
	DDRC  |= 0x0F;
	PORTC &= 0xF0; // Clear First 4 bits at PortC
	DDRA  |= 0x3F;
	PORTA &= 0xC0; // Clear First 6 bits at PortA

	DDRB &=~(1<<PB2); //Make PB2 As Input for INT2
	PORTB |=(1<<PB2); // (Internal Pull up)

	DDRD  &=~(1<<PD2); //Make PD2 As Input for INT0
	PORTD |= (1<<PD2); // (Internal Pull up)

	DDRD  &=~(1<<PD3); //Make PD3 As Input for INT1
	PORTD &=~(1<<PD2); //(External Pull Down)
}
/**********************************************************/
void SSD_DisplayValue(unsigned char counter)
{
	/* Display the value of seven Segment
	 *
	 * Clear First 4 Bits of PortC
	 * Put the counter value into the first  4 Bits of PortC
	 * delay for 10 microsecond
	 */

	PORTC &= 0xF0;
	PORTC |= (counter & 0x0F);
	_delay_ms(.01);
}
/**********************************************************/
void SevenSegmentTimeCal(void)
{
	/* Time Calculations for seconds, minutes, hours*/
	/* the Limit of the Stop watch 99:59:59 */

	if(SSD1_value > 9)
	{
		SSD1_value =0;
		SSD2_value++;
	}
	if(SSD2_value >5)
	{
		SSD2_value =0;
		SSD3_value++;
	}

	if(SSD3_value > 9)
	{
		SSD3_value =0;
		SSD4_value++;
	}
	if(SSD4_value > 5)
	{
		SSD4_value =0;
		SSD5_value++;
	}

	if(SSD5_value > 9)
	{
		SSD5_value =0;
		SSD6_value++;
	}

	if(SSD6_value > 9)
	{
		SSD1_value =0;
		SSD2_value =0;
		SSD3_value =0;
		SSD4_value =0;
		SSD5_value =0;
		SSD6_value =0;
	}
}
/**********************************************************/
void SevenSegmentSelect(unsigned char Select_one) {
	PORTA &= 0xC0;
	PORTA = Select_one;
}
/**********************************************************/
void Timer1_CTCMode_Init(void)
{
	/*
	 *Timer/Counter1 Set Normal port operation OC1A/OC1B disconnected.
	 *Compare Output Mode for Compare unit A  (COM1A1 =0 COM1A0=0)
	 *Compare Output Mode for Compare unit B  (COM1B1 =0 COM1B0=0)
	 *FOC1A = 1: Force Output Compare for Compare unit A --> because non-PWM mode
	 *Waveform Generation Mode (select mode 4 CTC MODE)
				WGM13 = 0
				WGM12 = 1
				WGM11 = 0
				WGM10 = 0
	 *
	 *Select clk/64 (From prescaler) - clk = 1Mhz
	 * timer frequency  = clk / 64 = 1000000 / 64 = 15625
	 * TCNT1 =0
	 * OCR1A = top --> (15625 to generate interrupt each 1 sec)
	 * Output Compare A Match Interrupt Enable
	 * 		OCIE1A =1
	 * Enable Global Interrupt
	 * 		SREG |=(1<<7)
	 */
	TCNT1  = 0;
	OCR1A  = 15625;
	TIMSK |= (1<<OCIE1A);
	SREG  |= (1<<7);
	TCCR1A |= (1<<FOC1A);
	TCCR1B |= (1<<WGM12) |(1<<CS11) |(1<<CS10);
}
/**********************************************************/
/*External Interrupt Request 0*/
void EXTI0_Inti(void)
{
	//The falling edge of INT0
	MCUCR |=(1<<ISC01);
	//Enable External Interrupt Request 0
	GICR  |=(1<<INT0);
	//Enable Global Interrupt Request
	SREG |=(1<<7);
}
/**********************************************************/
/*External Interrupt Request 1*/
void EXTI1_Inti(void)
{
	//The a rising  edge of INT1
	MCUCR |=(1<<ISC10) | (1<<ISC11);
	//Enable External Interrupt Request 1
	GICR  |=(1<<INT1);
	//Enable Global Interrupt Request
	SREG |=(1<<7);
}
/**********************************************************/
/*External Interrupt Request 2*/
void EXTI2_Inti(void)
{
	//The a Falling  edge of INT2
	MCUCSR &=~(1<<ISC2);
	//Enable External Interrupt Request 2
	GICR  |=(1<<INT2);
	//Enable Global Interrupt Request
	SREG |=(1<<7);
}
/**********************************************************/
/**********************************************************/
ISR(TIMER1_COMPA_vect)
{
	/* Increment the value of Second by 1*/
	SSD1_value++;
}
/**********************************************************/
ISR(INT0_vect)
{
	/*Reset Timer*/
	SSD1_value =0;
	SSD2_value =0;
	SSD3_value =0;
	SSD4_value =0;
	SSD5_value =0;
	SSD6_value =0;
}
/**********************************************************/
ISR(INT1_vect)
{
	/*
	 * Stop Timer by selecting No clock source
	 * 		CS10 = 0
	 * 		CS11 = 0
	 * 		CS12 = 0
	 */
	TCCR1B &=~(1<<CS10) & ~(1<<CS11) & ~(1<<CS12);
}
/**********************************************************/
ISR(INT2_vect)
{
	/*Return timer to count*/
	TCCR1B |= (1 << CS11) | (1 << CS10);
}
/**********************************************************/
/**********************************************************/
