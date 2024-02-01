/***********************************************************/
/* Author      : Mohamed samy                              */
/* Date        : 1 FEB 2023                                */
/* Version     : V01                                       */
/* Description : Implementation file                       */
/***********************************************************/

#ifndef PROJECT_H_
#define PROJECT_H_

/***********************************************************/
/***********************************************************/
#define SSD_SELECT_1		1
#define SSD_SELECT_2		2
#define SSD_SELECT_3		4
#define SSD_SELECT_4		8
#define SSD_SELECT_5		16
#define SSD_SELECT_6		32
/***********************************************************/
/***********************************************************/
void GPIO_Inti();
void Timer1_CTCMode_Init();
void EXTI0_Inti();
void EXTI1_Inti();
void EXTI2_Inti();
void SSD_DisplayValue(unsigned char counter);
void SevenSegmentSelect(unsigned char Select_one);
void SevenSegmentTimeCal(void);
ISR(TIMER1_COMPA_vect);
ISR(INT0_vect);
ISR(INT1_vect);
ISR(INT2_vect);
/***********************************************************/
/***********************************************************/
#endif /* PROJECT_H_ */
