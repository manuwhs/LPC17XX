/****************************************************************************
 *   $Id:: dac.h 5795 2010-12-03 00:02:09Z usb00423                         $
 *   Project: NXP LPC17xx DAC example
 *
 *   Description:
 *     This file contains DAC code header definition.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/
#ifndef __DAC_H 
#define __DAC_H

#define DAC_DMA_ENABLED		0

#define TIMER_CONTROLLED	0
#define DMA_CONTROLLED		1

#define DAC_BIAS			(0x1<<16)
#define PI	3.1415926
#define MAX_NUM_SAMPLES_CYCLE	500

extern void DACInit( uint8_t mode );
void set_samples_cycle(uint16_t num);
void set_sound( uint32_t frecuency, uint32_t frecuency_ADC);
extern uint32_t time_DAC;
extern uint32_t DAC_frecuency;		// Frecuency at which the DAC is transmiting
extern uint32_t frecuency_sound;	// Frecuency we want to hear
extern uint32_t num_samples_cycle;	// Frecuency we want to hear
extern uint32_t samples_per_cicle;
extern uint32_t buffer_DAC[MAX_NUM_SAMPLES_CYCLE];
extern uint32_t Pulses_DAC;

#endif /* end __DAC_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
