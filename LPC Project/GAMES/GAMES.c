
#include "LPC17xx.h"
#include "../OV7670/I2C.h"
#include "../OV7670/ov7670.h"
#include "../OV7670/ov7670reg.h"
#include "../GLCD/GLCD.h"
#include <string.h>
#include <math.h>
#include "../TIMER/timer.h" 
#include "../UART/uart.h" 
#include "../Touch_Pannel/TouchPanel.h"
#include "../DMA/dma.h"
#include "../DAC/dac.h"
#include "games.h"
#include "../State_Machine/states.h"
uint16_t rand_seed = 1;
uint32_t rand_number (void) {
	   uint32_t random_number = LPC_GPIO0->FIOPIN >> 10;
	   random_number += *(&caramels[0][0]+rand_seed)+ rand_seed +rand_seed*rand_seed;
	   rand_seed++;
	   return random_number;
}


// Tenemos que poner el bucle con "s" porque al hacerlo directamente con s peta */
void conversor_IaS(int numero, char * cadnum);

//*******************  GLOBAL GAMES VARIABLES *******************

Rectangle pl1, pl2, ball;	// Rectangles asociated with the players
uint8_t game = 0;			 // To select the game we are playing 

uint8_t dir_ball = 0;

uint8_t vel_x_base = 3;		// Reference velocities for the game
uint8_t vel_y_base = 6;
int vel_x = 3;			   // Actual velocity of the ball after computing it
int vel_y = 0;
int desp_x;				   // Displacement in pixels of the object
int desp_y;

uint16_t  color_ball = 0xFFFF;
uint16_t bgColor = 0x2222;
uint8_t newGame = 1;			/* Flag that tells us if its a new game */
uint8_t border_field = 10;
uint16_t time_game = 0;
uint32_t num_times_game = 0;   // Number of iterations of the game so far

//*******************  PONG GAME VARIABLES *******************

uint8_t score1 = 0, score2 = 0;
uint8_t time_pong = 0;


//*******************  SPACE INVADERS GAME VARIABLES *******************
Rectangle misil[10], invader;
uint16_t invaders_x = 320 -(1+20);	// These are the coordinates where the imaginary array of invaders starts
uint16_t invaders_y	= 20;			// To move the invaders we move this array

int vel_invaders = 5;
uint16_t lowest_invader = 100;     // X position of the lowest invader for optimization

uint8_t num_destroyed = 0;		// Num of destroyed invaders 
uint32_t destroyed = 0;		// Binary array that tells us which invaders are destroyed 

uint8_t n_invaderRows = 6;			 //Number of Rows of invaders
uint8_t invadersPerRow = 5;			 //Number of Invaders per Row

uint16_t misil_active = 0;
uint8_t lives = 3;

const uint16_t figures[8][8] = {
{0x03C0,0x1FF8,0x3FFC,0x399C,0x3FFC,0x0660,0x0DB0,0x300C},/* Malo 1 A */
{0x03C0, 0x1FF8, 0x3FFC, 0x399C, 0x3FFC,0x0E70, 0x1998, 0x0C30}, /* Malo 1 B */

{0x0820,0x2448,0x2FE8,0x3BB8,0x3FF8,0x1FF0,0x0820,0x1010},	/* Malo 2 A */
{0x0820,0x0440,0x0FE0,0x1BB0,0x3FF8,0x2FE8,0x2828,0x06C0},/* Malo 2 B */

{0x0180,0x03C0,0x07E0,0x0DB0,0x0FF0,0x05A0,0x0810,0x0420}, /* Malo 3 A */
{0x0180,0x03C0,0x07E0,0x0DB0,0x0FF0,0x0240,0x05A0,0x0A50},	 /* Malo 3 B*/

{0x0000,0x07E0,0x1FF8,0x3FFC,0x6DB6,0xFFFF,0x398E,0x1004},/* Supermalo */
{0x0180,0x03C0,0x07E0,0x0660,0x07E0,0x0FF0,0x0BD0,0x0810}  /* Nave */

};
//*******************  CANDY CRUSH GAME VARIABLES *******************

uint8_t candy_array[CANDY_COL][CANDY_ROW];
int candy_desp[CANDY_COL][CANDY_ROW]; 

 

// candy_array[Selecciona Columna (Eje X)][Selecciona Fila (Eje Y)]
/* We have a total of 7x7 = 49 candy array and 6 different types (so far), 
We will enconde each element of the array in 4 bits, we create an array of bits

	X X X   X   X X X X
	-----	- 	-------
	7:5 -> Number of candy gaps below this candy
	4:	-> Inicates whether that candy has been eliminated or not
	3:0	-> Kind of candy of that (i,j)
Haremos lo siguiente, el bit [5] es ELIMINADO y el bit [6] es CAYENDO.
Un caramelo debe estar cayendo si el que tiene abajo lo esta tambien o esta eliminado.
Un caramelo CAYENDO pasa a estar quieto cuando el de abajo tambien esta quieto.
En el caso del que cae porque el de abajo esta ELIMINADO, cuando el eje_Y del que este cayendo
sea igual o menor al del eliminado, entonces, el valor del ELIMINADO pasa a ser el de que esta cayendo.
Lo mismo pasa cuando uno cayendo cae sobre otro.
 */

