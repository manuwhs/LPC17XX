#include <stdio.h>
#include "GLCD/GLCD.h"
#include "GAMES/games.h"
#include "OV7670/ov7670.h"
#include "UART/uart.h" 
#include "Timer/timer.h" 
#include "Touch_Pannel/TouchPanel.h"
#include "SRF04/Ultrasonic.h"
#include "DAC/dac.h"
#include "DMA/dma.h"
#include "WEBSERVER/Configuration_files/HTTP_demo.h"
#include <math.h>
#include "State_Machine/States.h"
#include "SERVOS/servos.h"
#include <RTL.h>
#include <Net_Config.h>
   // Añadir --ASM para obtener codigo ensamblador
void conversor_IaS(int numero, uint8_t * cadnum);
uint8_t cadena[10];
int distance_aux = 0;
extern uint16_t Calibration_vars[9];


int main(void){	 

	int i,j;
	

	// INITIALICE VARIABLES
	 pass = 0;
	 distance_ready = 0;
	 speed_sound = (331 + temperature*0.6) * 100;

	
	state=1;
	draw_flag=1;
	screen_flag=0;
	Detection_color=Black;

	for(i=0;i<9;i++)
	Calibration_vars[i]=80;

	// CONFIGURE  PERIFERICOS
	
	tx_cadena_UART0("Configurando UART0\n\r");	
	Init_UART0(115200);

	tx_cadena_UART0("Configurando Camara\n\r");	
	 Init_OV7670 ();	
		
	tx_cadena_UART0("Configurando TIMERS\n\r");		
   	Init_TIMER1X ();
	periodic_signal_ms(1000);
	 periodic_signal2_ms(35);	 // HIja de puta el sobrecargo hace que el sistema deje de funcionar 
	
	tx_cadena_UART0("Configurando DAC+DMA\n\r");	
	DACInit(1);
	DMA_Init();
	 DMA_DAC_Channel_Init(0);
	 set_sound( 300, 3000);

	tx_cadena_UART0("Configurando SRF04 \n\r");
   	get_distance();

   	tx_cadena_UART0("Configurando Motores\n\r");	
 	 Init_Servos (); 

    tx_cadena_UART0("Configurando LCD\n\r");	
	LCD_Initialization();		
 	 LCD_Clear(bgColor);
	TP_Init(); 
	TouchPanel_Calibrate();

	tx_cadena_UART0("Configurando WEBSERVER \n\r");

    init_TCP ();
	tx_cadena_UART0("Todo configurado\n\r");		
  	 while (1) {
			 State_Machine() ;
			 i = 0;
			 while (i < 50000){i++;}
			check_webserver();
	}

}
