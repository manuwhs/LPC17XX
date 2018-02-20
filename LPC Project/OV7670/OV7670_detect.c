
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

/******************************************************************************
* Function Name  : detect_read_frame_RGB565
* Description    : Reads a QVGA RGB565 frame from the OV7670, and checks pixel by pixel if
				   its considered part of the detected object (color-based detection)
				   If its considered detected it will paint it red in the screen, if not
				   it will paint it normal.
				   - It will also call a function that calculates que center of the detected object

* Input          : - algorithm: Algorith used to detect the object
* Output         : None
* Return         : None
* Attention		 : We have divided the screen into 20x20px zones (16x12 zones)
				   For every zone we have a variable with the number of pixels detected
				   as an object. The algorithms will use these variables to detect center.
*******************************************************************************/

 void detect_read_frame_RGB565 (uint8_t algorithm) {
	 uint32_t i,j,k,l;
	 uint16_t  pixel_RGB565=0x00;
	 Window (0,0,319,239);
	 LCD_SetCursor(0,0);		/* Set the cursor to (0,0) */
	  //****************   Reset the Read Pointer		  **************\\

	  LPC_GPIO0->FIOMASK &= ~ (1 << 29);	   /* UNMASKING  RRCK*/
	  LPC_GPIO0->FIOMASK &= ~ (1 << 30);	   /* UNMASKING WEN*/
   	  LPC_GPIO0->FIOMASK &= ~ (1 << 24);	   /* UNMASKING RCLK*/	/* Por algun motivo mejora el tiempo en 1,25 ms */

	  LPC_GPIO0->FIOCLR |= (1 << 29);	 // Set RCLK to 0
	  LPC_GPIO0->FIOSET |= (1 << 24);  //  Set RCK to 1 
	  LPC_GPIO0->FIOCLR |= (1 << 24);	 //  Set RCLK to 0 
	  LPC_GPIO0->FIOSET |= (1 << 24);  //  Set RCK to 1 
	  LPC_GPIO0->FIOCLR |= (1 << 24);	 //  Set RCLK to 0 
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
	  while( l < HEIGHT_ZONES){
	  k = 0;
	  while( k < PIX_H_ZONE){ 
	  j = 0;
	  while( j < WIDTH_ZONES){
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
			scn_zone[l][j]++;
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
			scn_zone[l][j]++;
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
			scn_zone[l][j]++;
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
			scn_zone[l][j]++;
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
			scn_zone[l][j]++;
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
			scn_zone[l][j]++;
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
			scn_zone[l][j]++;
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
			scn_zone[l][j]++;
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
			scn_zone[l][j]++;
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
			scn_zone[l][j]++;
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
			scn_zone[l][j]++;
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
			scn_zone[l][j]++;
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
			scn_zone[l][j]++;
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
			scn_zone[l][j]++;
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
			scn_zone[l][j]++;
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
			scn_zone[l][j]++;
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
			scn_zone[l][j]++;
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
			scn_zone[l][j]++;
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
			scn_zone[l][j]++;
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
			scn_zone[l][j]++;
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

	  if (algorithm == 0) {
	  	montwell_detect();
		}
	  else if (algorithm == 1) {
	  	 masscentre_detect();
	   }		


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

	frame_read_delay_us = get_timer1()- frame_read_delay_us;
	conversor_IaS(get_timer1()-frame_read_delay_us, delay_read);
	GUI_Text(30, 150,delay_read,0, 0xFFFF);
	 */
   //show_blocks();
   	  GUI_Text(det_x, det_y,"X",0xFFFF, 0x0);
	}

/******************************************************************************
* Function Name  : montwell_detect
* Description    : Calculates the center of the detected object using a mountain-wells algorithm
				   It used the array that has the number of detected pixels of every ZONE (scn_zone[][])
				   Every Zone adds up a value to its adyacent ZONES  proportional to the number of detected
				   pixels it has so that zones surrounded by other zones with a lot of detected pixels
				   behave as mountains.
				   The center of the object will be the zone with more value.

* Input          : None
* Output         : It generates de (x,y) coordinates of the detected object and saves it into
				   the global variables det_x and det_y
* Return         : None
* Attention		 : We have divided the screen into 20x20px zones (16x12 zones)
				   For every zone we have a variable with the number of pixels detected
				   as an object. The algorithms will use these variables to detect center.
*******************************************************************************/
void montwell_detect (void){
		/* We ponder the values */
		uint16_t max_block, ponder;
	    uint16_t i,j,k,l;
		uint16_t aux_x, aux_y;
	 	for (i = 0; i < HEIGHT_ZONES; i++) {
			for (j = 0; j < WIDTH_ZONES; j++) {			  /* For every zone */

				 	for (l = 0; l < HEIGHT_ZONES; l++) {
						for (k = 0; k < WIDTH_ZONES; k++) {	   /*We add value to the zone */
							// ponder = const_block*distances[abs(i-l)][abs(j-k)]; 
							ponder = const_block*((i-l)*(i-l)+(j-k)*(j-k)); 
							if (ponder != 0) {
			   					scn_zone[l][k] += scn_zone[i][j]/ponder;
							}
						}
					}
		  	}
	   }
	   	max_block = 0;

	   /* We find the highest block */
	   	for (i = 0; i < HEIGHT_ZONES; i++) {
			for (j = 0; j < WIDTH_ZONES; j++) {			  /* For every zone */
				 if (scn_zone[i][j] > max_block) {
					 max_block = scn_zone[i][j];
					 aux_x = j;
					 aux_y = i;
				}
			}
		}
		det_y = aux_y*PIX_H_ZONE;
		det_x = aux_x*PIX_W_ZONE;
  }

/******************************************************************************
* Function Name  : masscentre_detect 
* Description    : Calculates the center of the detected object using the mass-center algorithm
				   It used the array that has the number of detected pixels of every ZONE (scn_zone[][])				   The center of the object will be the zone with more value.

* Input          : None
* Output         : It generates de (x,y) coordinates of the detected object and saves it into
				   the global variables det_x and det_y
* Return         : None
* Attention		 : We have divided the screen into 20x20px ZONES (16x12 zones)
				   For every zone we have a variable with the number of pixels detected
				   as an object. The algorithms will use these variables to detect center.
*******************************************************************************/ 


void masscentre_detect (void){
 		uint16_t i,j;
		uint32_t aux_x, aux_y;
		aux_x = 0;	 // Max is Sum{400 * 192 *i} [i<16] < 32 bits */
		aux_y = 0;
		total_mass = 0;
	 	for (i = 0; i < HEIGHT_ZONES; i++) {
			for (j = 0; j < WIDTH_ZONES; j++) {
			   aux_y +=  scn_zone[i][j]*i;
			   aux_x +=  scn_zone[i][j]*j;
			   total_mass += scn_zone[i][j];
		  }
	   }
	   det_x = (aux_x*PIX_W_ZONE)/(total_mass);
	   det_y = (aux_y*PIX_H_ZONE)/(total_mass);
}

 /******************************************************************************
* Function Name  : show_blocks
* Description    : Shows the number of detected pixels at every ZONE divided by 10. (0-40)
* Input          : None
* Output         : None
* Return         : None
* Attention		 : We have divided the screen into 20x20px ZONES (16x12 zones)
				   For every zone we have a variable with the number of pixels detected
				   as an object. The algorithms will use these variables to detect center.
*******************************************************************************/ 
 void show_blocks (void) {
 	uint16_t i,j;
		for (i = 0; i < HEIGHT_ZONES; i++) {
			for (j = 0; j < WIDTH_ZONES; j++) {
				conversor_IaS(scn_zone[i][j]/10, delay_read);
				GUI_Text(j*PIX_W_ZONE,i*PIX_H_ZONE ,delay_read,0, 0xFFFF);		
		  }
	   }
}
