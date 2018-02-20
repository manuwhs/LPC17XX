#ifndef __SRF04__
#define __SRF04__

#include "LPC17xx.h"

#define PERIOD_ADC   	10000		 ///< Period of the ADC convertion in ms

#define PRESCALER_T1  	24	  	 /// < Frecuency dividor for TIMER1
#define TIMER1_FRECUENCY 25000000/(PRESCALER_T1+1)



void periodic_signal_ms(uint32_t period);
void periodic_signal2_ms(uint32_t period);
void periodic_signal_us(uint32_t period);
void Init_TIMER1X(void);
void delay_us(uint32_t delay);
uint32_t get_timer1(void);

extern volatile uint32_t general_period_ms;
extern volatile uint32_t general_period2_ms;
extern volatile uint32_t general_period_us;
extern uint16_t delayers[10];
extern uint16_t delay_only;
#endif
	  
