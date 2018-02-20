#include "Timer.h"

/**
 * @brief  Configure TIMER1 for general timing pourpuses. It just configures the general timer
		   but unables the maching interrupt until the porpouse is given.
 *
 * @param  none
 * @return none
 */

volatile uint32_t general_period_ms = 10;
volatile uint32_t general_period2_ms = 10;
volatile uint32_t general_period_us = 10;
void Init_TIMER1X(void) {
	uint16_t i;

	// POWER UP THE TIMER  AND SET IT UP AS A TIMER
	LPC_SC->PCONP |= (0x01<<2);
	LPC_TIM1->CTCR &= ~ (3 << 0);

	// RESET TIMER (AND PC) TO AVOID REGISTER OVERFLOW AND THEN STOP IT
	LPC_TIM1->TCR = 0x11;		 /* RESET */
	LPC_TIM1->TCR = 0x00;		 /* DISABLE (STOP COUNTER)*/

	// Configure the TIMER1 frecuency to 1 MHz

	LPC_SC->PCLKSEL0 &= ~ (3 << 4);		/* Select PCLK /4 = 25 MHz */
	LPC_TIM1->PR = PRESCALER_T1;		/* Divides frecuency by 25 */
	
	/* NO NEED TO CONFIGURE EXTERNAL PINS COZ THIS IS FOR INTERNAL POURPUSE */

	// Config TIMER1 interrupt in NVIC 
  	NVIC_SetPriority(TIMER1_IRQn,0x01);
  	NVIC_EnableIRQ(TIMER1_IRQn);	

	// Disable all MAT0.X interrupt.
	LPC_TIM0->MCR = 0; 

	// Enable TIMER1 (Starts Counting)
	LPC_TIM1->TCR |= (1 << 0);

	// Enable MAT1.X interrupt when match
//	LPC_TIM1->MCR = ( 0x0249 ); // 10 0100 1001

	for (i = 0; i < 10; i++){	/* Set the delayers to 0 */ 
		delayers[i] = 0;
	}
}
/**
 * @brief  Using TIMER1.0, creates a periodic interrupt function which period is given
 		in miliseconds by the parameter "period"
 *
 * @param  period  : Period in miliseconds
 * @return none
 */
void periodic_signal_ms(uint32_t period) {
	general_period_ms = period * (TIMER1_FRECUENCY)/1000;
	LPC_TIM1->MR0 = LPC_TIM1->TC + general_period_ms;
	LPC_TIM1->MCR |= ( 0x01 ); 			   // Enable interrupt MAT1.0
} 
/**
 * @brief  Using TIMER1.3, creates a periodic interrupt function which period is given
 		in miliseconds by the parameter "period"
 *
 * @param  period  : Period in miliseconds
 * @return none
 */
void periodic_signal2_ms(uint32_t period_ms) {
	general_period2_ms = period_ms * (TIMER1_FRECUENCY)/1000;
	LPC_TIM1->MR3 = LPC_TIM1->TC + general_period2_ms;
	LPC_TIM1->MCR |= ( 0x1 << 9 ); 			   // Enable interrupt MAT1.3
} 
/**
 * @brief  Using TIMER1.2, creates a periodic interrupt function which period is given
 		in miliseconds by the parameter "period"
 *
 * @param  period  : Period in miliseconds
 * @return none
 */

void periodic_signal_us(uint32_t period) {
	general_period_us = period * ((TIMER1_FRECUENCY)/1000000);
	LPC_TIM1->MR2 = LPC_TIM1->TC + general_period_us;
	LPC_TIM1->MCR |= ( 0x1 << 6 ); 			   // Enable interrupt MAT1.2

} 

/**
 * @brief  Using TIMER1.1, creates one unique delay of "delay" microseconds
 *
 * @param  delay  : Delay in microseconds
 * @return none
 */

 void delay_us(uint32_t delay) {
	LPC_TIM1->MR1 = LPC_TIM1->TC + delay*((TIMER1_FRECUENCY)/1000000);
	LPC_TIM1->MCR |= ( 0x01 << 3); 			  // Enable interrupt MAT1.1
} 

/**
 * @brief  Gets the value of the TIMER1
 *
 * @param  delay  : Delay in microseconds
 * @return none
 */

uint32_t get_timer1() {
	return LPC_TIM1->TC;
} 








