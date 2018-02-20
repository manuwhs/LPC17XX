
#ifndef __SERVOS__
#define __SERVOS__

#include "LPC17xx.h"


#define PULSE_WIDTH     10000		      ///< Pulse width in microseconds
#define PERIOD_TIM   	10000		 ///< Period of the ADC convertion in us
#define PRESCALER_T0  	24	  	 /// < Frecuency dividor 
#define ADC_VALUE_MAX  0x3FF 	///< Maximum value of the ADC
#define PERIOD_PWM  6000 	
#define FRECUENCY 1000000
#define ADC_ALT		0
#define ADC_AZIM	1
//#define CONS_X	 1.5 //75/10000
//#define CONS_Y	 1.5 //75/10000
//#define CONS_DIF_X	0.5//1/5000
//#define CONS_DIF_Y	0.5//1/5000
#define ADC_MAX  0xF60
#define ADC_MIN  0x09F
#define MANUAL  1
#define TRACKING  0
#define ADC_MUEST 10
#define SCREEN_X_MAX 320
#define SCREEN_Y_MAX 240

void Init_ADC(void);
void Init_MAT(void);
void Init_PWM(void);
void PWM_Altura_Set( float); 
void PWM_Azimut_Set( float);
void New_pos( float, float );
void Control_PID(void);
void Init_Servos(void);
// Global Variables 

extern float ADC_value_Alt; ///< Contains the 8 MSB of the 12-bit ADC measure
extern float ADC_value_Azim;
extern float x;
extern float y;
extern float x_ant;
extern float y_ant;
extern float umbral_x;
extern float umbral_y;
extern uint32_t det_x;
extern uint32_t det_y;

extern volatile uint32_t resul_1;
extern volatile uint32_t resul_2;
extern volatile uint8_t modo;
extern volatile float ADC_x;
extern volatile float ADC_y;

extern volatile float cons_pro_x;
extern volatile float cons_pro_y;

extern volatile float cons_pro_x_cam;
extern volatile float cons_pro_y_cam;

extern volatile float cons_dif_x;
extern volatile float cons_dif_y;

extern volatile float cons_int_x;
extern volatile float cons_int_y;

#endif

						   



