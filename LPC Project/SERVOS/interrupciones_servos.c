#include "servos.h"

void TIMER0_IRQHandler(void) {

	// Check if MR1 match interrupt flag is active
	if (LPC_TIM0->IR & (1 << 1)) 
	{	
	    	LPC_TIM0->IR = 0xFF; /* Clear MR0 interrupt flag */

		LPC_TIM0->MR1 += PERIOD_TIM; 	
	 	LPC_TIM0->MR2 += PERIOD_TIM/ADC_MUEST;
		if(modo == MANUAL)
		{
				ADC_value_Azim = resul_1; /* Store converted value */
				x=ADC_value_Azim/ADC_VALUE_MAX;
				ADC_x = ADC_value_Azim/ADC_VALUE_MAX;
		
				ADC_value_Alt = resul_2; /* Store converted value */
				ADC_y = ADC_value_Alt/ADC_VALUE_MAX;

				y=ADC_value_Alt/ADC_VALUE_MAX;
		}
		if(modo == TRACKING)
		{
				cons_pro_x = cons_pro_x_cam;
				cons_pro_y = cons_pro_y_cam;
				cons_dif_x = 0.5;
				cons_dif_y = 0.5;
				ADC_value_Azim = resul_1; /* Store converted value */
				x=(float)det_x/SCREEN_X_MAX;
				if(det_x < 20 || det_x > 300)
				{
						x = 0.5;
				}
				ADC_x = ADC_value_Azim/ADC_VALUE_MAX;
				umbral_x= 0.5;
				ADC_value_Alt = resul_2; /* Store converted value */
				ADC_y = ADC_value_Alt/ADC_VALUE_MAX;
				umbral_y= 0.5;
				y= (float)det_y/SCREEN_Y_MAX;
				if(det_y < 20 || det_y > 220)
				{
						y = 0.5;
				}
		}
	 	Control_PID();
	 }
	 if (LPC_TIM0->IR & (1 << 2)) 
	 {
	 	LPC_TIM0->IR = 0xFF;
	 	LPC_TIM0->MR2 += PERIOD_TIM/ADC_MUEST;
	  	LPC_ADC->ADCR |= (1 << 24);
	 }
	 
  }

 void ADC_IRQHandler(void) {
	volatile uint32_t adgdr;
	adgdr = LPC_ADC->ADSTAT;
	adgdr = LPC_ADC->ADGDR; /* Read ADC clears interrupt */
	LPC_ADC->ADCR &= ~(7 << 24);
	if(((LPC_ADC->ADDR4 >> 31) & 0x1) == 1)
	{
		resul_1 = ((LPC_ADC->ADDR4 >> 6) & 0x3FF);
		LPC_ADC->ADCR &= ~( 1 << 4);
		LPC_ADC->ADCR |= ( 1 <<5);
		
	}
	else if(((LPC_ADC->ADDR5 >> 31) & 0x1) == 1)
	{
		resul_2 = ((LPC_ADC->ADDR5 >> 6) & 0x3FF);
		LPC_ADC->ADCR &= ~( 1 << 5);
		LPC_ADC->ADCR |= ( 1 << 4);
	} 
//	LPC_ADC->ADCR |= (1 << 24);

}

