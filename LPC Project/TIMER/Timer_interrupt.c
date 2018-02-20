
#include "Timer.h"
#include "../OV7670/ov7670.h"
#include "../UART/uart.h" 
#include "../GLCD/GLCD.h" 
#include "../GAMES/games.h"
#include "../Touch_Pannel/Touchpanel.h"
#include <math.h>
#include "../DAC/dac.h"
#include "../DMA/dma.h"
#define PI	3.1415926

uint8_t gaming = 0; 	/* Flag para saber si entramos en una nueva ejecucion del juego o no */
uint32_t flag, i;

uint16_t delayers[10];

char cado[10];

uint16_t delay_only = 0;

uint8_t move_inv_flag = 0;


void TIMER1_IRQHandler(void) {
	// Check if MR0 match interrupt flag is active
	if (LPC_TIM1->IR & (1 << 0)) {	
	    LPC_TIM1->IR |= (1 << 0); /* Clear MR0 interrupt flag	*/			
		LPC_TIM1->MR0 += general_period_ms; 
		fps = fps_aux;
		fps_aux = 0;

		time_pong++;
		time_game++;
		move_inv_flag = 1;
		// WDTFeed();			// Actualice watchdog
		
	}

	// Check if MR1 match interrupt flag is active
	if (LPC_TIM1->IR & (1 << 1)) {	
	    LPC_TIM1->IR |= (1 << 1); /* Clear MR3 interrupt flag	*/			
		delay_only = 0;	
	}

		// Check if MR2 match interrupt flag is active	
	if (LPC_TIM1->IR & (1 << 2)) {	
	    LPC_TIM1->IR |= (1 << 2); // Clear MR2 interrupt flag				
		LPC_TIM1->MR2 += general_period_us; 

	}

	//*************************************************************************************************
	 //***************************************  GAMES *************************************************
	//*************************************************************************************************
	  
	if (LPC_TIM1->IR & (1 << 3)) {		   	// Check if MR3 match interrupt flag is active	
		LPC_TIM1->IR |= (1 << 3); 			// Clear MR3 interrupt flag	
		if (game > 0) {
			LPC_TIM1->MR3 += general_period2_ms; 
		}
		else {
			LPC_TIM1->MR3-= 1;
		}
	  if (gaming == 0) {
		  gaming = 1;

	  if (game == 1) {
			PONG();
		}
		else if (game == 3) {
			CANDY_CRUSH();
		}
		else if (game == 2) {

			space_invaders();
		 }
	
		gaming = 0;

		}
			
		
	}
 }	




	 	 /*
  	// Check if MR2 match interrupt flag is active
	if (LPC_TIM1->IR & (1 << 2)) {	
	    LPC_TIM1->IR |= (1 << 2); // Clear MR2 interrupt flag				
		LPC_TIM1->MR2 += general_period_us; 

		//                   UPDATE DAC VALUE                    
		time_DAC++;
   		//LPC_DAC->DACR = ((uint16_t)(512 + 512*(cos(2*PI*((float)time_DAC/samples_per_cicle))))<<6)|DAC_BIAS;
		LPC_DAC->DACR = buffer_DAC[time_DAC%num_samples_cycle];


			for (i = 0; i < 10; i++){//	 Set the delayers -1 
			if (delayers[i] > 0) {
				delayers[i]--;

			}

		}

 	 }
	 */ 

   /*
		conversor_IaS(DMATCCount, cado);
		GUI_Text(20, 70,"         ",0, 0xFFFF);
		GUI_Text(20, 70,cado,0, 0xFFFF);

		conversor_IaS(DMAErrCount, cado);
		GUI_Text(20, 40,"  ",0, 0xFFFF);
		GUI_Text(20, 40,cado,0, 0xFFFF);
		Pulses_DAC = time_DAC;
		time_DAC = 0;
		DMAErrCount	= 0;
		DMATCCount = 0;
		//periodic_signal_us(1000000/(60*time_game*NUM_SAMPLES_CYCLE));
		*/
		 /*
		 if (time_game%2 == 0) {
			  	DMA_DAC_audio_Channel_Init( 0, muelle,2768);
			}
			*/

