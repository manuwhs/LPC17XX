#include "LPC17xx.h"
#include "I2C.h"
#include "ov7670.h"
#include "ov7670reg.h"
#include "../GLCD/GLCD.h"
#include <string.h>
#include "../TIMER/timer.h" 
#include "../UART/uart.h" 
void conversor_IaS(int numero, char * cadnum);

/******************************************************************************
* Function Name  : ov7670_set
* Description    : Sets the value of an ov7670 register via I2C.
				   This function first waits for the I2C bus to be free so if it's not
				   it will stop the system untill it is.

* Input          : - reg_OV:  Address of the register 
*                  - value: Value of the register
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
uint32_t ov7670_set(uint8_t reg_OV, uint8_t value){
	 /* The I2C will be busy while I2CMasterState is PENDING or TRYING
	 It will be trying when we change the config to send and it will be pending
	 when it transferring something (between the START and the STOP) */

	// We first wait until the BUS is free
	while (((I2CMasterState == I2CSTATE_TRYING)||(I2CMasterState == I2CSTATE_PENDING)));	
	// Indicate we are gonna want to send something. We do this coz there is a period of time
	// between the setting up of the parameters and the START condition and the send of 
	// the START condition and to avoid other I2C sending to rewrite the parameters before their are
	// sent we do this.

	I2CMasterState = I2CSTATE_TRYING;	

	// LOAD the data into the I2C Engine
	I2CWriteLength = 3;		
	I2CReadLength = 0;	
	I2CMasterBuffer[0] = 0x42;  /* OV7670 addr + W */	 
	I2CMasterBuffer[1] = reg_OV;  /* Register  */	 
 	I2CMasterBuffer[2] = value;  /* Register  */	 
	I2C_mode = SSCB;		

	LPC_I2C0->I2CONCLR = I2CONCLR_AAC; /* This Master wont send ACK coz SCCB doesnt use it */
	LPC_I2C0->I2CONSET |= (1 << 5); ;	/* Set Start flag */

	return 0;
}
/******************************************************************************
* Function Name  : ov7670_read
* Description    : Reads the value of an ov7670 register via I2C.
				   This function wont end until the value is read so that we
				   have the desired value before doing anything with it.
				   The value of the register will be at I2CSlaveBuffer[0]

* Input          : - reg_OV:  Address of the register 

* Output         : I2CSlaveBuffer[0] = Value of the Register we want to read
* Return         : None
* Attention		 : None
*******************************************************************************/

uint32_t ov7670_read(uint8_t reg_OV){

	while (((I2CMasterState == I2CSTATE_TRYING)||(I2CMasterState == I2CSTATE_PENDING)));

	I2CMasterState = I2CSTATE_TRYING;	

	I2CWriteLength = 2;		
	I2CReadLength = 0;	
	I2CMasterBuffer[0] = 0x42;  /* OV7670 addr + W */	 
	I2CMasterBuffer[1] = reg_OV;  /* Register  */	 
 	I2CMasterBuffer[2] = 0x43;  /* OV7670 addr + R */	 
	I2C_mode = SSCB;		

	LPC_I2C0->I2CONCLR = I2CONCLR_AAC; /* This Master wont send ACK coz SCCB doesnt use it */
	LPC_I2C0->I2CONSET |= (1 << 5); ;	/* Set Start flag */

	while (((I2CMasterState == I2CSTATE_TRYING)||(I2CMasterState == I2CSTATE_PENDING)));

	I2CMasterState = I2CSTATE_TRYING;	

	I2CWriteLength = 1;		
	I2CReadLength = 1;	
 	I2CMasterBuffer[0] = 0x43;  /* OV7670 addr + R */	 
	I2C_mode = SSCB;		

	LPC_I2C0->I2CONCLR = I2CONCLR_AAC; /* This Master wont send ACK coz SCCB doesnt use it */
	LPC_I2C0->I2CONSET |= (1 << 5); ;	/* Set Start flag */

	/* This last line is so that we dont exit this function until we have the read value */
	while (((I2CMasterState == I2CSTATE_TRYING)||(I2CMasterState == I2CSTATE_PENDING)));

	return 0;
}
/******************************************************************************
* Function Name  : Init_OV7670
* Description    : Configure the microcontroller pins that communicate with the oc7670 camara
				   and configures the registers of it.

* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/


void Init_OV7670 (void){

	Init_I2C0();

	/* port 0.25 - VSYNC */
    LPC_PINCON->PINSEL1 &= ~(3 << 18);	 /* Para interrupcion normal -> EINT3 */
   	LPC_GPIO0->FIODIR &= ~(1 << 25);		   /* direction = Input */
	 // We wnable the interup in falling edge which means, a new image is gonna me sent
	 //	_______		___________________________		__________________________	  ________
	 //		   |___|						   |___|					      |___|				VSYNK
	 //        ^    						   ^
	 //		   |New Image will be sent		   |New Image will be sent		   ....
	 // Remember inverting VSYNK using the registers.
	LPC_GPIOINT->IO0IntEnF |= (1 << 25);		/* Falling edge interrupt in P0.26 */
	// Configuramos prioridad de la interrupcion y la activamos:
	NVIC_SetPriority(EINT3_IRQn, 0x06);
	NVIC_EnableIRQ(EINT3_IRQn);

	// We firt MASK the interrupt coz we only wanna get the images when selected in the menu */
