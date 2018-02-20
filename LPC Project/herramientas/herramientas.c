#include <LPC17xx.h>
#include <string.h>
// #include "../UART/uart.h"

	
  void conversor_IaS(int numero, char * cadnum){			  //Convierte entero a cadena y la guarda en cadnum
	       int i = 0, j = 0;
		   char aux[10];							 
		   char convertir = 1;
		   while(convertir){					   //Mientras quede cifra por convertir
		      aux[i] = numero % 10 + 48;			   //Igualamos la posicion i del array a la unidad mas pequeña de la cifra
		      numero = numero / 10;				   //Dividimos por 10 la cifra
		      i++;									   //Aumentamos la posicion donde gruardarmos la proxima cifra
		      if (numero == 0 ){					   //Si ya no hay mas numero por corvertir se indica pasa salir
		      convertir = 0; }}
		   
		   for (j=0; j<i ;j++){						//Copiamos en la cadena dada, la obtenida dada la vuelta
		       *(cadnum+j) = aux[i-j-1];}			
			  *(cadnum+i) = 0;                       //Ponemos un 0 en la ultima como fin de cadena               	
		 }

   int conversor_SaI(char * cadnum){			  //Convierte cadena a entero, recibe cadena y puntero a entero
	       int i = 0, numero=0;
		   int aux = numero;                    //Auxiliar por si acaso no podemos traducir el valor
		   int n = strlen(cadnum);									//Cifras que tiene el caracter
		   int mul = 1;									//Multiplicador de la cifra
		   numero = 0;						  	  //Igualamos a 0 el numero que nos dan por si acaso tenia otro valor
		   for(i=0; i < n ; i++) {
		      if((*(cadnum +(n-1)-i) >='0' )&&(*(cadnum +(n-1)-i) <= '9')){  //Si el caracter va del 0 al 9
		         numero += (*(cadnum +(n-1)-i)-48)*mul;			//Le sumamos al numero esa cifra elevada a lo que toke		 
		       	 mul *= 10;   }                                      //Recordar que 48 es el caracter 0
		  	  else { 										//Si algun graciosete nos da un caracter no valido
			    numero = aux;                               // Le devolvemos al int dado su valor inicial

		   }

			 }
		 return numero;
		}

   uint8_t conversor_HexaS(uint8_t numero, char * cadnum){			  //Convierte entero a cadena y la guarda en cadnum
	       uint8_t i = 0, j = 0;
		   char aux[10];							 

		   for (i = 0; i < 2; i++) {
			    aux[i] = (numero >> 4*i) & 0xF;
			   if (( aux[i] <=9)) {		// Si esta entre 0 y 9
			   		 aux[i] += '0';
				}
				else {
					 aux[i] += 'A' - 10;
				}

		   }
		   for (j=0; j<i ;j++){						//Copiamos en la cadena dada, la obtenida dada la vuelta
		       *(cadnum+j+2) = aux[i-j-1];
			 }			
			  *(cadnum+i+2) = ',';                       //Ponemos un 0 en la ultima como fin de cadena 
			   *(cadnum+i+1+2) = 0;                       //Ponemos un x en la ultima como fin de cadena               	
		 	  *(cadnum+0) = '0';
			   *(cadnum+1) = 'x';
		 }
   



