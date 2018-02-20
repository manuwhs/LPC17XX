
#include "LPC17xx.h"
#include "I2C.h"
#include "ov7670.h"
#include "ov7670reg.h"
#include "../GLCD/GLCD.h"
#include <string.h>
#include "../TIMER/timer.h" 
#include "../UART/uart.h" 

uint16_t MAX_X = 320;
uint16_t MAX_Y = 240;
uint8_t text_mode = 0;
void conversor_IaS(int numero, char * cadnum);

char aux_lum[10];
/* Variables with the value of the registers */
uint8_t OV7670contrast = 0x40;
uint8_t OV7670bright = 0x00;
uint16_t OV7670luminis = 0;		   /* Luminosidad en modo manual */
uint8_t OV7670R_gain = 0, OV7670G_gain = 0, OV7670B_gain = 0;
uint8_t OV7670fps = 20;
uint8_t resol_type = 1; 	/* Type of resolution VGA, QVGA, QQVGA or QQQVGA */
/******************************************************************************
* Function Name  : getOV7670_fps
* Description    : Gets the number of frames per second we are requesting to the OV7670
				   and stores it in the global variable  OV7670fps
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None

*******************************************************************************/

void getOV7670_fps (void) {
	uint16_t cocacola;
	ov7670_read(REG_DBLV);
	if (I2CSlaveBuffer[0] == 0) 
		cocacola = 1;
	else if (I2CSlaveBuffer[0] == 64)
		cocacola = 4;
	else if (I2CSlaveBuffer[0] == 128)
		cocacola = 6;
	else if (I2CSlaveBuffer[0] == 192)
		cocacola = 8;

    ov7670_read(REG_CLKRC); 		  // Configured to 30 fps
	OV7670fps = (15 * cocacola)/(I2CSlaveBuffer[0]+1);
}
/******************************************************************************
* Function Name  : getOV7670_RGBgain
* Description    : Gets the RGB gain of the OV7670 light sensors.
				   This gain tells us how much the RGB gain is amplified.
				   It stores them in   OV7670R_gain, OV7670G_gain and OV7670B_gain
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None

*******************************************************************************/

void getOV7670_RGBgain (void) {
	ov7670_read(REG_RED);
	OV7670R_gain = I2CSlaveBuffer[0];
	ov7670_read(REG_GGAIN);
	OV7670G_gain = I2CSlaveBuffer[0];
	ov7670_read(REG_BLUE);
	OV7670B_gain = I2CSlaveBuffer[0];
}
/******************************************************************************
* Function Name  : getOV7670_BriConLum
* Description    : Gets the Bightness, Contrast and Luminance of the OV7670 camera
				   and stores them into the variables  OV7670bright, OV7670contrast
				   and 	OV7670luminis
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None

*******************************************************************************/

void getOV7670_BriConLum (void) {

	ov7670_read(REG_BRIGHT);
	OV7670bright = I2CSlaveBuffer[0];
	ov7670_read(REG_CONTRAS);
	OV7670contrast = I2CSlaveBuffer[0];

/*	ov7670_read(REG_VREF);					// BECAUSE EMPIRICALLY WE DONT USE THEM
	OV7670luminis = (I2CSlaveBuffer[0] & (0xC0)) << 2;	  // VREF [7:6]
*/
	ov7670_read(REG_GAIN);
	OV7670luminis = I2CSlaveBuffer[0];

}
/******************************************************************************
* Function Name  : OV7670_BriConLum
* Description    : Sets the Bightness, Contrast and Luminance of the OV7670 camera.
				   It also sets the luminance mode: MANUAL or AUTOMATIC
* Input          : - B: Brightness (0-255), Default = 0.
*				   - C: Contrast (0-255),	Default = 0x40 (zero contrast)
*				   - L: Luminance (0-255) XXXXXXXXXXXX
*				   - mode: Luminance mode. 0 -> MANUAL and 1->Automatic
* Output         : None
* Return         : None
* Attention		 : None

*******************************************************************************/
void OV7670_BriConLum (uint8_t  B, uint8_t C, uint16_t L, uint8_t mode) {


    ov7670_set(REG_BRIGHT, B); 	   // 0x55 0x00
    ov7670_set(REG_CONTRAS, C);    // 0x56 0x40


   if (mode == 0x00) {	 // Manual L			 Valor de 0 a 1023

	 ov7670_read(REG_COM8);							 		 // LEER REGISTRO
   	 ov7670_set(REG_COM8, I2CSlaveBuffer[0]&(~(0x1 << 2)));   //	 COM8[2] = 0	
   	 //  ov7670_set(REG_VREF, (L >> 2) & 0xC0 ); 		  // Empìricammente estos no afectan
   	 ov7670_set(REG_GAIN, L & 0xFF); 
											   
	}
	else if (mode == 0x01) {		 // Automatico L		Valor de 0 a 7
	 ov7670_read(REG_COM8);							 		 // LEER REGISTRO
   	 ov7670_set(REG_COM8, I2CSlaveBuffer[0]|(0x1 << 2));   //	 COM8[2] = 1
	 ov7670_set(REG_COM9, (L & 0x07)<<4);  		
	}
}

