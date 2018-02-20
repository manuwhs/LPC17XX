/**************************************************************************//**
 * @file     main_srf04.c
 * @brief    Example of code for measuring the distance in a SRF04 ultrasonic
 *           sensor
 * @author   JPM
 * @version  V1.0
 * @date     18. September 2013
 *
 * @par
 * This software generates a trigger for srd04 ultrasound distance sensor
 * and measure pulse width in microseconds when a complete pulse
 * has been received. 
 *
 * @par
 * This software is supplying for academical use for students of 
 * Advanced Digital Electronic Systems at University of Alcalá
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/

#include "servos.h"
#include "uart.h"

// Constants 



float ADC_value_Alt = 0; ///< Contains the 8 MSB of the 12-bit ADC measure
float ADC_value_Azim = 0;
float x = 0;
float y = 0;
float x_ant = 0;
float y_ant = 0;
float umbral_x = 0.64;  //0.47;
float umbral_y = 0.45; //0.55;
//uint32_t det_x = 160;
//uint32_t det_y = 120;

volatile uint32_t resul_1;
volatile uint32_t resul_2;
volatile uint8_t modo = MANUAL;
volatile float ADC_x = 0.5;
volatile float ADC_y = 0.5;

volatile float cons_pro_x = 2;
volatile float cons_pro_y = 2;

volatile float cons_pro_x_cam = 0.7;
volatile float cons_pro_y_cam = 0.5;

volatile float cons_dif_x = 0.6;
volatile float cons_dif_y = 0.6;

volatile float cons_int_x = 0.1;
volatile float cons_int_y = 0.1;

void Init_Servos (void) {
	Init_PWM();
	Init_ADC();
}