uint8_t new_candy_array[CANDY_COL][CANDY_ROW]; // Same as the others but for the new generated candy
uint8_t new_candy_desp[CANDY_COL][CANDY_ROW];  
// When we detect we have to delete candy we initialice this variables from the [0][0] position
// and falling, and when they get to the bottom then we

 uint16_t ts_x = 0;	   // X-axis value touched
 uint16_t ts_y = 0;
 uint8_t candy_state = CANDY_WAITING; // State of the candy game
 uint8_t sel_candyx = 0;			   // Selected candy x value
 uint8_t sel_candyy = 0;
 uint8_t candy_swapx;
 uint8_t candy_swapy;
 int candy_sel_dep,	candy_swap_desp;

 uint8_t candy_st;			 // Tipo de swap que estamos haciendo bit [0] indica horitontal o vertical
 							 //                                   bit [1] indica hacia un lado u otro
 int difc_x;					// Diference between selected candy and the other selected
 int difc_y;
 int candy_score = 0;
 int highest_candy_score = 0;

 uint8_t candy_acc= 0; 		// Acceleración del candy


 // --------------------------------------------------------------------------------------
 // |*************************************************************************************|		
 // |******       ******************** PONG GAME ***************************       *******|	 
 // |*************************************************************************************|
 //	---------------------------------------------------------------------------------------

 /******************************************************************************
* Function Name  : PONG
* Description    : This function creates the PONG game. It has to be called periodicly to 
				   refresh the game. When its called:
				   	- Erases ball and bars.
					- Gets the new position of them.
					- Checks for collisions and acts accordingly
					- Redraws Ball and Bars
* Output         : None
* Return         : None
* Attention		 : This funcion needs to be called periodicly with a TIMER.
				   The more times we call it per second, the faster everything
				   will go. We have to find a comprise between precision and velocity.
*******************************************************************************/	 
  void PONG(void){

   	if (newGame == 1){
		init_PONG();
		newGame = 0;
		
	}
	/* Draw Pseudo- Static elements  */
	conversor_IaS(score1, delay_read);
	GUI_Text(160-20, 20,delay_read,0, 0xFFFF);
	conversor_IaS(score2, delay_read);
	GUI_Text(160+20, 20,delay_read,0, 0xFFFF);	
    LCD_DrawLine(160,0,160,239,	0);	

	/* Destroy bars and ball */
	DrawRectangle(pl1.x, pl1.y, pl1.w, pl1.h, bgColor);
	DrawRectangle(pl2.x, pl2.y, pl2.w, pl2.h, bgColor);
	DrawRectangle(ball.x, ball.y, ball.w, ball.h, bgColor);	
		
	
	/* GET THE DESPLACEMENT OF BARS AND BALL ****************************************************/
	desp_x = (int)(vel_x *(1.0+ ((float)time_pong/10)));
	desp_y = (int)(vel_y *(1.0+ ((float)time_pong/10)));

	/* CHECK THAT THE NEW POSITION OF BALL IS INSIDE SCREEN ************************************************/
	if (ball.y + desp_y < 0){
		ball.y = 0;
	}
	else {
		ball.y += desp_y;
	}

	if (ball.x + desp_x < 0){
		ball.x = 0;
	}
	else {
		ball.x += desp_x;
	}


	/* CHECK THAT THE BARS POSITION ARE INSIDE SCREEN  ************************************************/
 	if (player1_y - ((pl1.h/2)+1) > 0) {
		pl1.y = player1_y - ((pl1.h/2)+1);	
	}
	else {
		pl1.y = 1;	
	}
 	if (player2_y - ((pl2.h/2)+1) > 0) {
		pl2.y = player2_y - ((pl2.h/2)+1);	
	}
	else {
		pl2.y = 1;	
	}		

	/* CHECK COLLISION WITH BARS AND BALL ************************************************/
	if (hitTest(ball,pl2) != 0) {
		vel_x = -vel_x_base;
		vel_y =((ball.y + ball.h/2)- (pl2.y + pl2.h/2))/vel_y_base;
		DMA_DAC_audio_Channel_Init( 0, muelle,2768);
	}
	if (hitTest(ball,pl1) != 0) {
		
		vel_x = vel_x_base;
		vel_y =((ball.y + ball.h/2)- (pl1.y + pl1.h/2))/vel_y_base;
		DMA_DAC_audio_Channel_Init( 0, muelle,2768);
	}

	 /* FIELD BOUNDARIES **************************************************************/
	if (ball.x < border_field) {			   /* P2 scored */
		 	vel_x = vel_x_base;
			vel_y = 0;
			ball.x = 160 - ball.w/2;
			ball.y = 120 - ball.h/2;
			score2++;
			time_pong = 0;	
	}
	else if  (ball.x > MAX_X -( 1 + ball.w + border_field)) {			   /* P1 scored */
			vel_x = -vel_x_base;
			vel_y = 0;
			ball.x = 160 - ball.w/2;
			ball.y = 120 - ball.h/2;
			score1++;	
			time_pong = 0;
	}

	if (ball.y < border_field) {			   /* Upper roof */
		   vel_y = abs(vel_y);
		   ball.y += border_field - ball.y ;
	}
	else if  (ball.y > MAX_Y - (1 + ball.h + border_field)) {			   /*  Lower roof*/
		   vel_y = -1 * abs(vel_y);
		   ball.y -= ball.y-(MAX_Y - (1 + ball.h + border_field));
	}


  /* DRAW BARS AND BALL **************************************************************/

	DrawRectangle(pl1.x, pl1.y, pl1.w, pl1.h, 0);
	DrawRectangle(pl2.x, pl2.y, pl2.w, pl2.h, 0);
	DrawRectangle(ball.x, ball.y, ball.w, ball.h, color_ball);
 }
  /******************************************************************************
* Function Name  : init_PONG
* Description    : Initializes the PONG game variables. 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/

void init_PONG(void){
 			// resolution(1);
			bgColor = 0x2222;
			LCD_Clear(bgColor);
			time_pong = 0;
			time_game = 0;
			score1 = 0;
			score2 = 0;

			ball.w = 12;
			ball.h = 12;
			ball.x = 160 - ball.w/2;
			ball.y = 120 - ball.h/2;

		  	pl1.w = 15;
			pl1.h = 60;
			pl1.x = 40;
	
			pl2.w = 15;
			pl2.h = 60;
			pl2.x = 270;
}
 
  // --------------------------------------------------------------------------------------
 // |*************************************************************************************|		
 // |******       ******************** SPACE INVADERS **********************       *******|	 
 // |*************************************************************************************|
 //	---------------------------------------------------------------------------------------
 
 /******************************************************************************
* Function Name  : space_invaders
* Description    : This function creates the Space Invaders game. It has to be called periodicly to 
				   refresh the game. When its called:
				   	- Erases player and misils.
					- Gets the new position of them.
					- Checks for collisions and acts accordingly
					- Redraws player and misils.
					- Periodicly it moves the invaders (Erase them and redraw them all).
* Output         : None
* Return         : None
* Attention		 : This funcion needs to be called periodicly with a TIMER.
				   The more times we call it per second, the faster everything
				   will go. We have to find a comprise between precision and velocity.
*******************************************************************************/
void space_invaders(void){

    int i,j;

	if (newGame == 1){
		init_space();
		newGame = 0;
		
	}

	/******************** Marcianitos ***********************/
	Window(0,0,319,239);

	conversor_IaS(num_destroyed, delay_read);
	GUI_Text(160-20, 20,delay_read,0, 0xFFFF);

   	conversor_IaS(lives, delay_read);
	GUI_Text(160+20, 20,delay_read,0, 0xFFFF);

	/* Destroy player and misils */
	DrawRectangle(40 +1 - (invader.w), pl1.y, invader.w, invader.h, bgColor);
	DrawRectangle(ball.x, ball.y, ball.w, ball.h, bgColor);	
		for (i=0; i < 10; i++) {
			if ((misil_active & (1 << i))) {
				DrawRectangle(misil[i].x,misil[i].y,misil[i].w,misil[i].h,bgColor);	
			}	
	 }

	/* GET THE NEXT POSITION OF MISILS AND BALL ****************************************************/
	desp_x = (int)(vel_x *(1.0+ ((float)time_game/10)));


	if (ball.x + desp_x < 0){
		ball.x = 0;
	}
	else {
		ball.x += desp_x;
	}
	
	//GUI_Text(40, player1_y,"X1",0, 0xFFFF);	

 	if (player1_y - ((invader.h/2)+1) > 0) {
		pl1.y = player1_y - ((invader.h/2)+1);	
	}
	else {
		pl1.y= 1;	
	}

 	if (player1_y - ((invader.h)+1) > MAX_Y - 1) {
		pl1.y= MAX_Y - (1+invader.h+1);	
	}	


   //	GUI_Text(40, pl1.y,"X2",0, 0xFFFF);	


	/* CHECK MISILS ****************************************************/
	for (i=0; i < 10; i++) {
			if ((misil_active & (1 << i))) {
				
				misil[i].x -= 2;

				if (misil[i].x < 10) {
					DrawRectangle(misil[i].x,misil[i].y,misil[i].w,misil[i].h,bgColor);	
					misil_active &= ~(1 << i); 
				}

				if (hitTest(misil[i],pl1) != 0) {
					DrawRectangle(misil[i].x,misil[i].y,misil[i].w,misil[i].h,bgColor);
					if (lives == 0) {
					    game = 0;
				   		pass =0;
						game = 0; 
						periodic_signal2_ms(0); 
				   		state=23;draw_flag = 1;screen_flag=0;
					}
					lives--; 
					misil_active = 0;		 // Remove all misils

								 // Reestart ball
				   	 	vel_x = vel_x_base;
						ball.x = pl1.x + pl1.w;
						ball.y = pl1.y + invader.h/2  - ball.h/2;
						time_pong = 0;


				}

			 	if (hitTest(misil[i],ball) != 0) {
					DrawRectangle(misil[i].x,misil[i].y,misil[i].w,misil[i].h,bgColor);
					misil_active &= ~(1 << i);	

	 				  	vel_x = vel_x_base;
						ball.x = pl1.x + pl1.w;
						ball.y = pl1.y + invader.h/2  - ball.h/2;
						time_pong = 0;


				}

			 }
		
	}
	
	/* CHECK BALL-INVADER COLLISION ****************************************************/
	if (ball.x >= lowest_invader) {		  // If there may be a collision
		Window(0,0,319,239);
 	if  (ball.x > MAX_X -( 1 + ball.w + border_field)){
				  		vel_x = vel_x_base;
						ball.x = pl1.x + pl1.w;
						ball.y = pl1.y + invader.h/2  - ball.h/2;
						time_pong = 0;
	}
	for (i=0; i < n_invaderRows*invadersPerRow; i++) {
			if ((destroyed & (1 << i))==0) {
				invader.x = invaders_x - ((i/invadersPerRow)*20 + invader.w);  // Al escribir en modo vertical el (0,0) de
													// la imagen es arriba a la derecha.
				invader.y = invaders_y + (i%invadersPerRow)*36;
	
				if (hitTest(ball,invader) != 0) {
					DrawRectangle(invader.x,invader.y,invader.w +1,invader.h,bgColor);
					num_destroyed += 1;
					destroyed |= 1 << i;

				  		vel_x = vel_x_base;
						ball.x = pl1.x + pl1.w;
						ball.y = pl1.y + invader.h/2  - ball.h/2;
						time_pong = 0;

					break;
				}
			 }
		
	}
   }
 
   /* DRAW bars and ball and misils **************************************************************/
	text_mode = 1;
	PutFigure(40, pl1.y, 7, 0xFFFF, bgColor,0);
	text_mode = 0;
	DrawRectangle(ball.x, ball.y, ball.w, ball.h, color_ball);
    for (i=0; i < 10; i++) {
			if ((misil_active & (1 << i))) {
					DrawRectangle(misil[i].x,misil[i].y,misil[i].w,misil[i].h,0x0);	
			 }
	}

	   /* DRAW INVADERS **************************************************************/
   			if (move_inv_flag == 1)  {
					move_inv_flag = 0;
					DrawInvaders();

			if (time_game%4 == 0)  {	  // Throw misils
				for (i = 0; i<10; i++) {   // Buscamos un hueco para el nuevo misil

				// Poner una variable que indique que no mas !!!
					if ((misil_active & (1 << i))==0) {
					   misil_active |= 1 << i;
					   misil[i].x = 220;
					   misil[i].y = 10 + rand_number()%300;
					   break;
				}	}		
		   	  }
			  /*
		   if (time_game%30 == 0)  {	// Add the motherfucka
			   text_mode = 1;
			   PutFigure(300, 120, 6, 0xFFFF, bgColor,0);
			   text_mode = 0;
		   }
				*/
	  		}
}

 /******************************************************************************
* Function Name  : DrawInvaders
* Description    : This function moves the invaders (Erase them and redraw them).  
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/

void DrawInvaders(void){
	int i,j;
	Window(0,0,319,239);
	// Destroy all invaders
	DrawRectangle(invaders_x - (n_invaderRows*20),invaders_y ,n_invaderRows*20 +1,invadersPerRow*36,bgColor);
 	text_mode = 1;

	if (invaders_y > MAX_Y - (invadersPerRow*36 +10)) {
		vel_invaders = -5;
		invaders_x -= 5;
	}
	if (invaders_y < 10) {
		vel_invaders = 5;
		invaders_x -= 5;
	}

   invaders_y += vel_invaders;
   Window(0,0,319,239);
	for (i=0; i < n_invaderRows; i++) {
		for (j = 0; j < invadersPerRow; j++) {
			if ((destroyed & (1 << (invadersPerRow*i + j)))==0) {
				// Draw every invader not destroyed and "make it move" with "time_game%2"
				PutFigure(invaders_x - i*20, invaders_y + j*36, (i%3)*2 + time_game%2, 0xFFFF, bgColor,0);
			}
		 }
	}
 	text_mode = 0;
}



/******************************************************************************
* Function Name  : init_space
* Description    : Initializes the Space Invaders game variables. 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void init_space(void){
			uint8_t i;

		//	resolution(1);
			bgColor = 0x2222;
			LCD_Clear(bgColor);
			lives = 3;
			time_pong = 0;
			time_game = 0;
			misil_active = 0;
			num_destroyed = 0;
			destroyed = 0;
 			invaders_x = 320 -(1+20);	
			invaders_y	= 20;
			ball.w = 10;
			ball.h = 6;
			vel_x = vel_x_base;
			ball.x = pl1.x + pl1.w;
			ball.y = pl1.y + invader.h/2  - ball.h/2;

			invader.w = 16 ;	   /* El +1 es debido a que la funcion del cuadrado no rellena el borde */
			invader.h = 32 ;	   // Poner estas dos lineas en la inicializacion del juego

  			pl1.w = invader.w;	   /* El +1 es debido a que la funcion del cuadrado no rellena el borde */
			pl1.h = invader.h;	   // Poner estas dos lineas en la inicializacion del juego
			pl1.x = 40 - pl1.w;

		 	for (i = 0; i<10; i++) {
					misil[i].x = 0;
					misil[i].y = 0;
					misil[i].w = 10;
					misil[i].h = 6;
			}

}



