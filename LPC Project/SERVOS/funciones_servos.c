
#include "servos.h"

void Init_PWM() {
	
		LPC_SC->PCONP |= 1<<6; /* enable PWM1 Power */

		LPC_GPIO0->FIODIR |= (1 << 1);	 // Control motor 1
		LPC_GPIO0->FIODIR |= (1 << 11);	 // Control motor 0

		LPC_GPIO0->FIOMASK |= (1 << 1);  //Enmascaramos para que no haya conflicto con otras partes del programa
		LPC_GPIO0->FIOMASK |= (1 << 11);	//Enmascaramos para que no haya conflicto con otras partes del programa

		LPC_PINCON->PINSEL7 |= (3<<20); /* set GPIOs for PWM1.2 pin on PWM P3.26*/
		LPC_PINCON->PINSEL7 |= (3<<18);	/* set GPIOs for PWM1.2 pin on PWM P3.25*/
		LPC_PWM1->TCR = 0x00000002;		/* Counter Reset */
	
		LPC_PWM1->PR = PRESCALER_T0;		/* count frequency: pclk */

		LPC_PWM1->MCR = (1<<1);	/* reset on PWMMR0 match*/

		LPC_PWM1->MR0 = PERIOD_PWM;		/*valor para el periodo de la señal PWM*/
		/* set PWM cycle */
		LPC_PWM1->MR3 = PERIOD_PWM/2; /* valor para un ciclo de trabajo del 50%*/
		
		LPC_PWM1->MR2 = PERIOD_PWM/2;	/* valor para un ciclo de trabajo del 50%*/	

		LPC_PWM1->LER |= (1 << 0) | (1 << 3) | (1 << 2); /* PWM 0 , 2 y 3 latch enabled */
	
		LPC_PWM1->PCR |= (3<<10); /* Habilita PWM2 y PWM3 modo simple*/

		LPC_PWM1->TCR = (0x1<<0) | (0x1<<3); /* counter enable, PWM enable */

}
void Init_ADC(void) {

	LPC_SC->PCONP |= (1 << 12) ; /* Enable power to ADC*/

	// Set frecuency
//	LPC_SC->PCLKSEL0 &= ~ ( (1 << 24) |(1 << 25) ) ; /* Select CLK/4 = 25 MHz*/
	LPC_ADC->ADCR = ( 10 << 8 );  /* ADC prescaler set to 5 to get a 5 MHz ADC frecuency */

	// Config Input pins
	LPC_PINCON->PINSEL3 |= ( 3 << 28);
	LPC_PINCON->PINSEL3 |= ( 3 << 30); /* P0.25 y 0.26 is AD0.4 y AD0.5*/
	LPC_PINCON->PINMODE3 &= ~( 3 << 28);
	LPC_PINCON->PINMODE3 &= ~( 3 << 30);
	LPC_PINCON->PINMODE3 |= ( 2 << 28); /* P0.25 no pull up/down */
	LPC_PINCON->PINMODE3 |= ( 2 << 30);	/* P0.25 no pull up/down */

	// Select AD0.2 pin channel to be converted	on normal mode
//	LPC_ADC->ADCR |= ( 1 << 16 ); /* No BURST mode  ADCR.16 = 0*/
	LPC_ADC->ADCR |= ( 1 << 4);  /* select AD0.4 pin de inicio (se activará el ADC0.5 en la interrupción del ADC) */
//	LPC_ADC->ADCR |= ( 1 << 5);	 /* select AD0.5 pin */

	// Select the "Init Convertion" signal

	LPC_ADC->ADCR &= ~( 7 << 24);		/*Borramos los bits de inicio de conversión por software*/
//	LPC_ADC->ADCR &= ~ ( 1 << 27 );	  /* On rising edge */

    // Enable interrupt on convertion on any channel
	LPC_ADC->ADINTEN &= ~( 1 << 8); 		/* Provoque interrupt whenever a conversion is done */
	LPC_ADC->ADINTEN |= ( 1 << 4);		/* Provoque interrupt for channel 4 done */
	LPC_ADC->ADINTEN |= ( 1 << 5);		/* Provoque interrupt for channel 5 done */
	NVIC_SetPriority(ADC_IRQn,0x05); 	/* Priority of the interrupt*/
  	NVIC_EnableIRQ(ADC_IRQn);		   /* ADC interrupt enable */

	// CONFIGURE DE "INIT CONVERTION" SIGNAL on MAT0.1		 XXXXXXXXXXXX
	Init_MAT();

	// Enable ADC (Because when we powet it up it's in low power mode)
	LPC_ADC->ADCR |=( 1 << 21); /* enable ADC */
	LPC_ADC->ADCR |=( 1 << 24); /* START Conversion */
}

