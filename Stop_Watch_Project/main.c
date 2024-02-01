/***********************************************************/
/* Author      : Mohamed samy                              */
/* Date        : 1 FEB 2023                                */
/* Version     : V01                                       */
/* Description : Stop Watch system                         */
/***********************************************************/

/**********************************************************/
/************************* Library ************************/
/**********************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Project.h"
/**********************************************************/
/************** Extern Global Variables *******************/
/**********************************************************/
extern unsigned char SSD1_value;
extern unsigned char SSD2_value;
extern unsigned char SSD3_value;
extern unsigned char SSD4_value;
extern unsigned char SSD5_value;
extern unsigned char SSD6_value;
/**********************************************************/

int main()
{
	GPIO_Inti();
	Timer1_CTCMode_Init();
	EXTI0_Inti();
	EXTI1_Inti();
	EXTI2_Inti();

	while(1)
	{
		SevenSegmentSelect(SSD_SELECT_1);
		SSD_DisplayValue(SSD1_value);

		SevenSegmentSelect(SSD_SELECT_2);
		SSD_DisplayValue(SSD2_value);

		SevenSegmentSelect(SSD_SELECT_3);
		SSD_DisplayValue(SSD3_value);

		SevenSegmentSelect(SSD_SELECT_4);
		SSD_DisplayValue(SSD4_value);

		SevenSegmentSelect(SSD_SELECT_5);
		SSD_DisplayValue(SSD5_value);

		SevenSegmentSelect(SSD_SELECT_6);
		SSD_DisplayValue(SSD6_value);

		/*Check The Value Of each 7Segment*/
		SevenSegmentTimeCal();
	}
	return 0;
}
