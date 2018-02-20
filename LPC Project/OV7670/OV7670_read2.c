
#include "LPC17xx.h"
#include "I2C.h"
#include "ov7670.h"
#include "ov7670reg.h"
#include "../GLCD/GLCD.h"
#include <string.h>
#include <math.h>
#include "../TIMER/timer.h" 
#include "../UART/uart.h" 
#include "../SRF04/Ultrasonic.h"

// Tenemos que poner el bucle con "s" porque al hacerlo directamente con s peta */
 
void conversor_IaS(int numero, char * cadnum);

/******************************************************************************
* Function Name  : read_window_RGB565
* Description    : Reads a QVGA RGB565 frame from the OV7670, windows it and shows it on the screen
				   The windowning can only be done in 20px steps.
				   The LCD screen will be windowed to the same size.

* Input          : - Vw:  Size of the window width we are deleting (0-320) 
						  It will be truncked to be 20px multiple 
*		         : - Vh:  Size of the window height we are deleting (0-240) 
						  It will be truncked to be 20px multiple 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/

   void read_window_RGB565 (uint16_t Vw, uint16_t Vh) {
	 uint32_t i=0, j=0,s = 0;
	 uint16_t  pixel_RGB565=0x00;
	 uint32_t h=0, w=0;

	 h =  20*(HEIGHT_ZONES - Vh/20);	 // Height zones we will read
	 w = (Vw/20);						//  Width zones we will not read

 	 LCD_SetCursor(0,0);		/* Set the cursor to (0,0) */
	 Window (0,0,MAX_X - (1 + w*20), h -1);
	  //****************   Reset the Read Pointer		  **************\\ 

	  LPC_GPIO0->FIOMASK &= ~ (1 << 29);	   /* UNMASKING  RRCK*/
	  LPC_GPIO0->FIOMASK &= ~ (1 << 30);	   /* UNMASKING WEN*/
   	  LPC_GPIO0->FIOMASK &= ~ (1 << 24);	   /* UNMASKING RCLK*/	/* Por algun motivo mejora el tiempo en 1,25 ms */

	  LPC_GPIO0->FIOCLR |= (1 << 29);	 // Set RCLK to 0
	  LPC_GPIO0->FIOSET |= (1 << 24);  //  Set RCK to 1 
	  LPC_GPIO0->FIOCLR |= (1 << 24);	 //Set RCK to 0 
	  LPC_GPIO0->FIOSET |= (1 << 24);  //  Set RCK to 1 
	  LPC_GPIO0->FIOCLR |= (1 << 24);	 //Set RCK to 0 
	  LPC_GPIO0->FIOSET |= (1 << 24);  //  Set RCK to 1 
	  LPC_GPIO0->FIOSET |= (1 << 29);  //  Set RCLK to 1 
	  //   LPC_GPIO0->FIOCLR |= (1 << 30); /* Make pin LOW to disable FIFO write */

	  LPC_GPIO0->FIOMASK |=  1 << 29;	   /* MASKING  */
	  LPC_GPIO0->FIOMASK |=  1 << 30;	   /* MASKING */

	  fps_aux ++;	  /* To know how many fps we are reading */

	  while (i < h) { 						/* Number of height lines */
	  j = 0;
	     while ( j < w) {			/* Number of width zones */
			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
 			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */

			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
 			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */

			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
 			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */

			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
 			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */

			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
 			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */

			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
 			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */

			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
 			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */

			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
 			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */

			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
 			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */

			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
 			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */

			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
 			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */

			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
 			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */

			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
 			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */

			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
 			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */

			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
 			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */

			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
 			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */

			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
 			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */

			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
 			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */

			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
 			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */

			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
 			LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
			LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
		 j++;
		 }
		 while( j < WIDTH_ZONES) { 
		 
		 	for (s = 0; s < 4; s++){  
			 	LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
	
			 // WRITE INDEX
			LPC_GPIO2->FIOCLR = PIN_CS;
			LPC_GPIO1->FIOCLR = PIN_RS |PIN_RD;	   /* Quita 2 ms si unimos !!*/

		   	LPC_GPIO2->FIOPIN =  0x0022 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (0x0022 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;
			LPC_GPIO2->FIOCLR = PIN_CS;
		
			// WRITE DATA  *************************************************************************
		
			LPC_GPIO2->FIOCLR = PIN_CS;
			LPC_GPIO1->FIOSET = PIN_RS;
		
			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;

	// New pixel ****************************************************************************
	LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
	pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
	LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */	
	LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
	pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
		
			// WRITE DATA  *************************************
		
		
			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;
	// New pixel ****************************************************************************
	LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
	pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
	LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */	
	LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
	pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
		
			// WRITE DATA  *************************************
		
		
			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;	
	// New pixel ****************************************************************************
	LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
	pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
	LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */	
	LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
	pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
		
			// WRITE DATA  *************************************
		
		
			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;		
	// New pixel ****************************************************************************
	LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
	pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
	LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */	
	LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
	pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
		
			// WRITE DATA  *************************************
		
		
			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;	
	
	}			

		 	j++;	
		}
		i++;
	}
		 /*****************  GET and Show the FPS and READ_DELAY ************************* */

	  /*
	// 	programa_main ();
		conversor_IaS(fps, delay_read);
		GUI_Text(30, 100,delay_read,0, 0xFFFF);
		
		frame_read_delay_us = get_timer1()- frame_read_delay_us;
		conversor_IaS(frame_read_delay_us, delay_read);
		GUI_Text(30, 200,delay_read,0, 0xFFFF);

	   	conversor_IaS(distance_cm, delay_read);
		GUI_Text(60, 150,delay_read,0, 0xFFFF); 
	*/
	}

	   










	/*	
		 // Registro lectura camara 
		conversor_IaS(j, delay_read);
		GUI_Text(60, 0,delay_read,0, 0xFFFF);

		conversor_IaS(i, delay_read);
		GUI_Text(60, 50,delay_read,0, 0xFFFF);		

		conversor_IaS(Vh, delay_read);
		GUI_Text(60, 100,delay_read,0, 0xFFFF);

		conversor_IaS(Vw, delay_read);
		GUI_Text(60, 150,delay_read,0, 0xFFFF);

		conversor_IaS(WIDTH_ZONES, delay_read);
		GUI_Text(160, 0,delay_read,0, 0xFFFF);

		conversor_IaS(HEIGHT_ZONES, delay_read);
		GUI_Text(160, 50,delay_read,0, 0xFFFF);		

		conversor_IaS(QVGA_HEIGHT, delay_read);
		GUI_Text(160, 100,delay_read,0, 0xFFFF);

		conversor_IaS(QVGA_WIDTH, delay_read);
		GUI_Text(160, 150,delay_read,0, 0xFFFF);
	 */