//	LPC_GPIO0->FIOMASK |= 1 << 26;	   /* MASKING the interrupt */

	/* port 0.29 - RRST */	  
	// Reset the read FIFO pointer. It only becomes effective at a rising edge of RCLK
    LPC_PINCON->PINSEL1 &= ~(3 << 26); /* function = gpio */
	LPC_GPIO0->FIODIR |= (1 << 29); /* direction = OUTPUT */
	LPC_GPIO0->FIOSET |= (1 << 29);  /* Make pin HIGH to prevent reset */
	LPC_GPIO0->FIOMASK |= 1 << 29;	   /* MASKING */

	/* port 0.30 - WEN */	  
	// Controled by the menu so that the FIFO can read new images
    LPC_PINCON->PINSEL1 &= ~(3 << 28); /* function = gpio */
    LPC_GPIO0->FIODIR |= (1 << 30); /* direction = OUTPUT  */
	LPC_GPIO0->FIOSET |= (1 << 30); /* Make pin HIGH to enable FIFO write */
	LPC_GPIO0->FIOMASK |= 1 << 30;	   /* MASKING  */

	/* port 0.24 - RCLK */	 		 //###### Cambiar a P0.24 ####\\\
	// To control the reading of bytes
    LPC_PINCON->PINSEL1 &= ~(3 << 16); /* function = gpio */
	LPC_GPIO0->FIODIR |= (1 << 24); /* direction = OUTPUT */
	LPC_GPIO0->FIOCLR |= (1 << 24);  /* Make pin LOW  */

	/*  OE -> Mass --> Always allowed */
   	/*  HREF -> Not used, not connected*/


    /* Puerto D[0-7] */

	LPC_PINCON->PINSEL3 &= ~(3 << 4);		// P1.18
	LPC_PINCON->PINSEL3 &= ~(3 << 6);		// P1.19
	LPC_PINCON->PINSEL3 &= ~(3 << 8);		// P1.20
	LPC_PINCON->PINSEL3 &= ~(3 << 10);		// P1.21
	LPC_PINCON->PINSEL3 &= ~(3 << 12);		// P1.22
	LPC_PINCON->PINSEL3 &= ~(3 << 14);		// P1.23
	LPC_PINCON->PINSEL3 &= ~(3 << 16);		// P1.24
	LPC_PINCON->PINSEL3 &= ~(3 << 18);		// P1.25

	// Tenemos libres P0.4 P0.5
	//                P0.14 y P0.24	   P0.10 - P0.11	  P0.0 y P0.1
	// Alomejor lo ideal seria cuanto mas seguido mejor
	 // Configuramos como entradas

	LPC_GPIO1->FIODIR &= ~(1 << 18);
	LPC_GPIO1->FIODIR &= ~(1 << 19);
	LPC_GPIO1->FIODIR &= ~(1 << 20);
	LPC_GPIO1->FIODIR &= ~(1 << 21);
	LPC_GPIO1->FIODIR &= ~(1 << 22);
	LPC_GPIO1->FIODIR &= ~(1 << 23);
	LPC_GPIO1->FIODIR &= ~(1 << 24);
	LPC_GPIO1->FIODIR &= ~(1 << 25);