/******************************************************************************
* Function Name  : OV7670_RGBgain
* Description    : Sets the RGB gain when we are in MANUAL mode
* Input          : - R: Color red gain 		(0-255)
*				   - G: Color green gain 	(0-127)
*				   - B: Color blue gain		(0-255)
* Output         : None
* Return         : None
* Attention		 : None

*******************************************************************************/
void OV7670_RGBgain (uint8_t R ,uint8_t G, uint8_t B) {

	ov7670_set(REG_RED, R);	   // 8 bits
	ov7670_set(REG_GGAIN, G);	// 7 bits
	ov7670_set(REG_BLUE, B);	// 8 bits
    OV7670R_gain = R; 
	OV7670G_gain = G; 
	OV7670B_gain = B;
	
}
/******************************************************************************
* Function Name  : OV7670_RGBgain_Range
* Description    : Sets the RGB gain range and the prefixed gain
* Input          : - R: Color red gain range 		(0-255)
*				   - G: Color green gain range	(0-127)
*				   - B: Color blue gain	range	(0-255)
*				   - pre: Prefixed gain			(0-255)
* Output         : None
* Return         : None
* Attention		 : None

*******************************************************************************/
void OV7670_RGBgain_Range (uint8_t R ,uint8_t G, uint8_t B, uint8_t pre) {

	ov7670_set(REG_R_LMT, R);	   // 8 bits
	ov7670_set(REG_G_LMT, G);	// 7 bits
	ov7670_set(REG_B_LMT, B);	// 8 bits
 	ov7670_set(REG_GFIX, pre);	// 8 bits

}
/******************************************************************************
* Function Name  : OV7670_AWBmode 
* Description    : Sets the AWB (Automatic White Balance) mode.
				   The AWB configures the RGB gain aumaticly so that we get a good colores image.
				   It has 3 modes:
				   		1- Manual: We choose these gains with the function OV7670_RGBgain
						2- Auto 1: It doest with an average based algorithm.
						3- Auto 2: It doest with a temperatura based algorith
* Input          : - mode: AWB mode		(0-2)
* Output         : None
* Return         : None
* Attention		 : None

*******************************************************************************/
void OV7670_AWBmode (uint8_t mode) {
   if (mode == 0x00) {	 // Manual
   		ov7670_read(REG_COM8);							 		 // LEER REGISTRO
   	 	ov7670_set(REG_COM8, I2CSlaveBuffer[0]&(~(0x1 << 1)));   //	 COM8[1] = 0
	}
	else if (mode == 0x01) {
	 ov7670_read(REG_COM8);							 		 // LEER REGISTRO
   	 ov7670_set(REG_COM8, I2CSlaveBuffer[0]|(0x1 << 1));   //	 COM8[1] = 1
	 ov7670_set(REG_AWBCTR0, 0x0D);	  // Normal Automatic mode a full en RGB y ganancia max x4
	}
	else if (mode == 0x02) {
	 	ov7670_read(REG_COM8);							 		 // LEER REGISTRO
   	 	ov7670_set(REG_COM8, I2CSlaveBuffer[0]|(0x1 << 1));   //	 COM8[1] = 1
		ov7670_set(REG_AWBCTR0, 0x0C);	  // Advanced Autmatic mode a full en RGB y ganancia max x4
 	}
}

/******************************************************************************
* Function Name  : AWBC_control 
* Description    : Configures the parameters of the algorith used for the Automatic White Balance
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None

*******************************************************************************/
void AWBC_control (void) {
	ov7670_set(0x43, 0x14);
	ov7670_set(0x44, 0xf0);
	ov7670_set(0x45, 0x34);
        
	ov7670_set(0x46, 0x58);
	ov7670_set(0x47, 0x28);
	ov7670_set(0x48, 0x3a);
	ov7670_set(0x59, 0x88);
	ov7670_set(0x5a, 0x88);
        
	ov7670_set(0x5b, 0x44);
	ov7670_set(0x5c, 0x67);
	ov7670_set(0x5d, 0x49);
	ov7670_set(0x5e, 0x0e);
}

