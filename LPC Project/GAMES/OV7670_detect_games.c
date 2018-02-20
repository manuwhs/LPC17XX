
#include "../LPC17xx.h"
#include "../OV7670/I2C.h"
#include "../OV7670/ov7670.h"
#include "../OV7670/ov7670reg.h"
#include "../GLCD/GLCD.h"
#include <string.h>
#include <math.h>
#include "../TIMER/timer.h" 
#include "../UART/uart.h" 
#include "../Touch_Pannel/TouchPanel.h"
#include "../DMA/dma.h"
#include "../DAC/dac.h"
#include "games.h"
extern	Coordinate * Ptr;
// Tenemos que poner el bucle con "s" porque al hacerlo directamente con s peta */
void conversor_IaS(int numero, char * cadnum);

uint16_t player1_y, player2_y;					//Self-expleinatory
uint16_t detect_width_zone = 0x7C3E;   /* Zone we are gonna detect.	0111 1100 0011 1110	*/


/* For the final game, this function is not suposed to write anything on the screen, just get the 
damn position of the bars and the game logic will be triguered periodicly by another function 
so this one has to be pretty quick... we will have the "calibration menu" to choose how the object
is detected and in the final version of this game yo wont see yourself.	   */

/* Funcion de deteccion cuya cosa especial es que solo lee las width zones (16 posibles)dadas
en forma binaria por una variable de 16 bits detect_width_zone*/

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

void Detect_for_games (uint8_t algorithm) {
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
	  while( j < WIDTH_ZONES){		/* We have to change this to only choose the zones we want to be rewritten */
	  								/* Since there are 16 width zones... one 16 bit variable will do
		 No necesitamos hacerlo 20 veces ya que con el bucle porque ya lo tenemos  	*/

	   if (detect_width_zone & (1 << (15-j))) {

// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j]++;
	 	}												 
// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j]++;
	 	}												 
// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j]++;
	 	}												 
// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j]++;
	 	}												 
// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j]++;
	 	}												 
// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j]++;
	 	}												 
// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j]++;
	 	}												 
// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j]++;
	 	}												 
// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j]++;
	 	}												 
// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j]++;
	 	}												 
// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j]++;
	 	}												 
// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j]++;
	 	}												 
// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j]++;
	 	}												 
// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j]++;
	 	}												 
// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j]++;
	 	}												 
// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j]++;
	 	}												 
// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j]++;
	 	}												 
// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j]++;
	 	}												 
// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j]++;
	 	}												 
// NEW PIXEL *************************************************************************
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 8);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 |= (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
	/*******************  PIXEL DETECTION  ************************* */
	 if ((abs(cd_R - ((pixel_RGB565 >> 11) & RED_MASK)) < umbral)		 // If its the color we want 
	  	&&(abs(cd_G - ((pixel_RGB565 >> 5) & GREEN_MASK)) < 2* umbral)	 // Coz G has 1 more bit
	  	&&(abs(cd_B - ((pixel_RGB565 >> 0) & BLUE_MASK))  < umbral)){
			scn_zone[l][j]++;
	 	}												 

		}
		else {
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
			
		}
		j++;

	   }
	   k++;
	   }
	   l++;
	   }

	
	  // **********************  DETECTION WITH CAMERA******************** 
	  
	  if (algorithm == 0) {
	  	masscentre_detect_players();
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
	GUI_Text(160, 200,delay_read,0, 0xFFFF);
	
	frame_read_delay_us = get_timer1()- frame_read_delay_us;
	conversor_IaS(frame_read_delay_us, delay_read);
	GUI_Text(130, 220,delay_read,0, 0xFFFF);

	frame_read_delay_us = get_timer1();
	*/

	/*
	frame_read_delay_us = get_timer1()- frame_read_delay_us;
	conversor_IaS(frame_read_delay_us, delay_read);
	GUI_Text(130, 180,delay_read,0, 0xFFFF);
	*/
} 



/******************************************************************************
* Function Name  : masscentre_detect_players
* Description    : Detects player 1 and player 2 for the multiplier games.
				   The way we differentiate them is by the position of the zones.

* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void masscentre_detect_players (void){
 		uint16_t i,j;
		uint32_t aux;

		aux = 0;	 //  De las 16 zonas X, cojemos solo las 5 de mas a los extremos, saltandonos la primera
		total_mass = 0;
	 	for (i = 0; i < HEIGHT_ZONES; i++) {
			for (j = 1; j < 6 ; j++) {
			   aux +=  scn_zone[i][j]*i;
			   total_mass += scn_zone[i][j];
		  }
	   }
	   player1_y = (aux*PIX_W_ZONE)/(total_mass);

	   	aux = 0;	 // Max is Sum{400 * 192 *i} [i<16] < 32 bits */
		total_mass = 0;
	 	for (i = 0; i < HEIGHT_ZONES; i++) {
			for (j = 10; j < WIDTH_ZONES -1; j++) {
			   aux +=  scn_zone[i][j]*i;
			   total_mass += scn_zone[i][j];
		  }
	   }
	   player2_y = (aux*PIX_W_ZONE)/(total_mass);

}
		
 

