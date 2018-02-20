#include "LPC17xx.h"
#include "I2C.h"
#include "ov7670.h"
#include "ov7670reg.h"
#include "../GLCD/GLCD.h"
#include <string.h>
#include <math.h>
#include "../TIMER/timer.h" 
#include "../UART/uart.h" 
#include <math.h>
void conversor_IaS(int numero, char * cadnum);
uint8_t conversor_HexaS(uint16_t numero, char * cadnum);
uint8_t take_picture;
uint16_t try_QCIF_H = 240;
uint16_t try_QCIF_W = 320;
uint8_t show_picture = 0;
/******************************************************************************
* Function Name  : photo_frame_RGB565
* Description    : Reads any RGB565 OV7670 image resolution.
				   There are 2 global variables which have the image resoltion
				   		-
						-
				   This function windows the LCD screen to this size (In case its lower than QVGA)

				   It also can:
				   	- Send the image through the UART0
					- Show an image saved in memory
				   This functions are triggered via global variables that are modified
				   by other functions.

* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/

 void photo_frame_RGB565 (void) {
	 uint32_t i=0,j=0;
	 char cadnum[7];
	 uint16_t  pixel_RGB565=0x00;
	 
	 LCD_SetCursor(0,0);		/* Set the cursor to (0,0) */
	 Window (0,0,try_QCIF_W -1,try_QCIF_H-1);
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

	  //  LCD_SetCursor(0,0);		/* Set the cursor to (0,0) */
	  /****************************************************************************************/
	  /***************************** UART0 Image Sending Option *******************************/
	  /****************************************************************************************/

	  if (take_picture == 1) {	   
	  	
		/* We have to disable the Write Enable so that the frame we are reading doesnt get overrwritter
		which will cause the blurry efect */

		LPC_GPIO0->FIOMASK &= ~ (1 << 30);	   /* UNMASKING WEN*/
		LPC_GPIO0->FIOCLR |= (1 << 30); /* Make pin LOW to disable FIFO write */
		LPC_GPIO0->FIOMASK |=  1 << 30;	   /* MASKING  */

	   	LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */			 /* Por la perdida de ese byte */
		LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */

	  	take_picture = 0;
		   // tx_cadena_UART0("transmitiendo foto\n\r");
		   tx_cadena_UART0("#include <LPC17xx.h>\n\r");
		   tx_cadena_UART0("const uint8_t nos[38400] = {\n\r");

			  while( i <  try_QCIF_H*try_QCIF_W) {   
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF)<<0);	// D[0-7] 	 	P1.18-25
				conversor_HexaS(pixel_RGB565, cadnum);
				tx_cadena_UART0(cadnum);
		
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */	
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				pixel_RGB565 = (((LPC_GPIO1->FIOPIN >> 18) & 0xFF) << 0);	// D[0-7] 	 	P1.18-25
				LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
				conversor_HexaS(pixel_RGB565, cadnum);
				tx_cadena_UART0(cadnum);
					 
				 	i++;	
			}
		//tx_cadena_UART0("\n\r foto transmitidas\n\r");
		tx_cadena_UART0("\n\r }; ");
		}
  	  /****************************************************************************************/
	  /***************************** Image Showing Option *************************************/
	  /****************************************************************************************/
		else if (show_picture == 1) {
			 while( j < try_QCIF_H) { 
		  	 i = 0;  
	//		 LCD_SetCursor(j,try_QCIF_W);		/* Set the cursor to (0,0) */
		  	 while( i < try_QCIF_W) { 
	
				//pixel_RGB565 = (nos[2*(j*try_QCIF_W+i)] << 8);	// D[0-7] 	 P1.18-25  Commented coz we dont load picture until the end
				//	pixel_RGB565 |= (nos[2*(j*try_QCIF_W+i)+1] << 0);	// D[0-7] 	 	P1.18-25
	
		  		// WRITE INDEX
				LPC_GPIO2->FIOCLR = PIN_CS;
				LPC_GPIO1->FIOCLR = PIN_RS |PIN_RD;	   /* Quita 2 ms si unimos !!*/
				// LPC_GPIO1->FIOCLR = PIN_RD;
			
			   	LPC_GPIO2->FIOPIN =  0x0022 & 0x000000ff;        /* Write D0..D7 */
				LPC_GPIO0->FIOPIN =  (0x0022 << 7) & 0x007F8000; /* Write D8..D15 */
			
				LPC_GPIO1->FIOCLR = PIN_WR;
				LPC_GPIO1->FIOSET = PIN_WR;
				LPC_GPIO2->FIOCLR = PIN_CS;	
			
				LPC_GPIO1->FIOSET = PIN_RS;
	
					// WRITE DATA  *************************************************************************
			
				LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
				LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
			
				LPC_GPIO1->FIOCLR = PIN_WR;
				LPC_GPIO1->FIOSET = PIN_WR;
	
			 	i++;	
				}
			j++;
			}
		 LPC_GPIO0->FIOMASK &= ~ (1 << 30);	   /* UNMASKING WEN*/
		 LPC_GPIO0->FIOSET 	|= (1 << 30); 		/* Make pin HIGH to enable FIFO write */
		 LPC_GPIO0->FIOMASK |=  1 << 30;	   /* MASKING  */
		}

		else{
	    
		//LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
		//LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */

	  /****************************************************************************************/
	  /***************************** Image Viewing Option *************************************/
	  /****************************************************************************************/

	  while( j < try_QCIF_H) { 
	  	i = 0;  

		// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CAMBIAR LO DE 4
	  	while( i < try_QCIF_W/4) { 
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
		
			LPC_GPIO1->FIOSET = PIN_RS;

				// WRITE DATA  *************************************************************************
		
			LPC_GPIO2->FIOPIN =  pixel_RGB565 & 0x000000ff;        /* Write D0..D7 */
			LPC_GPIO0->FIOPIN =  (pixel_RGB565 << 7) & 0x007F8000; /* Write D8..D15 */
		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;

	 	// New pixel ****************************************************************************
			 	LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				LPC_GPIO0->FIOPIN =  (LPC_GPIO1->FIOPIN >> 3 ) & 0x007F8000; /* Write D8..D15 */
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				LPC_GPIO2->FIOPIN =  ((LPC_GPIO1->FIOPIN >> 18) & 0xFF);        /* Write D0..D7 */
			   	LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
		   	// WRITE DATA  *************************************************************************	
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;

	 	// New pixel ****************************************************************************
			 	LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				LPC_GPIO0->FIOPIN =  (LPC_GPIO1->FIOPIN >> 3 ) & 0x007F8000; /* Write D8..D15 */
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				LPC_GPIO2->FIOPIN =  ((LPC_GPIO1->FIOPIN >> 18) & 0xFF);        /* Write D0..D7 */
			   	LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
		   	// WRITE DATA  *************************************************************************	
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;
		// New pixel ****************************************************************************
			 	LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				LPC_GPIO0->FIOPIN =  (LPC_GPIO1->FIOPIN >> 3 ) & 0x007F8000; /* Write D8..D15 */
				LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				LPC_GPIO2->FIOPIN =  ((LPC_GPIO1->FIOPIN >> 18) & 0xFF);        /* Write D0..D7 */
			   	LPC_GPIO0->FIOCLR |= (1 << 24);	   /* Set RCK to 0 */
		   	// WRITE DATA  *************************************************************************	
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;
					 
		 	i++;	
		}
			j++;
		}
	}
		 /*****************  GET and Show the FPS and READ_DELAY ************************* */
	  /*
		conversor_IaS(fps, delay_read);
		GUI_Text(30, 100,delay_read,0, 0xFFFF);
	  */	
		frame_read_delay_us = get_timer1()- frame_read_delay_us;
		conversor_IaS(frame_read_delay_us, delay_read);
		GUI_Text(30, 200,delay_read,0, 0xFFFF);
	
		/*
		 // Registro lectura camara 
		conversor_IaS(I2CSlaveBuffer[0], delay_read);
		GUI_Text(30, 50,delay_read,0, 0xFFFF);
		*/
			 
	}