void Init_MAT(void) {

	// POWER UP THE TIMER  AND SET IT UP AS A TIMER
	LPC_SC->PCONP |= (0x01<<1); /* Alimentación del timer */
	LPC_TIM0->CTCR &= ~ (3 << 0);

	// RESET TIMER (AND PC) TO AVOID REGISTER OVERFLOW AND THEN STOP IT
	LPC_TIM0->TCR = 0x3;		 /* RESET */
	LPC_TIM0->TCR = 0x0;		 /* DISABLE (STOP COUNTER)*/

	// Configure the TIMER0 frecuency to 1 MHz

	LPC_SC->PCLKSEL0 &= ~ (3 << 2);		/* Select PCLK /4 = 25 MHz */
	LPC_TIM0->PR = PRESCALER_T0;		/* Divides frecuency by 25 */
	
	// Configure the pins for MAT01
//	LPC_PINCON->PINSEL3 |= (3 << 26);	 /* Select MAT0.1 (P1.29) pin for output match */
	
	// Config TIMER0 interrupt in NVIC 
  	NVIC_SetPriority(TIMER0_IRQn,0x02);
  	NVIC_EnableIRQ(TIMER0_IRQn);	
	

	// Disable all MAT0.1 interrupt, reset and unable of TC when Match on MR01
	LPC_TIM0->MCR &=  ~(7 << 3); 

	// Enable TIMER0 (Starts Counting)
	LPC_TIM0->TCR |= (1 << 0);

	//SET INTERRUPT AND REGISTER TO START THE PERIODIC WAVE  

    // Config delay time
	LPC_TIM0->MR1 = LPC_TIM0->TC + PERIOD_TIM;
	LPC_TIM0->MR2 = LPC_TIM0->TC + PERIOD_TIM/ADC_MUEST;

	// Enable MAT0.1 interrupt when match
	LPC_TIM0->MCR |=  (1 << 3) | (1 << 6);
}
void PWM_Azimut_Set( float duty_x ) {	/* Cargar el ciclo de trabajo al PWM del eje x */
	LPC_PWM1->MR2 = (duty_x*PERIOD_PWM);	
	LPC_PWM1->LER |=(1<<2);
}

void PWM_Altura_Set( float duty_y ) {	/* Cargar el ciclo de trabajo al PWM del eje y */
	LPC_PWM1->MR3 = (duty_y*PERIOD_PWM);	
	LPC_PWM1->LER |=(1<<3);
} 

void New_pos( float PWM_Alt, float PWM_Azimut ) 	/* Función para establecer nuevos ciclos de trabajo */
{
	PWM_Azimut_Set(PWM_Azimut);
	PWM_Altura_Set(PWM_Alt);	
} 