/******************************************************************************
* Function Name  : hitTest
* Description    : Checks if the two given rectangles are colliding
* Input          : - r1: First rectangle (Rectangle structure)
*				   - r2: Second  rectangle (Rectangle structure)
* Output         : None
* Return         : None
* Attention		 : What this function really does is checking that 
				  none of the 4 vertex of the first rectangle is inside the other.
				  So it only work for all cases if the firt rectangle is smaller
				  than the second one but is twice as fast as checking both rectangles
				  so we alwais put the smaller rectangle a r1. 
*******************************************************************************/

	uint8_t hitTest (Rectangle r1, Rectangle r2) {
	
		uint8_t state = 0 , i = 0;
		uint16_t vertex_x, vertex_y;	   

		 while (i <4 ){			  // Checks if any vertex of r1 is inside r2 
			 vertex_x = r1.x + (i%2)*r1.w;
			 vertex_y = r1.y + ((i+1)/3 )*r1.h;
	
			 if ((vertex_x >= r2.x )&&(vertex_x <= r2.x + r2.w)&&
				   (vertex_y >= r2.y )&&(vertex_y <= r2.y + r2.h)) {
						 state |= 1 << i;
			 }
		i++;
	   }
	 return state;
	}

















   // --------------------------------------------------------------------------------------
 // |*************************************************************************************|		
 // |******       ******************** CANDY CRUSH **********************       *******|	 
 // |*************************************************************************************|
 //	---------------------------------------------------------------------------------------



