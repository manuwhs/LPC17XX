
#include "LPC17xx.h"
#include "I2C.h"
#include "ov7670.h"
#include "ov7670reg.h"
#include "../GLCD/GLCD.h"
#include <string.h>
#include <math.h>
#include "../TIMER/timer.h" 
#include "../UART/uart.h" 

// Tenemos que poner el bucle con "s" porque al hacerlo directamente con s peta */
void conversor_IaS(int numero, char * cadnum);

/* Writes in det_x and det_y the coordinates of the detected object using the mountains and wells
algorithm from the blocks */

   /* Funcion de deteccion enventadana */
void detect_window_RGB565 (uint16_t Vw, uint16_t Vh, uint8_t algorithm) {
	 uint32_t i,j,k,l;
	 uint16_t  pixel_RGB565=0x00;
	 uint32_t h=0, w=0;
	 h =  Vh/20;	     //  height zones we will not read
	 w = (Vw/20);		 //  width zones we will not read

 	 LCD_SetCursor(0,0);		/* Set the cursor to (0,0) */
	 Window (0,0,MAX_X - 1 - w*20,(MAX_Y - 1 - h*20));

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

	  // num_frame += 1;		/* Por algun motivo mejora el tiempo en 1,25 ms */
	  
	  fps_aux ++;	  /* To know how many fps we are reading */

		for (i = 0; i < HEIGHT_ZONES; i++) {
			for (j = 0; j < WIDTH_ZONES; j++) {
				scn_zone[i][j] = 0;
		  }
	   }

	  l = 0;
	  while( l < HEIGHT_ZONES - h){			  // Por cada zona H
	  k = 0;
		  // A partir de aqui leemos de la FIFO una fila de zonas horizontales (640)*20 completa

	  while( k < PIX_H_ZONE){ 				  // Por cada línea de la zona H (20)
	  j = 0;

	  // A partir de aqui leemos de la FIFO una W (640) completa
	     while ( j < w) {			/* Por cada linea de la zona W que no leemos */
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
	//	 LCD_SetCursor(20*l+k,j*20);
		 while( j < WIDTH_ZONES) {	   // Por cada zona W restante 
		// No necesitamos hacerlo 20 veces con el bucle porque ya lo tenemos  
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
	
			 // WRITE INDEX
			LPC_GPIO2->FIOCLR = PIN_CS;
			LPC_GPIO1->FIOCLR = PIN_RS |PIN_RD;	   /* Quita 2 ms si unimos !!*/
			// LPC_GPIO1->FIOCLR = PIN_RD;
		
		   	LPC_GPIO2->FIOPIN =  0x0022 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (0x0022 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;
			LPC_GPIO2->FIOCLR = PIN_CS;

	/*******************  PIXEL DETECTION  ************************* */

//////////////////////////////////////////////////////////////////////////////////////////////////////
//				ANULAR SI QUEREMOS FILAS e INCLUIR k>0 si queremos COLUMNAS     /////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j-w]++;;
	 		pixel_RGB565 = 0x1F<<11;
	 	}
				
			// WRITE DATA  *************************************************************************
		
			LPC_GPIO2->FIOCLR = PIN_CS;
			LPC_GPIO1->FIOSET = PIN_RS;
		
			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;
													 


// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25

	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j-w]++;;
	 		pixel_RGB565 = 0x1F<<11;
	 	}
			// WRITE DATA  *************************************************************************
		
			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;

// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25

	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j-w]++;;
	 		pixel_RGB565 = 0x1F<<11;
	 	}
			// WRITE DATA  *************************************************************************
		
			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;

// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25

	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j-w]++;;
	 		pixel_RGB565 = 0x1F<<11;
	 	}
			// WRITE DATA  *************************************************************************

			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;

// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25

	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j-w]++;;
	 		pixel_RGB565 = 0x1F<<11;
	 	}
			// WRITE DATA  *************************************************************************

			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;

// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25

	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j-w]++;;
	 		pixel_RGB565 = 0x1F<<11;
	 	}
			// WRITE DATA  *************************************************************************

			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;

// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25

	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j-w]++;;
	 		pixel_RGB565 = 0x1F<<11;
	 	}
			// WRITE DATA  *************************************************************************

			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;

// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25

	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j-w]++;;
	 		pixel_RGB565 = 0x1F<<11;
	 	}
			// WRITE DATA  *************************************************************************
		
			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;

// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25

	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j-w]++;;
	 		pixel_RGB565 = 0x1F<<11;
	 	}
			// WRITE DATA  *************************************************************************

			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;

// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25

	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j-w]++;;
	 		pixel_RGB565 = 0x1F<<11;
	 	}
			// WRITE DATA  *************************************************************************
		
			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;

// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25

	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j-w]++;;
	 		pixel_RGB565 = 0x1F<<11;
	 	}
			// WRITE DATA  *************************************************************************
		
			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;

// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25

	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j-w]++;;
	 		pixel_RGB565 = 0x1F<<11;
	 	}
			// WRITE DATA  *************************************************************************
		
			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;

// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25

	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j-w]++;;
	 		pixel_RGB565 = 0x1F<<11;
	 	}
			// WRITE DATA  *************************************************************************
		
			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;

// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25

	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j-w]++;;
	 		pixel_RGB565 = 0x1F<<11;
	 	}
			// WRITE DATA  *************************************************************************
		
			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;

// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25

	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j-w]++;;
	 		pixel_RGB565 = 0x1F<<11;
	 	}
			// WRITE DATA  *************************************************************************
		
			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;

// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25

	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j-w]++;;
	 		pixel_RGB565 = 0x1F<<11;
	 	}
			// WRITE DATA  *************************************************************************
		
			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;

// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25

	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j-w]++;;
	 		pixel_RGB565 = 0x1F<<11;
	 	}
			// WRITE DATA  *************************************************************************
		
			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;

// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25

	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j-w]++;;
	 		pixel_RGB565 = 0x1F<<11;
	 	}
			// WRITE DATA  *************************************************************************
		
			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;

// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25

	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j-w]++;;
	 		pixel_RGB565 = 0x1F<<11;
	 	}
			// WRITE DATA  *************************************************************************
		
			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;

// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25

	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j-w]++;;
	 		pixel_RGB565 = 0x1F<<11;
	 	}
			// WRITE DATA  *************************************************************************
		
			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;													 

		j++;
	   }
	   k++;
	   }
	   l++;
	   }

	
	  // **********************  DETECTION ******************** \\
	frame_read_delay_us = get_timer1()- frame_read_delay_us;
	conversor_IaS(get_timer1()-frame_read_delay_us, delay_read);
	GUI_Text(30, 150,delay_read,0, 0xFFFF);
	  if (algorithm == 0) {
	  	montwell_detect();
		}
	  else if (algorithm == 1) {
	  	 masscentre_detect();
	   }		

	  GUI_Text(det_x, det_y,"X",0xFFFF, 0x0);

	  // ******************* Recalculate the new umbral in case the color has changed ****** \\
	  cd_R = (color_detect >> 11) & RED_MASK;
	  cd_G = (color_detect >> 5) & GREEN_MASK;
	  cd_B = (color_detect >> 0) & BLUE_MASK;

 	   /*****************  GET and Show the FPS and READ_DELAY ************************* */
	   /*
	conversor_IaS(fps, delay_read);
	GUI_Text(30, 100,delay_read,0, 0xFFFF);
	
	frame_read_delay_us = get_timer1()- frame_read_delay_us;
	conversor_IaS(frame_read_delay_us, delay_read);
	GUI_Text(30, 200,delay_read,0, 0xFFFF);
	*/

	}


