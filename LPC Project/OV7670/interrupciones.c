

#include "LPC17xx.h"			/* LPC17xx Peripheral Registers */
#include "ov7670.h"
#include "../State_Machine/States.h"
uint32_t cont = 1;
uint8_t fps_divider;
void programa_main (void);

void EINT3_IRQHandler(void) {

if ((pass == 1)&&(state_in_process == 0)){
	if (cont < fps_divider) {
		cont ++;
	}
	else{
		if (((LPC_GPIOINT->IO0IntStatF >> 25) & 0x01)==1) { // if P0.26 falling edge -> Read frame
			frame_read_delay_us = get_timer1();

			if ( read_type == 0) { 	   // Vision pantalla completa de QVGA
			 	read_frame_RGB565_b ();
			}
			if ( read_type == 1) {		 // Vision pantalla completa QVGA con deteccion algoritmo 0
			 	detect_read_frame_RGB565 (0);
			}
			if ( read_type == 2) {	    // Vision pantalla completa QVGA con deteccion algoritmo 1

			 	detect_read_frame_RGB565 (1);
			}
			if ( read_type == 3) {		//  Deteccion para juegos algoritmo 0 sin mostrar en pantalla

			 	Detect_for_games (0); 
			}
					 
			if ( read_type == 5) {	   // Vision enventanada QVGA 

			 	read_window_RGB565 (Vd_OV7670, Vx_OV7670);
			}

		  if ( read_type == 6) {		// Funcion generica para mostrar imagenes y pasar la imagen via UART0

			 	photo_frame_RGB565 ();
			}
										   // Deteccion enventanada QVGA algoritmo 0 
			if ( read_type == 7) {

			 	detect_window_RGB565 (Vd_OV7670, Vx_OV7670,0);
			}
	    	if ( read_type == 8) {		    // Deteccion enventanada QVGA algoritmo 1

			 	detect_window_RGB565 (Vd_OV7670, Vx_OV7670,1);
			}	
		}
	cont = 1;
	}
}
	LPC_GPIOINT->IO0IntClr |= 1 << 25;		//Clear interrupt

	//programa_main();
}