void CANDY_CRUSH(void){
	int i,j;
	int aux1,aux;
	uint8_t aux_candy[8];
	uint8_t position;
    uint8_t change_finish = 0;

	num_times_game++;
	if (time_game >= 180) {
		newGame = 1;
	}
   	if (newGame == 1){
		init_CANDY();
		newGame = 0;
	}

	// SCORE

  	conversor_IaS(candy_score, delay_read);
	for (i = 0; i < 5 - strlen(delay_read);i++){
		aux_candy[i]= '0';
	}
	aux_candy[i] = 0;
 	GUI_Text(5, 20 ,"SCORE",0, 0xFFFF);
	GUI_Text(5, 42, aux_candy,0, 0xFFFF);
	GUI_Text(5 + 8*strlen(aux_candy), 42, delay_read,0, 0xFFFF);

    
	GUI_Text(5, 80 ,"TIME ",0, 0xFFFF);

  	conversor_IaS(time_game, delay_read);
	GUI_Text(5, 100,delay_read,0, 0xFFFF);

   /*
	aux1 = num_times_game*((general_period2_ms*100000)/TIMER1_FRECUENCY);
	aux = strlen(delay_read);
	GUI_Text(5, 100,delay_read,0, 0xFFFF);
	GUI_Text(5 + 8*aux, 100, ".",0, 0xFFFF);
  	conversor_IaS((aux1)%100, delay_read);
	GUI_Text(5+ 8*(aux+1), 100,delay_read,0, 0xFFFF);
	*/
	/*	 
	// DEBUG INFO
	conversor_IaS(score1, delay_read);
	GUI_Text(10, 20,delay_read,0, 0xFFFF);
	conversor_IaS(score2, delay_read);
	GUI_Text(40, 20,delay_read,0, 0xFFFF);	

	conversor_IaS(candy_state, delay_read);
	GUI_Text(10, 40,delay_read,0, 0xFFFF);

	conversor_IaS(ts_x, delay_read);
	GUI_Text(10, 60,delay_read,0, 0xFFFF);	
	conversor_IaS(ts_y, delay_read);
	GUI_Text(40, 60,delay_read,0, 0xFFFF);
   */
	//		LCD_Clear(0x00);

	// GET TOUCHSCREEN

	Ptr = Read_Ads7846();	 // Creo que esto detecta que pulsemos la pantalla

	if(Ptr!=(void*)0){		 // If detected pulse
		getDisplayPoint(&display, Ptr, &matrix ) ;	// display.x and display.y of detection

		// IF WE HAVE TOUCHED THE TABLET  IN THE CANDY ARRAY
		if ((display.x > CANDY_X)&&((display.x < CANDY_X + CANDY_ROW*CANDY_SEP))
			&&(display.y > CANDY_Y)&&((display.y < CANDY_Y + CANDY_COL*CANDY_SEP))) {

			ts_x = (display.x - CANDY_X)/CANDY_SEP;
			ts_y = (display.y - CANDY_Y)/CANDY_SEP;
   //************************************************************************************************
  //************************************* CANDY WAITING STATE **********************************
  //*************************************************************************************************
			if ((candy_state == CANDY_WAITING)) {	// IF  we have selected a candy to move
	   			sel_candyx = ts_x;
	   			sel_candyy = ts_y;

 				LPC_DAC->DACCNTVAL = 1134;					// Set emision frecuency
				DMA_DAC_audio_Channel_Init( 0, click_audio,1462);	  //Make sound bitcheeeessss !!!

			 	DrawRectangle(	CANDY_X + CANDY_SEP*sel_candyx,	 // UP
			 					CANDY_Y+ CANDY_SEP*sel_candyy,
								CANDY_SIZE,
								4,
								0xFFFF);									// DRAW TO INDICATE THE SELECTED CANDY

			 	DrawRectangle(	CANDY_X + CANDY_SEP*sel_candyx,	  // DOWN
			 					CANDY_Y + CANDY_SEP*sel_candyy + CANDY_SIZE -4,
								CANDY_SIZE,
								4,
								0xFFFF);									// DRAW TO INDICATE THE SELECTED CANDY
			 	
				DrawRectangle(	CANDY_X + CANDY_SEP*sel_candyx + CANDY_SIZE -4,  // RIGHT	 
			 					CANDY_Y + CANDY_SEP*sel_candyy,
								4,
								CANDY_SIZE,
								0xFFFF);									// DRAW TO INDICATE THE SELECTED CANDY
			 	
				DrawRectangle(	CANDY_X + CANDY_SEP*sel_candyx,  // LEFT	 
			 					CANDY_Y+CANDY_SEP*sel_candyy,
								4,
								CANDY_SIZE,
								0xFFFF);									// DRAW TO INDICATE THE SELECTED CANDY

	   			candy_state = CANDY_SELECTED;
			}
  //************************************************************************************************
  //************************************* CANDY SELECTED STATE *************************************
  //*************************************************************************************************
			else if ((candy_state == CANDY_SELECTED)) {	// IF  we have selected a candy to move
				difc_x = sel_candyx - ts_x;
				difc_y = sel_candyy - ts_y;
		
			   	if ((difc_x|difc_y) != 0) {		// If we have selected another candy to swap
					if (abs(difc_x) >= abs (difc_y)) {		   // Horizontal swap
						  if (difc_x > 0){					   // The difference cannot be 0 at this point
							candy_swapx = sel_candyx - 1;
							candy_swapy = sel_candyy;
							candy_st = (0 << 1)|(0 << 0);	   // Selected candy goes left
						}
						  else {
							candy_swapx = sel_candyx + 1;
							candy_swapy = sel_candyy;
							candy_st = (1 << 1)|(0 << 0);	   // Selected candy goes right
						}
					}
					else {
						  if (difc_y > 0){
							candy_swapx = sel_candyx ;	   
							candy_swapy = sel_candyy - 1;
							candy_st = (0 << 1)|(1 << 0);	   // Selected candy goes up
						}
						  else {
							candy_swapx = sel_candyx;
							candy_swapy = sel_candyy + 1;
							candy_st = (1 << 1)|(1 << 0);		// Selected candy goes down
						}
					}
				 //           SWAP CANDY  	 (We redraw candy to eliminate mark)
 
				candy_state = CANDY_SWAPPING;
				LPC_DAC->DACCNTVAL = 1134;					// Set emision frecuency
				DMA_DAC_audio_Channel_Init( 0, click_audio,1462);	  //Make sound bitcheeeessss !!!

				}
		   	}	  // FINISH SELECTED STATE
		}
 		if (display.x < 80) {
			tx_cadena_UART0("P");
		 }
	}	
	

  //************************************************************************************************
  //************************************* DRAW DELETING TRANSITION **********************************
  //*************************************************************************************************

   if (candy_state == CANDY_DELETING) {
	candy_acc++;
	// ****************************  DRAWING THE EXISTING CANDY *******************************
	for (i = 0; i < CANDY_COL; i++){		// DIBUJAMOS COLUMNA A COLUMNA VIENDO SI TENEMOS QUE DEJARLOS CAER O NO
		
		for (j = CANDY_ROW-1; j >= 0 ; j--){	   // Cada candy de la columna	(EMPEZAMOS DE ABAJO A ARRIBA) 

		  if (((candy_array[i][j] >> 4) & 1 ) != 1){	 // Si no se ha eliminado el candy 

			aux = candy_array[i][j] >> CANDY_GAPS;	// Number of deleted candy below

			if (aux != 0){							// If the candy is falling	(Then we have to draw it)

			   change_finish = 1;				   // Indicate that change havent finished

			   candy_desp[i][j] += CANDY_VEL + candy_acc;		// Desplaze the candy down

				// If it goes down enough !!!!!!!!!!!
			  if (candy_desp[i][j] >= CANDY_SEP*aux){
			  	  candy_desp[i][j] =  CANDY_SEP*aux;

			   	 candy_array[i][j+aux] =  candy_array[i][j] & 0xF;	// Give the PROPER candy the value 
				 		// This will also un-delete the value

				  candy_array[i][j] &= ~(7 << CANDY_GAPS);		// Other candy should give it the value	
				 // candy_array[i][j] = 6;		// Other candy should give it the value	
		 												
				 // candy_desp[i][j] = 0;	// Set displacement to 0
	
			   }
				draw_RGB565_image (CANDY_X + CANDY_SEP*i ,	 	// Desplazamiento X siempre sera fijo
									CANDY_Y + CANDY_SEP*j + candy_desp[i][j], 	// Desplazamiento y cambia
									 CANDY_SIZE, CANDY_SIZE,					// Tamaño
									 (uint8_t *) caramels[candy_array[i][j]&7]); // Tipo	
			}
		}	
	  } 
	 }
	 // ****************************  DRAWING THE NEW CANDY *******************************
	 
for (i = 0; i < CANDY_COL; i++){		// DIBUJAMOS COLUMNA A COLUMNA VIENDO SI TENEMOS QUE DEJARLOS CAER O NO
		
		for (j = CANDY_ROW-1; j >= 0 ; j--){	   // Cada candy de la columna	(EMPEZAMOS DE ABAJO A ARRIBA) 
							  // Ya que sino se sobreescribirian valores.
		  if (new_candy_array[i][j] != 0){	 //  If there is a new candy in that column

			aux = new_candy_array[i][j] >> CANDY_GAPS;	// Number of deleted candy in the column

			if (aux != 0){							// If the candy is falling	(Then we have to draw it)

			   change_finish = 1;				   // Indicate that change havent finished

			   new_candy_desp[i][j] += CANDY_VEL + candy_acc;		// Desplaze the candy down

				// IF THIS ONE IS DONE GOING DOWN
			  if (new_candy_desp[i][j] >= CANDY_SEP*aux){
			  	  new_candy_desp[i][j] =  CANDY_SEP*aux;

			   	 candy_array[i][ (aux-1) - j ] =  new_candy_array[i][j] & 0xF;	// Give the PROPER candy the value 
				 
				 // This will also un-delete the value
				 new_candy_array[i][j] &= ~(7 << CANDY_GAPS);		// Other candy should give it the value		 												// POSSIBLE PROBLEM OVERWRITTING BEFURE USING
				 // new_candy_desp[i][j] = 0;	// Set displacement to 0
	
			   }
			   	aux1 = new_candy_desp[i][j] - CANDY_SEP*(j+1) ;  // ABSOLUTE ORIGIN OF THE IMAGE (WITHOUT OFFSET)
				if (aux1 >= 0) {	   					// If the whole caramel fits in the screen 
					draw_RGB565_image (CANDY_X + CANDY_SEP*i ,	 	// Desplazamiento X siempre sera fijo
									 aux1 + CANDY_Y, 	// Desplazamiento y cambia
									 CANDY_SIZE, CANDY_SIZE,					// Tamaño
									 (uint8_t *) caramels[new_candy_array[i][j]&7]); // Tipo
				}
				else {
							   // If part of the caramel fits in the screen
					aux1 = aux1 + CANDY_SEP; 	   // aux1 is now how much of the image we show
					if (aux1 >= 1){ 
						draw_RGB565_image (CANDY_X + CANDY_SEP*i ,	 	// Desplazamiento X siempre sera fijo
									 CANDY_Y, 	// Desplazamiento y cambia
									 CANDY_SIZE, aux1,					// Tamaño
									 (uint8_t *) (caramels[new_candy_array[i][j]&7]+2*CANDY_SIZE*(CANDY_SIZE - aux1))); // Tipo					
					   
					}
				}
			}	
	  }   }
	 }
	 

		 if (change_finish == 0){
			for (i = 0; i < CANDY_COL; i++){
				for (j = 0; j < CANDY_ROW; j++){
					candy_desp[i][j] = 0;
					new_candy_array[i][j] = 0;
					new_candy_desp[i][j] = 0;
		
				}	
			}

		 	candy_state = CANDY_WAITING;
			candy_combo2();
			candy_acc = 0;
		}

	}
  //************************************************************************************************
  //************************************* DRAW CANDY SWAPPING **********************************
  //*************************************************************************************************

	if (candy_state == CANDY_SWAPPING) {
		// GRAFICAL TRANSITION OF THE CARAMElS
		// We have to draw, first the caramel selected, second, the part corresponding to the 
		// other caramel and third, the background parts.
		// We will use the variables candy_desp for the displacements
		// Depending of the direction of the transaction we have to increase or decrease one coordinate

		// IF WE ARE FINISHED MOVING THEM!!!
		 // Como se desplazan a la misma velocidad los 2, los espacios vacios tendran la misma longitud
		  
	   //************* ACTUALIZE POSITION OF CANDY *************************
   		if (((candy_st >> 1)& 0x1)== 0){	   	// Selected Toward LEFT or UP
			candy_desp[sel_candyx][sel_candyy] -=  (CANDY_VEL + candy_acc);		
			candy_desp[candy_swapx][candy_swapy]+=  CANDY_VEL + candy_acc;	   
		}
		else {										// Selected Toward RIGHT or DOWN
  			candy_desp[sel_candyx][sel_candyy] +=  CANDY_VEL + candy_acc;	
			candy_desp[candy_swapx][candy_swapy]-=  (CANDY_VEL + candy_acc);	   
		}
		candy_acc++;

		if (abs(candy_desp[sel_candyx][sel_candyy]) > CANDY_SEP){
		 	 aux = candy_array[sel_candyx][sel_candyy];
			 candy_array[sel_candyx][sel_candyy] = candy_array[candy_swapx][candy_swapy];
			 candy_array[candy_swapx][candy_swapy] = aux;


			 candy_desp[sel_candyx][sel_candyy] =  0;
			 candy_desp[candy_swapx][candy_swapy] = 0;

			 candy_state = CANDY_WAITING;
			 candy_acc = 0;
		}

		if ((candy_st & 0x1)== 0){				// If it is an horizontal swap
		 // SWAP CANDY
  			draw_RGB565_image (CANDY_X + CANDY_SEP*candy_swapx + candy_desp[candy_swapx][candy_swapy],	 	
								CANDY_Y+CANDY_SEP*sel_candyy, 	// Desplazamiento y cambia
								CANDY_SIZE, CANDY_SIZE,					// Tamaño
								(uint8_t *) caramels[candy_array[candy_swapx][candy_swapy]&7]); // Tipo

		  //  SELECTED CANDY 
			draw_RGB565_image (CANDY_X + CANDY_SEP*sel_candyx + candy_desp[sel_candyx][sel_candyy],	 	
								CANDY_Y+CANDY_SEP*sel_candyy, 	
								CANDY_SIZE, CANDY_SIZE,					// Tamaño
								(uint8_t *) caramels[candy_array[sel_candyx][sel_candyy]&7]); // Tipo

	
	
		
		 	// BACKGROUND PARTS
		 	// Como se desplazan a la misma velocidad los 2, los espacios vacios tendran la misma longitud
		 	aux = abs(candy_desp[sel_candyx][sel_candyy]); 
			aux = min_int(aux,CANDY_SEP - aux);
			// Aux is the length of the background we are seeing.
			if (aux > 0){				
				 DrawRectangle(	CANDY_X + CANDY_SEP*min_int(candy_swapx,sel_candyx),	 // LEFT BACKGROUND
				 			    CANDY_Y + CANDY_SEP*sel_candyy,
								aux,
								CANDY_SIZE ,
								bgColor);

 				 DrawRectangle(	CANDY_X + CANDY_SEP*(1+min_int(candy_swapx,sel_candyx))+ (CANDY_SEP - aux),	 // RIGHT BACKGROUND
				 			    CANDY_Y + CANDY_SEP*sel_candyy,
								aux,
								CANDY_SIZE ,
								bgColor);
			}
		 
		}
		else {	   // If it is a vertical swap
	
		// SWAP CANDY
  			draw_RGB565_image (CANDY_X + CANDY_SEP*candy_swapx ,	 	
								CANDY_Y+CANDY_SEP*candy_swapy + candy_desp[candy_swapx][candy_swapy], 
								CANDY_SIZE, CANDY_SIZE,					// Tamaño
								(uint8_t *) caramels[candy_array[candy_swapx][candy_swapy]&7]); // Tipo

  		  //  SELECTED CANDY 
			draw_RGB565_image (CANDY_X + CANDY_SEP*sel_candyx,	 	
								CANDY_Y+CANDY_SEP*sel_candyy + candy_desp[sel_candyx][sel_candyy], 	
								CANDY_SIZE, CANDY_SIZE,					// Tamaño
								(uint8_t *) caramels[candy_array[sel_candyx][sel_candyy]&7]); // Tipo


			// BACKGROUND PARTS
		 	// Como se desplazan a la misma velocidad los 2, los espacios vacios tendran la misma longitud
		 	aux = abs(candy_desp[sel_candyx][sel_candyy]); 
			aux = min_int(aux,CANDY_SEP - aux);
			// Aux is the length of the background we are seeing.
			if (aux > 0){				
				 DrawRectangle(	CANDY_X + CANDY_SEP*sel_candyx,	 // LEFT BACKGROUND
				 			    CANDY_Y + CANDY_SEP*min_int(candy_swapy,sel_candyy),
								CANDY_SIZE,
								aux,
								bgColor);

 				 DrawRectangle(	CANDY_X + CANDY_SEP*sel_candyx,	 // RIGHT BACKGROUND
				 			    CANDY_Y + CANDY_SEP*(1+min_int(candy_swapy,sel_candyy))+ (CANDY_SEP - aux),
								CANDY_SIZE,
								aux,
								bgColor);
			}

   		}		

		 candy_combo2();
		 }


}
  /******************************************************************************
* Function Name  : init_PONG
* Description    : Initializes the PONG game variables. 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/

void init_CANDY(void){
		int i,j;
 		//	resolution(1);
			bgColor = 0x00;
			LCD_Clear(bgColor);
			time_pong = 0;
			time_game = 0;
			num_times_game = 0;
			candy_score = 0;

	for (i = 0; i < CANDY_COL; i++){
		for (j = 0; j < CANDY_ROW; j++){
			candy_array[i][j]=rand_number()%6;
			candy_desp[i][j] = 0;
			new_candy_array[i][j] = 0;
			new_candy_desp[i][j] = 0;

		}	
	}
	candy_state = CANDY_WAITING;
	draw_all_candy();
	candy_combo2();
}
  /******************************************************************************
* Function Name  : init_PONG
* Description    : Initializes the PONG game variables. 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
 


 /* This function works in 2 rounds:
 	1- Checks for 3+ candy row or column combinations and indicate in each candy that its eliminated
	2- Checks all the candy and for every deleted one does the proper thing on the new_candy_array */
 void candy_combo2(void){
	int i,j,k,l;
	int aux;
	uint8_t candy_cont;

	uint8_t candy_detected;
  //**************************  CHECK FOR 3 IN A COLUMN *********************************
if (candy_state == CANDY_WAITING) {
  for (i = 0; i < CANDY_COL; i++) {	   		// We check every  column combination of 3       |
 //																						     \/
  	   candy_detected =  candy_array[i][0]&0xF;
	   candy_cont = 1;

 		for (j = 1; j < CANDY_ROW; j++) {	  // For every element of the column
			if ((candy_array[i][j]&0xF) == candy_detected){
				candy_cont++;
			}
			else {
				if ((candy_cont >= 3)) {		   // If there were 3 in a row and we have to delete it !!
					candy_score += 10 * candy_cont;				
					candy_state = CANDY_DELETING;
					for (k = 0; k < candy_cont; k++){	      // DELETE CANDY
						candy_array[i][j-(k+1)] |= 1 << 4;	  // Indicate those have been deleted
					}
				}
				candy_detected = candy_array[i][j]&0xF;
				candy_cont = 1;
	   		}
		} 
		//  HERE WE CHECK THE END OF THE COLUMN
				if ((candy_cont >= 3)) {		   // If there were 3 in a row and we have to delete it !!
					candy_score += 10 * candy_cont;	
					candy_state = CANDY_DELETING;
					for (k = 0; k < candy_cont; k++){	     // DELETE CANDY
						candy_array[i][j-(k+1)] |= 1 << 4;	  // Indicate those have been deleted
					}

				}
		}

    //**************************  CHECK FOR 3 IN A ROW *********************************
	// Here we have to check that there are 3+ in a row and check if there are other candy 
	// beside the columns deleted of the same candy 

   for (i = 0; i < CANDY_ROW; i++) {	   		// We check every  column combination of 3   -->
 //																						     
  	   candy_detected =  candy_array[0][i]&0xF;
	   candy_cont = 1;

 		for (j = 1; j < CANDY_COL; j++) {	  // For every element of the column
			if ((candy_array[j][i]&0xF) == candy_detected){
				candy_cont++;
			}
			else {

				if ((candy_cont >= 3)) {		   // If there were 3 in a row and we have to delete it !!
		 			candy_score += 10 * candy_cont;
					candy_state = CANDY_DELETING;
					for (k = 0; k < candy_cont; k++){			 // DELETE   	
						candy_array[j-(k+1)][i]|= 1 << 4;	   	// Indicate its deleted
					}
				}
			 candy_detected = candy_array[j][i]&0xF;
			 candy_cont = 1;
			}

	   	}
		 
		//  HERE WE CHECK THE END OF THE ROW
				if ((candy_cont >= 3)) {		   // If there were 3 in a row and we have to delete it !!
		 			candy_score += 10 * candy_cont;	
					candy_state = CANDY_DELETING;
					for (k = 0; k < candy_cont; k++){			 // DELETE   	
						candy_array[j-(k+1)][i]|= 1 << 4;	   	// Indicate its deleted
					}	
				}
	   }
  

	if (candy_state == CANDY_DELETING) {		// If we have deleted candy
  //*********************************  SECOND PART: CREATING NEW CANDY *************************
		 for (i = 0; i < CANDY_COL; i++){		// DIBUJAMOS COLUMNA A COLUMNA VIENDO SI TENEMOS QUE DEJARLOS CAER O NO
				for (j = 0; j < CANDY_ROW; j++){	   // Cada candy de la columna
					  if (((candy_array[i][j] >> 4 ) & 1) == 1){	// If candy is elimanted
								candy_array[i][j]= CANDY_DELETED;		 // DELETE COLOR   	
								aux = (new_candy_array[i][0] >> CANDY_GAPS) + 1;

								for (l = 0; l < aux; l++) {					 // CREATE THE CANDY
									new_candy_array[i][l] &= ~(7 << CANDY_GAPS);
									new_candy_array[i][l] |= (aux << CANDY_GAPS);
								}
						    	new_candy_array[i][aux-1] |= rand_number()%6;	  // IMPORTANTE |= que no =	
						
								for (k = 0; k < j; k++){					  // INDICATE FALLING CANDY
									candy_array[i][k]  += 1 << CANDY_GAPS;	  // Add up  deleted ones below
								}	
						}	
				   }
			}

			draw_all_candy();
	}  

   }
}



 



  void draw_all_candy (void) {
  	uint8_t i,j;
		 	for (i = 0; i < CANDY_COL; i++){		// DIBUJAMOS COLUMNA A COLUMNA VIENDO SI TENEMOS QUE DEJARLOS CAER O NO
				for (j = 0; j < CANDY_ROW; j++){	   // Cada candy de la columna
	   				draw_RGB565_image (CANDY_X + CANDY_SEP*i ,	 	// Desplazamiento X siempre sera fijo
										 CANDY_Y+CANDY_SEP*j, 	// Desplazamiento y cambia
										 CANDY_SIZE, CANDY_SIZE,					// Tamaño
										 (uint8_t *) caramels[candy_array[i][j]&7]); // Tipo
			 	}
		 	}
		}

	int min_int(int a, int b) {
		if (a > b) 
			return b;
		return a ;
	}

