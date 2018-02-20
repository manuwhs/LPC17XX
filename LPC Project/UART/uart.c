/* uart.c
 * contiene las funciones:

 1  UART0_IRQHandler(void)
 2  tx_cadena_UART0(char *ptr)
 3  uart0_set_baudrate(unsigned int baudrate) 
 4  uart0_init(int baudrate) 
  
 */
#include <LPC17xx.h>
#include "uart.h"
#include "../OV7670/ov7670.h"
#include "../GAMES/games.h"
#include <string.h>
#include "../Timer/timer.h"    
#include "../GLCD/glcd.h"  
#include "../Touch_Pannel/Touchpanel.h"   
#include "../DAC/dac.h"
/******************************************************************************
* Function Name  : Init_UART0
* Description    : Configures the UART0 with the given BaudRate.
				   Its configured as 8 bits/dato, sin pariad, y 1 bit de stop
* Input          : - baudrate: Bits per second of the UART0
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Init_UART0(int baudrate) {
    
    LPC_PINCON->PINSEL0 |= (1 << 4);        // Configuracion del P0.2 como RX0
   	LPC_PINCON->PINSEL0 |= (1 << 6);        // Configuracion del P0.3 como TX0
    LPC_UART0->LCR |= CHAR_8_BIT|STOP_1_BIT|PARITY_NONE;   // Tipo de transmision (8 bits/dato, sin pariad, y 1 bit de stop)

    uart0_set_baudrate(baudrate);                  // Configura Velocidad transmision
    
    LPC_UART0->IER |= THRE_IRQ_ENABLE;      // Habilita la interrupcion para TX
	LPC_UART0->IER |= RBR_IRQ_ENABLE; 		// Habilita la interrupcion para RX 
	NVIC_SetPriority(UART0_IRQn, 0x05);
    NVIC_EnableIRQ(UART0_IRQn);             // Habilita la interrupcion UART0(for Cortex-CM3 NVIC)
   
}


// Función para enviar una cadena de texto
// El argumento de entrada es la dirección de la cadena, o
// directamente la cadena de texto entre comillas
							     // activar flag interrupción por registro transmisor vacio

/******************************************************************************
* Function Name  : uart0_set_baudrate
* Description    : Sets the UART0 baudrate.
* Input          : - baudrate: Bits per second of the UART0
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/

int uart0_set_baudrate(unsigned int baudrate) {
    int errorStatus = -1; //< Failure

    // UART clock (FCCO / PCLK_UART0)
   // unsigned int uClk = SystemFrequency / 4;
    unsigned int uClk =SystemCoreClock/4;
    unsigned int calcBaudrate = 0;
    unsigned int temp = 0;

    unsigned int mulFracDiv, dividerAddFracDiv;
    unsigned int divider = 0;
    unsigned int mulFracDivOptimal = 1;
    unsigned int dividerAddOptimal = 0;
    unsigned int dividerOptimal = 0;

    unsigned int relativeError = 0;
    unsigned int relativeOptimalError = 100000;

    uClk = uClk >> 4; /* div by 16 */

    /*
     *  The formula is :
     * BaudRate= uClk * (mulFracDiv/(mulFracDiv+dividerAddFracDiv) / (16 * DLL)
     *
     * The value of mulFracDiv and dividerAddFracDiv should comply to the following expressions:
     * 0 < mulFracDiv <= 15, 0 <= dividerAddFracDiv <= 15
     */
    for (mulFracDiv = 1; mulFracDiv <= 15; mulFracDiv++) {
        for (dividerAddFracDiv = 0; dividerAddFracDiv <= 15; dividerAddFracDiv++) {
            temp = (mulFracDiv * uClk) / (mulFracDiv + dividerAddFracDiv);

            divider = temp / baudrate;
            if ((temp % baudrate) > (baudrate / 2))
                divider++;

            if (divider > 2 && divider < 65536) {
                calcBaudrate = temp / divider;

                if (calcBaudrate <= baudrate) {
                    relativeError = baudrate - calcBaudrate;
                } else {
                    relativeError = calcBaudrate - baudrate;
                }

                if (relativeError < relativeOptimalError) {
                    mulFracDivOptimal = mulFracDiv;
                    dividerAddOptimal = dividerAddFracDiv;
                    dividerOptimal = divider;
                    relativeOptimalError = relativeError;
                    if (relativeError == 0)
                        break;
                }
            }
        }

        if (relativeError == 0)
            break;
    }

    if (relativeOptimalError < ((baudrate * UART_ACCEPTED_BAUDRATE_ERROR) / 100)) {

        LPC_UART0->LCR |= DLAB_ENABLE; 	// importante poner a 1
        LPC_UART0->DLM = (unsigned char) ((dividerOptimal >> 8) & 0xFF);
        LPC_UART0->DLL = (unsigned char) dividerOptimal;
        LPC_UART0->LCR &= ~DLAB_ENABLE;	// importante poner a 0

        LPC_UART0->FDR = ((mulFracDivOptimal << 4) & 0xF0) | (dividerAddOptimal & 0x0F);

        errorStatus = 0; //< Success
    }

    return errorStatus;
}
 /******************************************************************************
* Function Name  : tx_cadena_UART0
* Description    : Sends the String given as input through the UART0.
				   When its called, this function doesnt directly send the string and waits for
				   it to be complete. It just saves it in a buffer and the UART0 interrupt handler
				   will send it char by char. When all the strings in the buffer are sent, empties it.
				   The buffer has an overflow protection, if the String is not going to fit, it waits
				   untill all the buffer is sent, so it gets empty and can start in the 0 position.
* Input          : - cadena: String to be sent through UART0
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/					   					  
 void tx_cadena_UART0(char *cadena){		  //Envia cadena por UART0
	  int i=0;
	  if (pos_cadena + strlen(cadena) >= UART_BUFFER_TX - 50) {	   // Si va a haber un overflow
	  		while (pos_cadena != 0);
			 	tx_cadena_UART0("\n\r");
			// tx_cadena_UART0("\n\r ************* PARON *************\n\r");	
	  }
	  for(i=0;i<=strlen(cadena);i++){ // Por cada caracter que se quiere enviar
	  	*(buffer_tx + pos_cadena + i)= cadena[i];	   //Se guarda la cadena en la posicion dada por ppos_cadena para que no se sobreescriban
	  }	  
	  *(buffer_tx + pos_cadena + i) = 0;			   //Finalizamos cadena con un 0 para asegurarnos
	  *(buffer_tx + pos_cadena + i + 1)=0;		   //Ponemos un segundo 0 para nuestro protocolo
	  pos_cadena += strlen(cadena) + 1; 		//Indicamos que la primera posicion libre esta despues del 0 final de
												//la ultima cadena escrita, asi como indica el protocolo, con la siguiente cadena
												//se sobreescribira el segundo 0 puesto por la ultima cadena
	  if(tx_completa==1){	     //Si todas las cadenas del buffer se han enviado habra que empezar el envio
	  LPC_UART0->THR = buffer_tx[0];}	// Pone el primer dato en el buffer de tranmision TX para que cuando se transmita
	                                 // segenere la interrupcion de dato transmitido y la atencion a la interrupcion
									 // se ocupara de enviar el resto de la cadena
	  tx_completa = 0;			 // Indicamos que se estan transmitiendo datos

//Protocolo:    cadena1..
//				cadena1.cadena2..
//              cadena1.cadena2.cadena3..
// Cuando se envien todas las cadenas pendientes, el puntero volvera a apuntar al principio del buffer
// Sobreescribiendo asi las cadenas ya enviadas

}

/******************************************************************************
* Function Name  : rx_cadena_UART0
* Description    : This function evaluates the information recieved by the UART0 and acts accordingly.
				   In this implementation, the words that are being received are stored as elements of an
				   String array (like ARGV[]). When we recieve an [ENTER], we call this funcion which 
				   evaluates them and empties the String array
* Input          : - buffer_rx: String array with the words recieved before the [ENTER]
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/

 void rx_cadena_UART0(void){		  //Recivir cadena por UART0   (como la consola jejeje)
	int i;

	if (strcmp(buffer_rx[0],"help") == 0){
		tx_cadena_UART0("COMMAND LIST: \n");
		tx_cadena_UART0("- setOV7670 [Reg] [Value] \n");
		tx_cadena_UART0(" Sets up the register [Reg] with the value [Value]\n");

		tx_cadena_UART0("- readOV7670 [Reg] \n");
		tx_cadena_UART0(" Reads the value of the register [Reg]\n");

		tx_cadena_UART0("- detectMode [Mode] \n");
		tx_cadena_UART0(" Sets the algorithm used to detect the object:\n");
	    tx_cadena_UART0(" 0: No detection\n 1: Mass Center Detection \n 2: Biggest Object Block Detection");
	}

	if (strcmp(buffer_rx[0],"setOV7670") == 0){
		 tx_cadena_UART0("Configurando camara \n");
		 ov7670_set(conversor_SaI(buffer_rx[1]), conversor_SaI(buffer_rx[2]));

	}


	if (strcmp(buffer_rx[0],"umbral") == 0){
		  tx_cadena_UART0("Umbral configurado");
		 set_Umbral(conversor_SaI(buffer_rx[1]));

	}

 	if (strcmp(buffer_rx[0],"read") == 0){
		  tx_cadena_UART0("Tipo de recepcion cambiado");
		 read_type = conversor_SaI(buffer_rx[1]);

	}

	if (strcmp(buffer_rx[0],"briconlum") == 0){
		  tx_cadena_UART0("Brillo, Contraste y Luminosidad \n");
		 OV7670_BriConLum(conversor_SaI(buffer_rx[1]),conversor_SaI(buffer_rx[2]), 
		 			   conversor_SaI(buffer_rx[2]),conversor_SaI(buffer_rx[3]) );

	}

	if (strcmp(buffer_rx[0],"AWBC") == 0){
		  tx_cadena_UART0("AWBC \n");
		  AWBC_control ();
	}

	if (strcmp(buffer_rx[0],"AWBmode") == 0){
		  tx_cadena_UART0("AWBC mode \n");
		  OV7670_AWBmode (conversor_SaI(buffer_rx[1]));
	}

	if (strcmp(buffer_rx[0],"AWBgain") == 0){
		  tx_cadena_UART0("AWBC gain \n");
		  OV7670_RGBgain (conversor_SaI(buffer_rx[1]) ,
		  				conversor_SaI(buffer_rx[2]), 
		  				conversor_SaI(buffer_rx[3]));
	}


	if (strcmp(buffer_rx[0],"fps") == 0){
		  tx_cadena_UART0("fps config \n");
			OV7670_FPS (conversor_SaI(buffer_rx[1]));
	}
	if (strcmp(buffer_rx[0],"vd") == 0){
		  tx_cadena_UART0("Vd !! \n");
			Vd_OV7670 = conversor_SaI(buffer_rx[1]);
			Vx_OV7670 = conversor_SaI(buffer_rx[2]);
			// ov7670_set(REG_VSTART, 0x03 + (Vx_OV7670*2)/4);
	}	  

	if (strcmp(buffer_rx[0],"cons") == 0){
		  tx_cadena_UART0("cons!! \n");
			const_block = conversor_SaI(buffer_rx[1]);
			// ov7670_set(REG_VSTART, 0x03 + (Vx_OV7670*2)/4);
	}

	if (strcmp(buffer_rx[0],"readOV7670") == 0){
		  tx_cadena_UART0("leyendo \n");
		  ov7670_read(conversor_SaI(buffer_rx[1]));

	}

	if (strcmp(buffer_rx[0],"periodic") == 0){
		  tx_cadena_UART0("haciendo interrup periodica\n");
		  	periodic_signal2_ms(conversor_SaI(buffer_rx[1]));
	}

	if (strcmp(buffer_rx[0],"pass") == 0){
		  tx_cadena_UART0("pass\n");
	pass = (conversor_SaI(buffer_rx[1]));
	}

	if (strcmp(buffer_rx[0],"photo") == 0){
		  take_picture = conversor_SaI(buffer_rx[1]);
	}
	if (strcmp(buffer_rx[0],"pic") == 0){
		  tx_cadena_UART0("ticture\n");
		  show_picture = conversor_SaI(buffer_rx[1]);
	}
   	if (strcmp(buffer_rx[0],"CIF") == 0){
			  tx_cadena_UART0("CIF\n");
		try_QCIF_H = conversor_SaI(buffer_rx[1]);
		try_QCIF_W = conversor_SaI(buffer_rx[2]);
	}
	if (strcmp(buffer_rx[0],"resol") == 0){
			  tx_cadena_UART0("scaler\n");
			  resolution (conversor_SaI(buffer_rx[1]));
	}
	if (strcmp(buffer_rx[0],"scaler") == 0){
			  tx_cadena_UART0("resolution\n");
			  scaler (conversor_SaI(buffer_rx[1]), conversor_SaI(buffer_rx[2]));
	}
	if (strcmp(buffer_rx[0],"timer") == 0){
			  tx_cadena_UART0("timer\n");
			  timing_resol (conversor_SaI(buffer_rx[1]), conversor_SaI(buffer_rx[2]),conversor_SaI(buffer_rx[3]));
	}
	if (strcmp(buffer_rx[0],"mirror") == 0){
			  tx_cadena_UART0("mirror\n");
			  OV7670_mirror (conversor_SaI(buffer_rx[1]));

	}

	if (strcmp(buffer_rx[0],"window") == 0){
			  tx_cadena_UART0("win\n");
			  Window (conversor_SaI(buffer_rx[1]),conversor_SaI(buffer_rx[2]),
			 		  conversor_SaI(buffer_rx[3]),conversor_SaI(buffer_rx[4]));

	}
	if (strcmp(buffer_rx[0],"text") == 0){
			  tx_cadena_UART0("max\n");
			  text_mode = conversor_SaI(buffer_rx[1]);
	}


if (strcmp(buffer_rx[0],"game") == 0){
			tx_cadena_UART0("game\n");
		    newGame = 1;
			//resolution(1);
			read_type = 3;
			game =  conversor_SaI(buffer_rx[1]);
			periodic_signal2_ms(100);

	}

if (strcmp(buffer_rx[0],"sound") == 0){

	set_sound( conversor_SaI(buffer_rx[1]), conversor_SaI(buffer_rx[2]));
			tx_cadena_UART0("sonido\n");
	}
	
if (strcmp(buffer_rx[0],"DAC") == 0){

			LPC_DAC->DACR = conversor_SaI(buffer_rx[1])<< 6;
			tx_cadena_UART0("son\n");
	}
if (strcmp(buffer_rx[0],"DMA") == 0){

			LPC_GPDMA->DMACSoftBReq |= 7;
			tx_cadena_UART0("son\n");
	}
if (strcmp(buffer_rx[0],"samples") == 0){
			 tx_cadena_UART0("sample\n");
			set_samples_cycle(conversor_SaI(buffer_rx[1]));
	}
if (strcmp(buffer_rx[0],"tDAC") == 0){
			 tx_cadena_UART0("tDAC\n");
			LPC_DAC->DACCNTVAL =conversor_SaI(buffer_rx[1]);
	}	
			
 /*
	if (strcmp(buffer_rx[0],"motor+") == 0){
			  tx_cadena_UART0("resolution\n");
			  cons_x += conversor_SaI(buffer_rx[1]);
			  cons_y +=	 conversor_SaI(buffer_rx[2]);
			  cons_dif_x += conversor_SaI(buffer_rx[3]);
			  cons_dif_y += conversor_SaI(buffer_rx[4]);
	}
	if (strcmp(buffer_rx[0],"motor-") == 0){
			  tx_cadena_UART0("resolution\n");
			  cons_x -= conversor_SaI(buffer_rx[1]);
			  cons_y -=	 conversor_SaI(buffer_rx[2]);
			  cons_dif_x -= conversor_SaI(buffer_rx[3]);
			  cons_dif_y -= conversor_SaI(buffer_rx[4]);
	}
*/	

}