//**************************************************************************\\
/********************** CAMERA INITIALIZATION  ********************/
//**************************************************************************\\
 
 
  	ov7670_set(REG_TSLB, 0x04);	   // TSLB --> Si 32 Imagen en negativo EL bit 0 es para ajuste de resolucion o algo asi
	ov7670_set(REG_COM15, 0x90);    // COM15 -> Poner 11 01 0000 para poner RGB 565 con rango 0-FF  
							   // El display solo se ve bien con rango [01] to [FE]  -->	 10 01
	ov7670_set(REG_COM7, 0x04);	   // COM7 para seleccionar el formato
							   /*
								 1000 0000 pa reset
								 Tenemos QVGA y RGB con 0001 0100 
								 Si queremos las barras de colores ponemos 0001 0110


							   */

	/********************** WINDOWING ********************/
	ov7670_set(REG_HREF, 0x80);			  // La primer columna sale mal.. habria que desplazar 1 todo
	ov7670_set(REG_HSTART, 0x16);		  // 0x16 es el ini y 0x4 el fin, si incremente el este lo hace bien 	  
	ov7670_set(REG_HSTOP, 0x04);		 // si mantengo las distancia se lee todo el frame pero columnas
											// raras	  
	ov7670_set(REG_VREF, 0x00);	  //		  0000 00 00   
	ov7670_set(REG_VSTART, 0x03);	//  0000 0010 	   --> 12      
	ov7670_set(REG_VSTOP, 0x7b);		//  0111 1011  	   -->	492 	Moverse aqui...

	/**************   FPS **************/
		//Con estos 2 podemos controlar correctamente los frames por segundo pero la imagen sale
	//atropellada, habra que cambiar otros registros

	ov7670_set(REG_CLKRC, 0x01);  // Prescaler	 CLKCRC
	ov7670_set(REG_DBLV, 0x4A);  // PLL !!!! DBLV	   PAsa de 60 a 15
							 /* 2 MSB los usamos para multiplicar la frec...  */
							 /* Se combina con COM 14 */
	
 

	/* Dividiendo PCLK por 1 o 2 no hace nada, por 4 solo escribe la mitad de la imagen 
	y son dos imagenes iguales de 1/4 seguidas
	Entre 8 son 4 imagenes de 1/8 seguidas Puede servir para escalar imagenes*/
	/* Si habilitamos que la escala sea hecha manualmente y no de forma predeficinida mendiante el bit 3 o acti
	vamos el bit 4 tambien para la scale del PCLK entonces
	de entrada nos da VGA en vez de QVGA y ahora dividiendo el PCLK por 2 volvemos a la imagen de QVGA */

   /************ SCALING ********/

   	ov7670_set(REG_COM3, 0x0C);		/* Enable Downsampler y Escaler */
	ov7670_set(REG_COM14, 0x19);	/* Enable Downsampler y Escaler y dividimos PCLK entre 2 para QVGA*/	 

	ov7670_set(0x70, 0x00);		  /* SCALING_XSC: Para escalar el X de 0x20 a 0x40 para efecto*/
	ov7670_set(0x71, 0x01);		  /* SCALING_XSC: Para escalar el Y de 0x20 a 0x40 para efecto*/

	ov7670_set(0x72, 0x11);		   // Downsampler a la mitad de VGA

			
	ov7670_set(0x73, 0xF1);	 // PCLK_DIV for timing
   	ov7670_set(0xa2, 0x02);  // SCALING_PCLK_DELAY
	       
	/************************* OTRAS COSAS ******************* */  



	/* GAMMA RELATED */
	ov7670_set(0x7a, 0x20);
	ov7670_set(0x7b, 0x1c);
	ov7670_set(0x7c, 0x28);
      
	ov7670_set(0x7d, 0x3c);
	ov7670_set(0x7e, 0x55);
	ov7670_set(0x7f, 0x68);
	ov7670_set(0x80, 0x76);
	ov7670_set(0x81, 0x80);
        
	ov7670_set(0x82, 0x88);
	ov7670_set(0x83, 0x8f);
	ov7670_set(0x84, 0x96);
	ov7670_set(0x85, 0xa3);
	ov7670_set(0x86, 0xaf);
        
	ov7670_set(0x87, 0xc4);
	ov7670_set(0x88, 0xd7);
	ov7670_set(0x89, 0xe8);

	/* *********  *************/

	ov7670_set(REG_COM8, 0xe0);	  // AGC/AEC enable y Banding Filter ON
	ov7670_set(0x00, 0x00);
        
	ov7670_set(0x10, 0x00);
	ov7670_set(0x0d, 0x00);
	ov7670_set(REG_COM9, 0x20);
	ov7670_set(0xa5, 0x05);
	ov7670_set(0xab, 0x07);

     /* Brillo y Contraste */   
	ov7670_set(0x24, 0x75);
	ov7670_set(0x25, 0x63);
	ov7670_set(0x26, 0xA5);
	ov7670_set(0x9f, 0x78);
	ov7670_set(0xa0, 0x68);
        
	ov7670_set(0xa1, 0x03);
	ov7670_set(0xa6, 0xdf);
	ov7670_set(0xa7, 0xdf);
	ov7670_set(0xa8, 0xf0);
	ov7670_set(0xa9, 0x90);
        
	ov7670_set(0xaa, 0x94);
	ov7670_set(0x13, 0xe5);
	ov7670_set(0x0e, 0x61);
	ov7670_set(0x0f, 0x4b);
	ov7670_set(0x16, 0x02);
        
	ov7670_set(0x1e, 0x37); 
	ov7670_set(0x21, 0x02);
	ov7670_set(0x22, 0x91);
	ov7670_set(0x29, 0x07);
	ov7670_set(0x33, 0x0b);
        
	ov7670_set(0x35, 0x0b);
	ov7670_set(0x37, 0x1d);
	ov7670_set(0x38, 0x71);
	ov7670_set(0x39, 0x2a);
	ov7670_set(0x3c, 0x78);
        
	ov7670_set(0x4d, 0x40);
	ov7670_set(0x4e, 0x20);
	ov7670_set(0x69, 0x5d);
	ov7670_set(0x74, 0x19);
	ov7670_set(0x8d, 0x4f);
        
	ov7670_set(0x8e, 0x00);
	ov7670_set(0x8f, 0x00);
	ov7670_set(0x90, 0x00);
	ov7670_set(0x91, 0x00);
	ov7670_set(0x92, 0x00);
        
	ov7670_set(0x96, 0x00);
	ov7670_set(0x9a, 0x80);
	ov7670_set(0xb0, 0x84);
	ov7670_set(0xb1, 0x0c);
	ov7670_set(0xb2, 0x0e);
        
	ov7670_set(0xb3, 0x82);
	ov7670_set(0xb8, 0x0a);


	ov7670_set(0x64, 0x04);
	ov7670_set(0x65, 0x20);
        
	ov7670_set(0x66, 0x05);
	ov7670_set(0x94, 0x04);
	ov7670_set(0x95, 0x08);
	ov7670_set(0x6c, 0x0a);
	ov7670_set(0x6d, 0x55);
                
	ov7670_set(0x4f, 0x80);
	ov7670_set(0x50, 0x80);
	ov7670_set(0x51, 0x00);
	ov7670_set(0x52, 0x22);
	ov7670_set(0x53, 0x5e);
	ov7670_set(0x54, 0x80);

	ov7670_set(REG_COM10, 0x02);   // Change VSYNK default polarity


	OV7670_AWBmode (1);			// Ganancia de color automatica
	getOV7670_RGBgain ();

	getOV7670_BriConLum ();
	OV7670_FPS(20);
	OV7670_mirror(1);
}	 
