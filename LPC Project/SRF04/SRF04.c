
#include "Ultrasonic.h"
// Global Variables 

uint32_t return_time; ///< Contains pulse width in microseconds 
uint32_t distance_ready; /// < When set to 1 -> Distance value is ready, when 0, it's not
uint32_t distance_cm;	/// < The distance in cm
uint32_t frecuency = 1000000;	/// < Frecuency of the timer
uint32_t temperature = 25;		/// < Temperature
uint32_t speed_sound = 340 * 100;		/// < speed of sound in cm/s
uint32_t times_measured = 0;		/// < speed of sound in cm/s


void configSRF04(void) {

	distance_ready = 0;	/* We indicate we havent got the distance yet */
	// POWER UP THE TIMER  AND SET IT UP AS A TIMER
	LPC_SC->PCONP |= (0x01<<23);   // TIMER3 Powered Up
	LPC_TIM3->CTCR &= ~ (3 << 0);

	// RESET TIMER (AND PC) TO AVOID REGISTER OVERFLOW AND THEN STOP IT
	LPC_TIM3->TCR = 0x11;		 /* RESET */
	LPC_TIM3->TCR = 0x00;		 /* DISABLE (STOP COUNTER)*/

	// Configure the TIMER0 frecuency to 1 MHz

	LPC_SC->PCLKSEL1 &= ~ (3 << 14);		/* Select PCLK /4 = 25 MHz */
	LPC_TIM3->PR = PRESCALER_T0;		/* Divides frecuency by 25 */
	
	// Configure the pins for output and capture
	LPC_PINCON->PINSEL0 |= (3 << 20);	/* Select MAT3.0 (P0.10) pin for output match */
	LPC_PINCON->PINSEL1 |= (3 << 14); /* Select CAP3.0 (P0.23) pin for input. */	 
	
	// Config TIMER0 interrupt in NVIC 
  	NVIC_SetPriority(TIMER3_IRQn,0x03);
  	NVIC_EnableIRQ(TIMER3_IRQn);	
	
	// Set MAT3.0 to 0 (should be 0 anyway) and set no action for Match on MR0
	LPC_TIM3->EMR =(LPC_TIM3->EMR &~(0x31))| (0x10);

	// Disable all MAT3.0 interrupt, reset and unable of TC when Match on MR0
	LPC_TIM3->MCR &=  ~(7 << 0); 

	// Enable TIMER3 (Starts Counting)
	LPC_TIM3->TCR |= (1 << 0 );
	
	// Disable CAP3.0 interrupt 
	LPC_TIM3->CCR &=  ~(1 << 2); 

}

/**
 * @brief  Initialize pulse on MAT0.0.
 		   Sets MAT0.0 to 0 for  usPulseDelay microsec
		   Sets MAT0.0 to 1 for  usPulseWidth microsec
 *
 * @param  none
 * @return none
 */
void initPulse(void) {
  	// Config MAT0.0 to set to 1 on Match
	LPC_TIM3->EMR =(LPC_TIM0->EMR &~(0x31))|(0x20);

    // Config delay time
	LPC_TIM3->MR0 = LPC_TIM0->TC + PULSE_DELAY;
	
	// Enable MAT0.0 interrupt when match
	LPC_TIM3->MCR |=  (1 << 0); 
}


/**
 * @brief  Starts the process to get the distance
 		   When distance_ready = 1, the new distance is calculated.
 *
 * @param  none
 * @return Last width pulse measured
 */

void get_distance(void) {
 	configSRF04();
	initPulse();
}


// ********************************  INTERRUPT *******************************************

void TIMER3_IRQHandler(void) {
	// Check if MR0 match interrupt flag is active
	if (LPC_TIM3->IR & (1 << 0)) {	
			
	   // If the interrupt event is for the begin of the pulse
		if (LPC_TIM3->EMR & (1 << 5)){	/* Because if so EMC[5:4] = 10 */		

			/* In case its a periodic measurement and the last measurement went wrong we
			   disenable all the capture interrupts */
			// Disable CAP3.0 interrupt 
			LPC_TIM3->CCR &=  ~(1 << 2); 
			// Config pulse width
		    LPC_TIM3->MR0 += PULSE_WIDTH; 
		    // Config MAT3.0 clear on match
	      	LPC_TIM3->EMR =(LPC_TIM3->EMR &~(0x31))| (0x11);
   		 }

	    else {   // Pulse has finished
			if (PERIOD_SRF04 == 0) {	/* If we are only gonna make one measurement */
			  	// Config no action in MAT0.0 on match
		      	LPC_TIM3->EMR &= ~(0x31);	/* EM = 0, EMR = 00  (Do nothing) */
				// Disable MAT3.0 interrupt 
		     	LPC_TIM3->MCR &= ~(1 << 0); 
			}
			else {	/* If we are gonna measure it periodicly */
				// Config MAT3.0 to set to 1 on Match
				LPC_TIM3->EMR =(LPC_TIM3->EMR &~(0x31))|(0x20);
				LPC_TIM3->MR0 += PERIOD_SRF04 - PULSE_WIDTH; 	
			}
			// Enable CAP3.0 interrupt 
			LPC_TIM3->CCR |=  (1 << 2); 

		    // Set Rising Edge Capture in CAP3.0
		    LPC_TIM3->CCR &= ~(1 << 1); // Clear CAP0FE
	    	LPC_TIM3->CCR |=  (1 << 0); // Set CAP0RE
    }

	LPC_TIM3->IR |= (1 << 0); /* Clear MR0 interrupt flag	*/
  }	
	// Check if CAP0 interrupt flag is active
	if (LPC_TIM3->IR & (1 << 4)){
		
    
    	// If Rising Edge in CAP0.0 detected
		if (LPC_TIM3->CCR & (1 << 0)){
			// Set capture in CAP0.0 falling edge
		    LPC_TIM3->CCR &= ~(1 << 0); // CAP0RE = 0
		    LPC_TIM3->CCR |=  (1 << 1); // CAP0FE  = 1
		   	distance_ready = 0;
      		// Capture timer value on rising edge
			return_time = LPC_TIM3->CR0;
         }

	     else {	  // If Falling Edge in CAP0.0 detected
			  // Measure pulse width
			  return_time = LPC_TIM3->CR0 - return_time;
			  // Now we desconfigure the timer and Power It down to save energy	  			
			  LPC_TIM3->CCR &= ~ (1 << 2); // Disable CAP3.0 interrupt 
			  //	LPC_SC->PCONP &= ~ (0x01<<1);
			  // Calculate distance
			  distance_cm = (return_time * (speed_sound/2))/frecuency;
			  distance_ready = 1;	/* We indicate we  got the distance  */
			  times_measured++;
    	}
 	 }	
		LPC_TIM3->IR |= (1 << 4); // Clear CAP0 interrupt flag	
}