/******************************************************************************
* Function Name  : set_Umbral 
* Description    : Sets the umbral for the color-based detection.
				   This umbral is the maximum Desviation of R,G	and B we accept
				   from the color we chose as object.
* Input          : - um: Umbral (0-31) 
* Output         : None
* Return         : None
* Attention		 : None

*******************************************************************************/  
   void set_Umbral (uint8_t um){
   		umbral = um;
	}

/******************************************************************************
* Function Name  : OV7670_FPS
* Description    : Sets the number of frames per second we want to recieve
				   It doest it configuring the OV7670 register so that it gives out
				   a number a FPS and then we reduce it or not with the ucontroller.
* Input          : - fps: Number of fps we want.
* Output         : None
* Return         : None
* Attention		 : None

*******************************************************************************/  
void OV7670_FPS (uint8_t fps) {

	OV7670fps = fps;
   if ((fps == 1)|(fps == 3)|(fps == 5)|(fps == 15)|(fps == 7)) {
   		ov7670_set(REG_CLKRC, 0); 		  // Configured to 15 fps
		ov7670_set(REG_DBLV, 0x00); 
		fps_divider = 15/fps;
   }
   if ((fps == 2)|(fps == 10)|(fps == 30)) {
   		ov7670_set(REG_CLKRC, 1); 		  // Configured to 30 fps
		ov7670_set(REG_DBLV, 64); 
		fps_divider = 30/fps;
   }
 
   if (fps == 12) {
   		ov7670_set(REG_CLKRC, 4); 		  // Configured to 30 fps
		ov7670_set(REG_DBLV, 64); 
 		fps_divider = 1;
   }
   if (fps == 20) {
   		ov7670_set(REG_CLKRC, 2); 		  // Configured to 30 fps
		ov7670_set(REG_DBLV, 64); 
		fps_divider = 1;
   }


   if (fps == 30) {
   		ov7670_set(REG_CLKRC, 1); 		  // Configured to 30 fps
		ov7670_set(REG_DBLV, 64); 
		fps_divider = 1;
   }

   if (fps == 13) {
   		ov7670_set(REG_CLKRC, 6); 		  // Configured to 30 fps
		ov7670_set(REG_DBLV, 128); 
		fps_divider = 1;
   }

   if (fps == 22) {
   		ov7670_set(REG_CLKRC, 3); 		  // Configured to 30 fps
		ov7670_set(REG_DBLV, 128); 
		fps_divider = 1;
   }

   if (fps == 24) {
   		ov7670_set(REG_CLKRC, 4); 		  // Configured to 30 fps
		ov7670_set(REG_DBLV, 192); 
		fps_divider = 1;
   }

    if (fps == 40) {
   		ov7670_set(REG_CLKRC, 2); 		  // Configured to 30 fps
		ov7670_set(REG_DBLV, 192); 
		fps_divider = 1;
   }

    if (fps == 60) {
   		ov7670_set(REG_CLKRC, 0); 		  // Configured to 30 fps
		ov7670_set(REG_DBLV, 64); 
		fps_divider = 1;
   }

}
/******************************************************************************
* Function Name  : reset_OV7670
* Description    : Resets the values of the main registers of the camera that we can change.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None

*******************************************************************************/  
void reset_OV7670 (void) {

	pass = 0;
    ov7670_set(0x4f, 0x80);
    ov7670_set(0x50, 0x80);
	
    ov7670_set(0x51, 0x00);
    ov7670_set(0x52, 0x22);
    ov7670_set(0x53, 0x5e);

    ov7670_set(0x54, 0x80);
    ov7670_set(0x56, 0x40);
    ov7670_set(0x58, 0x9e);
    ov7670_set(0x59, 0x88);
    ov7670_set(0x5a, 0x88);  
    ov7670_set(0x5b, 0x44);
    ov7670_set(0x5c, 0x67);
    ov7670_set(0x5d, 0x49);

    ov7670_set(0x5e, 0x0e);
    ov7670_set(0x69, 0x00);
    ov7670_set(0x6a, 0x40);

    ov7670_set(0x6c, 0x0a);
    ov7670_set(0x6d, 0x55);
    ov7670_set(0x6e, 0x11);
    ov7670_set(0x6f, 0x9f);	

	OV7670_AWBmode (1);			// Ganancia de color automatica
	getOV7670_RGBgain ();
	getOV7670_BriConLum ();
	OV7670_FPS(20);			

pass = 1;	
}
/******************************************************************************
* Function Name  : resolution
* Description    : This funcions DownSamples the VGA resolution given by the camera.
				   So that the camera gives out the standar resolution we want.
				   This funcion just uses the Downsampler of the camera
* Input          : - num: Chooses the resolution:
						0-> VGA
						1-> QVGA
						2-> QQVGA
						3-> QQQVGA
* Output         : None
* Return         : None
* Attention		 : This funcion will also change the number of FPS since low resolutions need
				   a lot of FPS in order to be achieved.

*******************************************************************************/  
void resolution (uint8_t num){
	resol_type = num;
	if ((num >= 1)&&(num <= 3)) {
		ov7670_set(REG_COM14, 0x18 + num);
		ov7670_set(0x72, num + (num << 4));		   // Downsampler	
		ov7670_set(0x73, 0xF0 + num);	 // PCLK_DIV for timing			  Para 1/8 -> Delay = 41   si hecho con scaler
		try_QCIF_W = 640/(1<<num);
		try_QCIF_H = 480/(1<<num);
		OV7670_FPS (10*(num+1));	// Coz the lower the resolution, the more fps we need
		ov7670_set(0xa2, 0x02);  // SCALING_PCLK_DELAY
		if (num == 3) {
				ov7670_set(0xa2, 0x00);
		}
	}
}
/******************************************************************************
* Function Name  : timing_resol 
* Description    : Sets the values of the timing registers.
				   These registers have to be configured accordingly to the resolution.
* Input          : - num: Chooses the resolution:
 					- num: Chooses the resolution:
 					- num: Chooses the resolution:
* Output         : None
* Return         : None
* Attention		 : None

*******************************************************************************/ 
void timing_resol (uint8_t num1, uint8_t num2, uint8_t num3) {
			ov7670_set(REG_COM14, 0x18 + num1);
		 	ov7670_set(0x73, 0xF0 + num2);	 // PCLK_DIV for timing	
			ov7670_set(0xa2, num3);			  // SCALING_PCLK_DELAY
}
/******************************************************************************
* Function Name  : scaler
* Description    : Scales the image that the camera gives out.
				   It uses the OV7670 scaler to do so (width and height separately)
* Input          : - XSC: Width Scalation 
  				   - YSC: Height Scalation 
* Output         : None
* Return         : None
* Attention		 : Images scaled with this function can only be seen properly in the mode XXXX
				   This function also scales the LCD screen to the proper size.
				   Scaling the XSC (Width) implies that we have to change the SCALING_PCLK_DELAY register
				   The value we give has to be rather empiric.

*******************************************************************************/ 
void scaler (uint8_t XSC, uint8_t YSC){
	if ((num >= 0)&&(num <= 0x20)) {
	ov7670_set(0x70, 0x20+XSC);		  /* SCALING_XSC: Para escalar el X de 0x20 a 0x40 para efecto*/
	ov7670_set(0x71, 0x20+YSC);		  /* SCALING_XSC: Para escalar el Y de 0x20 a 0x40 para efecto*/
	try_QCIF_W = ((640/(1<<resol_type))*0x20)/(0x20+XSC);
	try_QCIF_H = ((480/(1<<resol_type))*0x20)/(0x20+YSC);
	if (XSC >3) {
   	ov7670_set(0xa2, ((640/(1<<resol_type))-try_QCIF_W) + (2*(8- XSC)*2)/(1<<(resol_type -1))+((XSC-8)/8)*((XSC-8)/8));  // SCALING_PCLK_DELAY
	}
	else {
		ov7670_set(0xa2,0x2);
	}	
	}
}
/******************************************************************************
* Function Name  : OV7670_mirror
* Description    : It does a Vertical and/or hozizontal mirror to the screen.
				   It uses the OV7670 scaler to do so (width and height separately)
* Input          : - mirr: Mirror controler (0-3):
						0-> No mirror
						1-> Vertical mirror
						2-> Horizontal mirror
						3-> Vertical and Horizontal mirror
* Output         : None
* Return         : None
* Attention		 : None

*******************************************************************************/
void OV7670_mirror (uint8_t mirr){
	if (mirr < (1 << 2)){
		ov7670_read(0x1E);							 		 // LEER REGISTRO
   	 	ov7670_set(0x1E, I2CSlaveBuffer[0]^(mirr << 4));   

	}
}
