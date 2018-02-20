#include <LPC17xx.h>
#include "uart.h"
#include <string.h>

unsigned int baudrate = 19200;	   //Velocidad de transmision asincrona en bits/seg
char tipo_rx = 0;				 //Flag que indica si se espera recibir un caracter solo o una cadena


//Variables para UART0
char buffer_rx[UART_BUFFER_RX_WORDS][UART_BUFFER_RX];	        // Buffer de recepción
char buffer_tx[UART_BUFFER_TX];                   		// Buffer de envio de m1*m2 caracteres
int pos_cadena = 0;                    //Posicion de la ultima cadena enviada + 1, Posicion donde de escribira la siguiente
char *p_buffrx = buffer_rx[0];			           // Puntero de recepción

char *pcad_tx = buffer_tx;			 // Puntero de transmisión
char tx_completa = 1;	    	   // Flag de transmisión que es 1 cuando se puede transmitir

uint8_t uart_f;			// Flag para monitorizar la comunicaion
						// uart_f

uint8_t n_words_buffer = 0;


void UART0_IRQHandler(void) {
	
   switch(LPC_UART0->IIR&0x0E) {     //Cuando hay una interrupcion UART0
									 //Hace un AND entre los flags de interrupciones y el numero en binario 1111
									 //Para ver si alguno esta activado y por tanto atender a esa interrucion
									 //RECEPCION

   case 0x04:				         //Si se ha recibido un dato en el buffer 
   /* Vamos a guardar la palabra recibida en un array de cadenas, siendo cada cadena del array,
   	  una palabra del texto recibido */

		*p_buffrx = LPC_UART0->RBR; 	 //Guardamos el caracter transmitido en la posicion que apunta p_buffrx

		if(*p_buffrx == ' '){			 //Si el caracter recibido es un espacio (32), indicando el final de una palabra
		                             //y por tanto el final de la cadena que estamos guardando
           *p_buffrx = 0; 			 //Sobreescrimos el espacio " " con un 0, asi se guardara la palabra como cadena
		   	n_words_buffer++;		  /* Tendremos que mejorarlo para que compruebe que sea espacio unico */
			p_buffrx = buffer_rx[n_words_buffer]; 
		}							 // y empezamos una nueva cadena
		 
	    else if((*p_buffrx == 0xD)||(*p_buffrx == 0xA)){    //Si el caracter recibido es [ENTER] (ya esta guardado en buffer)
		   	if((*(p_buffrx-1) == 0xD)||(*(p_buffrx-1) == 0xA)){
			   *(p_buffrx - 1) = 0;		     //Sobreescribimos el [ENTER] con un cero indicando el fin de la cadena
			   							 // El caracter ENTER son 2: CR -> 13(0xD) y LF -> 10(0xA)
				rx_cadena_UART0();		/* LLamamamos a la funcion*/
			    p_buffrx = buffer_rx[0];        //Recolocamos el puntero al principio del buffer
				n_words_buffer = 0;
			}
			else {
				p_buffrx++; 
			}
		}
	    else{                            //Si no se ha acabado la cadena
	    	p_buffrx++; 			     //Aumentamos en 1 el puntero para que el siguiente caracter se guarde
		}							 //en la siguiente posicion
	break;
	

										//TRANSMISION\\ (ya iniciada por la funcion tx_cadena_UART0 )
   case 0x02:					       	//Si el registro de transmision esta vacio por lo que se ha enviado un dato
   	 pcad_tx++;					    //Se incrementa el puntero de la cadena para que apunte al siguiente dato a transmitor
     if((*pcad_tx)!=0){			   	    //Si la cadena a enviar no ha acabado por lo que hay mas datos por transmitir
       LPC_UART0->THR=(*pcad_tx);	    //Se carga el nuevo dato (caracter) en el registro de transmision,
	                                    //cuando se transmita generara otra interrupcion del mismo tipo al haberlo enviado
		}								//y esta funcion seguira asi hasta que sea el fin de la cadena
	   			
	   else {							 //Si se llega al final de una cadena (un 0 )
	    pcad_tx++;                        //Incrementamos al siguiente punto
		if((*pcad_tx)!=0){				  //Si el siguiente caracter no es 0, significa que hay mas cadenas por enviar
                                          //pcad_tx ya apunta al principio de la siguiente cadena
		LPC_UART0->THR=(*pcad_tx); }       //Cargamos el nuevo dato para que continue el ciclo de interrupciones

		else {							  //Si tambien es 0 incica segun nuestro protocolo el final de las cadenas
		                                  //Por lo que todas las cadenas en el buffer han sido enviadas
		tx_completa=1;				   	  //Indicamos que no se esta transmitiendo nada
	    pcad_tx = buffer_tx;   		  // Ponemos el puntero al principio del bufer de salida	
		pos_cadena = 0;	}				  //Inicializamos posiciones para que las nuevas cadenas
		}                     			  //Sobreescriban a las anteriores							   
				
     break;								
   };                                      //Fin del Switch
 } 




