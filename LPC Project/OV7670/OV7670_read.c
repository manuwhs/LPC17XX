#include "LPC17xx.h"
#include "I2C.h"
#include "ov7670.h"
#include "ov7670reg.h"
#include "../GLCD/GLCD.h"
#include <string.h>
#include <math.h>
#include "../TIMER/timer.h" 
#include "../UART/uart.h" 
#include "../Touch_Pannel/TouchPanel.h"



void conversor_IaS(int numero, char * cadnum);

volatile uint32_t num_frame = 0;
char numer_frames[5];
char delay_read[10];
volatile uint32_t frame_read_delay_us;
int aux = 0;
uint32_t n_f_c;

uint32_t num = 0;
uint32_t pass = 0;
uint32_t fps_aux = 0;
uint32_t fps = 0;

uint16_t color_detect = 0x0000;			 /* Color a detectar */
uint8_t cd_R =0, cd_G = 0, cd_B = 0;	 /* Valores de RGV del color a detectar */
uint8_t average;						 /* For converting to grayscale */

uint8_t umbral = 10;					 /* Umbral del color a detectar */
uint8_t read_type = 5;		/* Tipo de lectura de la camara */

uint16_t scn_zone[HEIGHT_ZONES][WIDTH_ZONES];			/* Value of every one of the detection zones */
uint8_t distances[HEIGHT_ZONES][WIDTH_ZONES];

uint32_t det_x = 0;
uint32_t det_y = 0;
uint32_t total_mass = 0;
uint16_t const_block = 20;  /* Valor empirico al cual cuando toda la pantalla esta roja,
la cruz esta en el centro */

uint16_t Vd_OV7670 = 100;
uint16_t Vx_OV7670 = 100;

void read_frame_RGB565 (void) {
	}

/******************************************************************************
* Function Name  : read_frame_RGB565_b
* Description    : Reads a QVGA RGB565 frame from the OV7670 and puts it in the screen
				   Function really optimized in code for it that cannot do anythin else
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
 void read_frame_RGB565_b (void) {
	 uint32_t i=0;
	 uint16_t  pixel_RGB565=0x00;
	 Window (0,0,319,239);
	 LCD_SetCursor(0,0);		/* Set the cursor to (0,0) */

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

	  while( i < 76800/20) {   	 // 20 pixels bucle
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
	 	// New pixel ****************************************************************************
			 	LPC_GPIO0->FIOSET |= (1 << 24);  /* Set RCK to 1 */
				LPC_GPIO0->FIOPIN =  (LPC_GPIO1->FIOPIN >> 3 ) & 0x007F8000; 	/* Write D8..D15 */
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

		 /*****************  GET and Show the FPS and READ_DELAY ************************* */
	  /*
		conversor_IaS(fps, delay_read);
		GUI_Text(30, 100,delay_read,0, 0xFFFF);
	  */
	  /*	
		frame_read_delay_us = get_timer1()- frame_read_delay_us;
		conversor_IaS(frame_read_delay_us, delay_read);
		GUI_Text(160, 120,delay_read,0, 0xFFFF);
		*/
		/*
		GUI_Text(45, 100,"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890",0, 0xFFFF);
		TextButton(100, 60, "Hola" ,0);
		TextButton(200, 100, "Hola wedwe we fw" ,0);

				actual_window.xmin=0;
				actual_window.xmax=319;
				actual_window.ymin=0;
				actual_window.ymax=239;

				CalibrationBar(brillo,90,10,10,100,vertical);
				CalibrationBar(brillo,40,30,30,100,vertical);
				text_mode = 1;
				CalibrationBar(brillo,100,100,80,25,vertical);
				text_mode = 0;
			*/
		/*
		 // Registro lectura camara 
		conversor_IaS(I2CSlaveBuffer[0], delay_read);
		GUI_Text(30, 50,delay_read,0, 0xFFFF);
		*/

	}
