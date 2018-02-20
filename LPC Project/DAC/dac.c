
#include "lpc17xx.h"
#include "dac.h"
#include "../Timer/timer.h" 
#include <math.h>
#include "../GLCD/GLCD.h"
void conversor_IaS(int numero, char * cadnum);

uint32_t buffer_DAC[MAX_NUM_SAMPLES_CYCLE];
uint32_t time_DAC = 0;
uint32_t DAC_frecuency = 1;		// Frecuency at which the DAC is transmiting
uint32_t frecuency_sound = 1;	// Frecuency we want to hear
uint32_t num_samples_cycle =100;
uint32_t samples_per_cicle =100;
uint32_t Pulses_DAC =1;

const uint8_t muelle[2];
const uint8_t click_audio[2];

char cad[10];
/******************************************************************************
* Function Name  : DACInit
* Description    : Configures the DAC to be DMA controlled or not, depending on the selected mode
* Input          : - mode: Selects the mode the DAC is configured:
						0-> Manual Control
						1-> DMA Control
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void DACInit( uint8_t mode){

  LPC_PINCON->PINSEL1 &= ~(3 << 20);	
  LPC_PINCON->PINSEL1 |= (2 << 20);  		// set p0.26 to DAC output  

  LPC_DAC->DACCTRL = 0;	 					// Disable all options
  LPC_DAC->DACR = (0x0<<6)|DAC_BIAS;		// Set voltage to minimum and set BIAS.	 3FF is max

  if (mode == TIMER_CONTROLLED) { 
 	  

  }
  else if (mode == DMA_CONTROLLED) {
  	  LPC_SC->PCLKSEL0 &= ~ (3 << 22);		/* Select PCLK /4 = 25 MHz */
	  LPC_DAC->DACCNTVAL = 3125;	 // Introduce a value at 8.000 Hz
	  LPC_DAC->DACCTRL |= (0x1<<1);	// Enable double buffer
	  LPC_DAC->DACCTRL |= (0x1<<2);	 // Enable counter
	  LPC_DAC->DACCTRL |= (0x1<<3);  // Enable DMA


	}

  return;
}

// Setting sound without DMA, with timer and buffer or calculating the wave form.
/******************************************************************************
* Function Name  : set_sound
* Description    : Generates the 
* Input          : - mode: Selects the mode the DAC is configured:
						0-> Manual Control
						1-> DMA Control
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void set_sound( uint32_t frecuency, uint32_t frecuency_ADC){

	DAC_frecuency = frecuency_ADC;		// Frecuency at which the DAC is transmiting
	frecuency_sound =  frecuency;	// Frecuency we want to hear
	samples_per_cicle = DAC_frecuency/frecuency_sound;
    periodic_signal_us(1000000/(frecuency_sound*num_samples_cycle));

}
/******************************************************************************
* Function Name  : set_samples_cycle
* Description    : Sets the samples per cycle of the sine function that will be driven into the DAC
				   It also has commented code to create other functions and to show them into de LCD
				   It can also show parameters of the reproduction.
* Input          : - num: Number of samples per cycle.
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
// Set samples per cycle of the sine(x) wave.
void set_samples_cycle(uint16_t num){
	uint16_t i;
	num_samples_cycle = num;
	samples_per_cicle = num;
	LCD_Clear(0);
	for (i = 0; i < num_samples_cycle; i++){
		buffer_DAC[i]=((uint16_t)(511 + 512*(sin(2*PI*((float)i/num_samples_cycle))))<<6);
		LCD_SetPoint(5+(i*300)/num_samples_cycle,(5+((buffer_DAC[i]>> 6)*230)>>10),0xFFFF);
		buffer_DAC[i]|=DAC_BIAS;
	}
	/*
	for (i = 0; i < num_samples_cycle/2; i++){
		buffer_DAC[i]=0;
		LCD_SetPoint(5+(i*300)/num_samples_cycle,(5+((buffer_DAC[i]>> 6)*230)>>10),0xFFFF);
		buffer_DAC[i]|=DAC_BIAS;
	}
 	for (i = i; i < num_samples_cycle; i++){
		buffer_DAC[i]=(1023 <<6);
		LCD_SetPoint(5+(i*300)/num_samples_cycle,(5+((buffer_DAC[i]>> 6)*230)>>10),0xFFFF);
		buffer_DAC[i]|=DAC_BIAS;
	}
	*/

	/*
	 for (i = 0; i < MAX_NUM_SAMPLES_CYCLE; i++){
		  buffer_DAC[i]=(muelle[i] <<6)|DAC_BIAS;
	 }
	*/
 /*   periodic_signal_us(1000000/(frecuency_sound*num_samples_cycle));
   	conversor_IaS(frecuency_sound, cad);
	GUI_Text(20, 20,cad,0, 0xFFFF);
   	conversor_IaS(num_samples_cycle, cad);
	GUI_Text(20, 70,cad,0, 0xFFFF);
   	conversor_IaS(1000000/(frecuency_sound*num_samples_cycle), cad);
	GUI_Text(20, 120,cad,0, 0xFFFF);
   	conversor_IaS(Pulses_DAC, cad);
	GUI_Text(20, 170,cad,0, 0xFFFF);
	*/
}

  /* Debo hacer funcion que le pase el array 	   */