void Control_PID()		/* Función de cálculo del movimiento de los motores por control PID y BORROSO combinados*/
{
	float dif_x = (x - x_ant)* cons_dif_x;		/* Cálculo del error diferencial eje x*/
	float dif_y = (y - y_ant)* cons_dif_y;		/* Cálculo del error diferencial eje y*/
	float Proporcional_x = (x-umbral_x) * cons_pro_x;		/* Cálculo del error proporcional eje x*/
	float Proporcional_y = (y-umbral_y) * cons_pro_y;		/* Cálculo del error proporcional eje x*/
	float duty_x = Proporcional_x + dif_x;		/*Cálculo del ciclo de trabajo para el motor x*/
	float duty_y = Proporcional_y + dif_y;		/*Cálculo del ciclo de trabajo para el motor y*/
	
	
	if(modo == TRACKING)
	{
		dif_x = (x - x_ant)* cons_dif_x;
		dif_y = (y - y_ant)* cons_dif_y;
		Proporcional_x = (umbral_x - x) * cons_pro_x;
		Proporcional_y = (umbral_y - y) * cons_pro_y;
		duty_x = Proporcional_x + dif_x;
		duty_y = Proporcional_y + dif_y;	
	}
	
	if ((x <(umbral_x+0.01)) && (x >(umbral_x-0.01)))
	{
						duty_x = 0;
						LPC_GPIO0->FIOMASK &= ~(1 << 1); 
						LPC_GPIO0->FIOCLR |= (1 << 1);
						LPC_GPIO0->FIOMASK |= (1 << 1);
	}
	else
	{
		
				if (Proporcional_x > 0)
				{
						duty_x = 1 - duty_x;
						LPC_GPIO0->FIOMASK &= ~(1 << 1); 
						LPC_GPIO0->FIOSET |= (1 << 1);   // Lo ponemos a uno para que la diferencia de potencial sea con referencia a VCC
						LPC_GPIO0->FIOMASK |= (1 << 1);
				}
				else
				{
						duty_x = - duty_x;
						LPC_GPIO0->FIOMASK &= ~(1 << 1); 
						LPC_GPIO0->FIOCLR |= (1 << 1);
						LPC_GPIO0->FIOMASK |= (1 << 1);		
				}
		
	}
	if (y <(umbral_y+0.01) && y > (umbral_y-0.01))
	{
						duty_y = 0;
						LPC_GPIO0->FIOMASK &= ~(1 << 11); 
						LPC_GPIO0->FIOCLR |= (1 << 11);
						LPC_GPIO0->FIOMASK |= (1 << 11);
	}
	else
	{
		
				if (Proporcional_y > 0)
				{
					duty_y = 1 - duty_y;
					LPC_GPIO0->FIOMASK &= ~(1 << 11); 
					LPC_GPIO0->FIOSET |= (1 << 11);   // Lo ponemos a uno para que la diferencia de potencial sea con referencia a VCC
					LPC_GPIO0->FIOMASK |= (1 << 11);
				}
					else
					{
						duty_y = - duty_y;
						LPC_GPIO0->FIOMASK &= ~(1 << 11); 
						LPC_GPIO0->FIOCLR |= (1 << 11);
						LPC_GPIO0->FIOMASK |= (1 << 11);			
					}
	}
	if(ADC_x < 0.14	||  ADC_x > 0.9)
	{
		if(ADC_x < 0.14 && Proporcional_x > 0){
			duty_x = 0;
			LPC_GPIO0->FIOMASK &= ~(1 << 1); 
			LPC_GPIO0->FIOCLR |= (1 << 1);
			LPC_GPIO0->FIOMASK |= (1 << 1);		
		}
		if(ADC_x > 0.9 && Proporcional_x < 0){
			duty_x = 1;
			LPC_GPIO0->FIOMASK &= ~(1 << 1); 
			LPC_GPIO0->FIOSET |= (1 << 1);   // Lo ponemos a uno para que la diferencia de potencial sea con referencia a VCC
			LPC_GPIO0->FIOMASK |= (1 << 1);
		}	
	}
	if(ADC_y < 0.1  || 	ADC_y > 0.8)
	{
		if(ADC_y < 0.1 && Proporcional_y > 0){
			duty_y = 0;
			LPC_GPIO0->FIOMASK &= ~(1 << 11); 
			LPC_GPIO0->FIOCLR |= (1 << 11);
			LPC_GPIO0->FIOMASK |= (1 << 11);	
		}
		if(ADC_y > 0.8 && Proporcional_y < 0){
			duty_y = 1;
			LPC_GPIO0->FIOMASK &= ~(1 << 11); 
			LPC_GPIO0->FIOSET |= (1 << 11);   // Lo ponemos a uno para que la diferencia de potencial sea con referencia a VCC
			LPC_GPIO0->FIOMASK |= (1 << 11);
		}	
	}			
						New_pos(duty_y, duty_x);
						x_ant = x;
						y_ant = y;
	

}










