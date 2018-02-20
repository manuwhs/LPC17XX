#include "../GLCD/GLCD.h"
#include "../OV7670/ov7670.h"
#include "../TIMER/Timer.h"
#include "States.h"
#include "../SERVOS/servos.h"
#include "../GAMES/games.h"
#include "../WEBSERVER/Configuration_files/HTTP_demo.h"

	int draw_flag,screen_flag;
	int state;
	int man_auto;
	int j = 0, nb = 0, state_in_process=0;
	
	Rectangle menu_rect[20]; 
	uint8_t num_botton_menu = 0;   
	uint8_t num_bars_menu = 0;	  
	uint32_t type_bar = 0;		   
	///       IMPORTANTE !!!! EL numero ahora de la barra es num_bars_menu, no se lo tenemos que pasar
   	/* Variables globales superimportantes:
		Rectangle menu_rect[10]:   Estructura de rectangulos donde cada uno de ellos representa un boton
								   o barra que puede ser pulsado.
								   Cada vez que dibujemos uno de ellos se creara automaticamente la entrada
								   apropiada.
		uint8_t num_botton_menu:   Número de botones en el menu
		uint8_t num_bars_menu:	   Número de barras en el menu
		uint32_t type_bar:		   Tipo de las barras del menu, cada bit es una barra (0 ó 1)
*/
	
	void conversor_IaS(int numero, char * cadnum);

void Menu_Principal(void)
{
	if(draw_flag==1)
				{
					pass=0;
					Window(0,0,319,239);
				LCD_Clear(Blue);
				GUI_Text(15,17,"MENU PRINCIPAL",Black,Blue);
				GUI_Text(40,51,"Camara",Black,Blue);
				GUI_Text(40,85,"Motores",Black,Blue);
				GUI_Text(40,119,"Control remoto",Black,Blue);
				GUI_Text(40,153,"Juegos",Black,Blue);
					
			// Ponemos a 0 el numero de botones y barras en el menu
	  	  num_botton_menu = 0;   
			  num_bars_menu = 0;	 
			  type_bar = 0;
			// Dibujamos primero todos los botones que crearan su rectangulo asociado
					DrawCross(20,51,Black);
					DrawCross(20,85,Black);
					DrawCross(20,119,Black);
					DrawCross(20,153,Black);
					
					draw_flag=0;
					delay_only = 1;
					delay_us (500000);   

					while (delay_only == 1) {}
				}
				
				Ptr=Read_Ads7846();
				
				if(Ptr!=(void*)0){		 // If detected pulse
				getDisplayPoint(&display, Ptr, &matrix ) ;	// display.x and display.y of detection
				screen_flag=1;	
				}
				else 
				{return;}
				
				if(screen_flag==1)   {				// Si hemos tocado y soltado 
				Window(0,0,319,239);						// In case somehow its fucked

				/// ACCION QUE PROGRAMEMOS A LOS BOTONES 
				if(DetectRectangle(menu_rect[0])) //Boton 1 Camara
				 {state=2;draw_flag = 1;screen_flag=0;}
			  else if(DetectRectangle(menu_rect[1])) //Boton 2 Motores
				 {state=3;draw_flag = 1;screen_flag=0;}
				else if(DetectRectangle(menu_rect[2])) //Boton 3 IP pagina web
				 {state=10;draw_flag = 1;screen_flag=0;}
				else if(DetectRectangle(menu_rect[3])) //Boton 4 juegos
				 {state=23;draw_flag=1;screen_flag=0;}
			 }
			 screen_flag=0;
}

//Estado menu camara
void Menu_Camara(void)
{
	if(draw_flag==1)
				{
					pass=0;
					Window(0,0,319,239);
				LCD_Clear(Blue);
				GUI_Text(15,17,"MENU CAMARA",Black,Blue);
				GUI_Text(40,51,"Ver captura camara",Black,Blue);
				GUI_Text(40,85,"Configurar",Black,Blue);
				GUI_Text(40,119,"Deteccion",Black,Blue);//Deteccion con seguimiento, deteccion sin seguimiento
				GUI_Text(40,153,"Ver configuracion actual",Black,Blue);
				
					// Ponemos a 0 el numero de botones y barras en el menu
	  	  num_botton_menu = 0;   
			  num_bars_menu = 0;	 
			  type_bar = 0;
			
					// Dibujamos primero todos los botones que crearan su rectangulo asociado
					DrawCross(20,51,Black);
					DrawCross(20,85,Black);
					DrawCross(20,119,Black);
					DrawCross(20,153,Black);
					TextButton(240,210,"Atras",Black);
					
					draw_flag=0;
					delay_only = 1;
					delay_us (500000);   

					while (delay_only == 1) {}
				}
				
				Ptr=Read_Ads7846();
				
				if(Ptr!=(void*)0){		 // If detected pulse
				getDisplayPoint(&display, Ptr, &matrix ) ;	// display.x and display.y of detection
				screen_flag=1;	
				}
				else{return;}
				
				if(screen_flag==1)   {				// Si hemos tocado
				Window(0,0,319,239);						
					
				/// ACCION QUE PROGRAMEMOS A LOS BOTONES 
				if(DetectRectangle(menu_rect[0])) //Boton 1 Ver Captura Camara
				 {state=16;screen_flag=0;state_in_process=0;draw_flag=1;}
			  else if(DetectRectangle(menu_rect[1])) //Boton 2 Configurar
				 {state=5;draw_flag = 1;screen_flag=0;}
				else if(DetectRectangle(menu_rect[2])) //Boton 3 Deteccion
				 {state=12;draw_flag = 1;screen_flag=0;}
				 else if(DetectRectangle(menu_rect[3])) //Boton 4 Ver config actual
				 {state=11;draw_flag = 1;screen_flag=0;}
				 else if(DetectRectangle(menu_rect[4])) //Boton 5 Atras
				 {state=1;draw_flag = 1;screen_flag=0;}
			 }
			 screen_flag=0;
}

//Estado ver camara
void Camera_Image(void)
{
	if(draw_flag==1)
	{
	 read_type=0;  //pantalla completa, impresion sin deteccion ni seguimiento
		
	pass=1;
	delay_only = 1;
	delay_us (500000);   

	while (delay_only == 1) {}
	  draw_flag=0;
	}
		
	Ptr=Read_Ads7846();
				
	if(Ptr!=(void*)0){		 // If detected pulse
			getDisplayPoint(&display, Ptr, &matrix ) ;	// display.x and display.y of detection
			screen_flag=1;	
		}
		else{return;}
		
		if(screen_flag==1)
		{state=2;draw_flag=1;screen_flag=0;}
		screen_flag=0;
			
}

//Estado menu motores
void Menu_Motores(void)
{
	if(draw_flag==1)
				{
					pass=0;
					Window(0,0,319,239);
				LCD_Clear(Blue);
				GUI_Text(15,17,"MENU MOTORES",Black,Blue);
				GUI_Text(40,51,"Configuracion",Black,Blue);
				GUI_Text(40,85,"Giro manual de la camara",Black,Blue);

					
					// Ponemos a 0 el numero de botones y barras en el menu
	  	  num_botton_menu = 0;   
			  num_bars_menu = 0;	 
			  type_bar = 0;
			
					// Dibujamos primero todos los botones que crearan su rectangulo asociado
					DrawCross(20,51,Black);
					DrawCross(20,85,Black);					
					TextButton(240,210,"Atras",Black);
					
					
					draw_flag=0;
					delay_only = 1;
					delay_us (500000);   

					while (delay_only == 1) {}
					
				}
				
					Ptr=Read_Ads7846();
				
				if(Ptr!=(void*)0){		 // If detected pulse
				getDisplayPoint(&display, Ptr, &matrix ) ;	// display.x and display.y of detection
				screen_flag=1;	
				}
				else{return;}
				
				if(screen_flag==1)   {				// Si hemos tocado  
				Window(0,0,319,239);						

				/// ACCION QUE PROGRAMEMOS A LOS BOTONES 
				if(DetectRectangle(menu_rect[0])) //Boton 1 Configuracion motores
				 {state=20;draw_flag = 1;screen_flag=0;}
			  else if(DetectRectangle(menu_rect[1])) //Boton 2 Giro manual
				 {state=18;draw_flag = 1;screen_flag=0;}
				else if(DetectRectangle(menu_rect[2])) //Boton 3 Atras
				 {state=1;draw_flag = 1;screen_flag=0;}
			 }
			 screen_flag=0;
}

//Estado Movimiento manual
void Giro_Manual_Camara(void)
{
	
	
	if(draw_flag==1)
	{
		state_in_process=1;
		read_type=5;
	
  Vd_OV7670=60;
	Vx_OV7670=60;
		
		Window(0,0,319,239);
			LCD_Clear(Blue);
		
		Calibration_vars[0]=(uint16_t)(umbral_x*255);
		Calibration_vars[1]=(uint16_t)(umbral_y*255);
		
		// Ponemos a 0 el numero de botones y barras en el menu
	  	num_botton_menu = 0;   
			num_bars_menu = 0;	 
			type_bar = 0;
		
	 // Dibujamos primero todos los botones que crearan su rectangulo asociado

		TextButton(258,215,"Volver",Black);
		
		
		// Despues dibujamos las calibration bars que tambien crearan su rectangulo asociado
		  	DrawCalibrationBar(20,210,200,15,horizontal);	
				DrawCalibrationBar(290,30,15,160,vertical);	
		
		draw_flag=0;			
		
		pass=1;
		state_in_process=0;		
					delay_only = 1;
					delay_us (500000);   

					while (delay_only == 1) {}

	}
	
	//******************** DETECCION DE BOTONES Y BARRAS *****************************
				
			Ptr = Read_Ads7846();	 

			if(Ptr!=(void*)0){		 // If detected pulse
				getDisplayPoint(&display, Ptr, &matrix ) ;	// display.x and display.y of detection
				screen_flag=1;	
				
			}	
			else{return;}

			
			if(screen_flag==1)   {				// Si hemos tocado  
				Window(0,0,319,239);						
				
				/// ACCION QUE PROGRAMEMOS A LOS BOTONES 
				 if(DetectRectangle(menu_rect[0])) //Boton 1 Volver
				 {state=3;draw_flag = 1;screen_flag=0;}
				 
			 }
				 
				//////////// OBTENCION DEL VALOR DE LAS BARRAS Y ACTUALIZACION

			  for (j = 0; j < num_bars_menu;j++) {		 // Mira todas las barras y las actualiza si debe
			  	nb =  num_botton_menu + j;				// Rectangulo correspondiente a la barra

			   	if(DetectRectangle(menu_rect[nb])){ 	
					state_in_process=1;
				    Get_CalibrationValue(j,menu_rect[nb],(type_bar>>j)&1);	/* Nuevo valor */
					UpdateBar(j,menu_rect[nb],(type_bar>>j)&1);			   /* Actualizacion */
					screen_flag=0;						
					umbral_x=(float)Calibration_vars[0]/255;
					umbral_y=(float)Calibration_vars[1]/255;

					}
				}
				state_in_process=0;
}

//Estado Configuracion motores
void Config_Mov_Motores(void)
{
	//elegir modo (control borroso, control PD) y configuracion variables de control
	if(draw_flag==1)
				{
					pass=0;
				LCD_Clear(Blue);
				GUI_Text(15,17,"CONFIGURAR GIRO DE LA CAMARA",Black,Blue);
				GUI_Text(40,51,"Elegir modo de control del giro",Black,Blue);
				GUI_Text(40,85,"Configurar variables de control",Black,Blue);

					
					// Ponemos a 0 el numero de botones y barras en el menu
	  	  num_botton_menu = 0;   
			  num_bars_menu = 0;	 
			  type_bar = 0;
			
					// Dibujamos primero todos los botones que crearan su rectangulo asociado
					DrawCross(20,51,Black);
					DrawCross(20,85,Black);					
					TextButton(240,210,"Atras",Black);
					
					
					draw_flag=0;
					
					delay_only = 1;
					delay_us (500000);   

					while (delay_only == 1) {}
					
				}
				
					Ptr=Read_Ads7846();
				
				if(Ptr!=(void*)0){		 // If detected pulse
				getDisplayPoint(&display, Ptr, &matrix ) ;	// display.x and display.y of detection
				screen_flag=1;	
				}
				else{return;}
				
				if(screen_flag==1)   {				// Si hemos tocado  
				Window(0,0,319,239);						

				/// ACCION QUE PROGRAMEMOS A LOS BOTONES 
				if(DetectRectangle(menu_rect[0])) //Boton 1 Elegir modo
				 {state=19;draw_flag = 1;screen_flag=0;}
			  else if(DetectRectangle(menu_rect[1])) //Boton 2 Configurar variables de control
				 {state=20;draw_flag = 1;screen_flag=0;}
				else if(DetectRectangle(menu_rect[2])) //Boton 3 Atras
				 {state=3;draw_flag = 1;screen_flag=0;}
			 }
			 screen_flag=0;
}
//Estado elegir modo
void Elegir_modo_control(void)
{
	if(draw_flag==1)
				{
					pass=0;
				LCD_Clear(Blue);
				GUI_Text(15,17,"ELEGIR MODO DE CONTROL DEL GIRO",Black,Blue);
									
					// Ponemos a 0 el numero de botones y barras en el menu
	  	  num_botton_menu = 0;   
			  num_bars_menu = 0;	 
			  type_bar = 0;
			
					// Dibujamos primero todos los botones que crearan su rectangulo asociado
					TextButton(40,60,"Modo control borroso",Black);
					TextButton(55,90,"Modo control PD",Black);					
					TextButton(240,210,"Atras",Black);
					
					
					draw_flag=0;
					
					delay_only = 1;
					delay_us (500000);   

					while (delay_only == 1) {}
					
				}
				
					Ptr=Read_Ads7846();
				
				if(Ptr!=(void*)0){		 // If detected pulse
				getDisplayPoint(&display, Ptr, &matrix ) ;	// display.x and display.y of detection
				screen_flag=1;	
				}
				else{return;}
				
				if(screen_flag==1)   {				// Si hemos tocado  
				Window(0,0,319,239);						

				/// ACCION QUE PROGRAMEMOS A LOS BOTONES 
				if(DetectRectangle(menu_rect[0])) //Boton 1 modo control borroso
				 {/*modo control borroso*/}
			  else if(DetectRectangle(menu_rect[1])) //Boton 2 modo control PD
				 {/*modo control PD*/}
				else if(DetectRectangle(menu_rect[2])) //Boton 3 Atras
				 {state=17;draw_flag = 1;screen_flag=0;}
			 }
			 screen_flag=0;
}

//Estado config variables de control
void Config_var_control(void)
{
	if(draw_flag==1)
				{
					pass=0;
					Window(0,0,319,239);
				LCD_Clear(Blue);
				GUI_Text(15,17,"CONFIGURAR VARIABLES DE CONTROL",Black,Blue);
				GUI_Text(40,51,"Variables de movimiento horizontal",Black,Blue);
				GUI_Text(40,85,"Variables de movimiento vertical",Black,Blue);

					
					// Ponemos a 0 el numero de botones y barras en el menu
	  	  num_botton_menu = 0;   
			  num_bars_menu = 0;	 
			  type_bar = 0;
			
					// Dibujamos primero todos los botones que crearan su rectangulo asociado
					DrawCross(20,51,Black);
					DrawCross(20,85,Black);					
					TextButton(240,210,"Atras",Black);
					
					
					draw_flag=0;
					
					delay_only = 1;
					delay_us (500000);   

					while (delay_only == 1) {}
					
				}
				
					Ptr=Read_Ads7846();
				
				if(Ptr!=(void*)0){		 // If detected pulse
				getDisplayPoint(&display, Ptr, &matrix ) ;	// display.x and display.y of detection
				screen_flag=1;	
				}
				else{return;}
				
				if(screen_flag==1)   {				// Si hemos tocado  
				Window(0,0,319,239);						

				/// ACCION QUE PROGRAMEMOS A LOS BOTONES 
				if(DetectRectangle(menu_rect[0])) //Boton 1 config variables movimiento horizontal
				 {state=21;draw_flag = 1;screen_flag=0;}
			  else if(DetectRectangle(menu_rect[1])) //Boton 2 config variables movimiento vertical
				 {state=22;draw_flag = 1;screen_flag=0;}
				else if(DetectRectangle(menu_rect[2])) //Boton 3 Atras
				 {state=3;draw_flag = 1;screen_flag=0;}
			 }
			 screen_flag=0;
}

//Estado Configurar variables movimiento horizontal
void Config_control_hor(void)
{
		
	if(draw_flag==1)
	{
		state_in_process=1;
		read_type=5;  //pantalla enventada, no deteccion
	
	Vd_OV7670=100;
	Vx_OV7670=60;
		
		Window(0,0,319,239);
		LCD_Clear(Blue);
		GUI_Text(225,50,"Proporcional",Black,Blue);
		GUI_Text(225,120,"Diferencial",Black,Blue);
		GUI_Text(20,190,"Mover camara",Black,Blue);
		
	 Calibration_vars[0]=(uint16_t)(umbral_x*255);
		
		// Ponemos a 0 el numero de botones y barras en el menu
	  	num_botton_menu = 0;   
			num_bars_menu = 0;	 
			type_bar = 0;
		
	 // Dibujamos primero todos los botones que crearan su rectangulo asociado
		TextButton(280,70,"+",Black);
		TextButton(240,70,"-",Black);
		TextButton(280,140,"+",Black);
		TextButton(240,140,"-",Black);
		TextButton(258,215,"Volver",Black);
		
		
		// Despues dibujamos las calibration bars que tambien crearan su rectangulo asociado
		  	DrawCalibrationBar(20,215,220,15,horizontal);	
		
		draw_flag=0;			
		
		pass=1;
		state_in_process=0;	
					delay_only = 1;
					delay_us (500000);   

					while (delay_only == 1) {}
	
	}
	
	//******************** DETECCION DE BOTONES Y BARRAS *****************************
				
			Ptr = Read_Ads7846();	 

			if(Ptr!=(void*)0){		 // If detected pulse
				getDisplayPoint(&display, Ptr, &matrix ) ;	// display.x and display.y of detection
				screen_flag=1;	
				
			}	
			else{return;}

			
			if(screen_flag==1)   {				// Si hemos tocado  
				Window(0,0,319,239);						
				
				/// ACCION QUE PROGRAMEMOS A LOS BOTONES 
				if(DetectRectangle(menu_rect[0])) //Boton 1 proporcional_x ++
				 {cons_pro_x+=0.1;}
			  else if(DetectRectangle(menu_rect[1])) //Boton 2 proporcional_x --
				 {if(cons_pro_x==0);
					else cons_pro_x-=0.1;}
				else if(DetectRectangle(menu_rect[2])) //Boton 3 diferencial_x ++
				 {cons_dif_x+=0.1;}
				 else if(DetectRectangle(menu_rect[3])) //Boton 4 diferencial_x --
				 {if(cons_dif_x==0);
					else cons_dif_x-=0.1;}
				 else if(DetectRectangle(menu_rect[4])) //Boton 5 Volver
				 {state=20;draw_flag = 1;screen_flag=0;}
				 
			 }
				 
				//////////// OBTENCION DEL VALOR DE LAS BARRAS Y ACTUALIZACION

			  for (j = 0; j < num_bars_menu;j++) {		 // Mira todas las barras y las actualiza si debe
			  	nb =  num_botton_menu + j;				// Rectangulo correspondiente a la barra

			   	if(DetectRectangle(menu_rect[nb])){ 	
						state_in_process=1;
				    Get_CalibrationValue(j,menu_rect[nb],(type_bar>>j)&1);	/* Nuevo valor */
					  UpdateBar(j,menu_rect[nb],(type_bar>>j)&1);			   /* Actualizacion */

					screen_flag=0;
						
						umbral_x=(float)Calibration_vars[0]/255;

					}
				}
				state_in_process=0;
}
//Estado Configurar variables movimiento vertical
void Config_control_ver(void)
{
	
	if(draw_flag==1)
	{
		state_in_process=1;
		read_type=5;
	
	Vd_OV7670=80;
	Vx_OV7670=60;
		
		Window(0,0,319,239);
			LCD_Clear(Blue);
		GUI_Text(10,210,"Proporcional",Black,Blue);
		GUI_Text(135,210,"Diferencial",Black,Blue);
		GUI_Text(245,10,"Mover cam",Black,Blue);
		
		Calibration_vars[0]=(uint16_t)(umbral_y*255);
		
		// Ponemos a 0 el numero de botones y barras en el menu
	  	num_botton_menu = 0;   
			num_bars_menu = 0;	 
			type_bar = 0;
		
	 // Dibujamos primero todos los botones que crearan su rectangulo asociado
		TextButton(110,190,"+",Black);
		TextButton(110,215,"-",Black);
		TextButton(230,190,"+",Black);
		TextButton(230,215,"-",Black);
		TextButton(258,215,"Volver",Black);
		
		
		// Despues dibujamos las calibration bars que tambien crearan su rectangulo asociado
		  	DrawCalibrationBar(285,30,17,160,vertical);	
		
		draw_flag=0;			
		
		pass=1;
			state_in_process=0;
					delay_only = 1;
					delay_us (500000);   

					while (delay_only == 1) {}
	
	}
	
	//******************** DETECCION DE BOTONES Y BARRAS *****************************
				
			Ptr = Read_Ads7846();	 

			if(Ptr!=(void*)0){		 // If detected pulse
				getDisplayPoint(&display, Ptr, &matrix ) ;	// display.x and display.y of detection
				screen_flag=1;	
				
			}	
			else{return;}

			
			if(screen_flag==1)   {				// Si hemos tocado  
				Window(0,0,319,239);						
				
				/// ACCION QUE PROGRAMEMOS A LOS BOTONES 
				if(DetectRectangle(menu_rect[0])) //Boton 1 proporcional_y ++
				 {cons_pro_y+=0.1;}
			  else if(DetectRectangle(menu_rect[1])) //Boton 2 proporcional_y --
				 {if(cons_pro_y==0);
					else cons_pro_y-=0.1;}
				else if(DetectRectangle(menu_rect[2])) //Boton 3 diferencial_y ++
				 {cons_dif_y+=0.1;}
				 else if(DetectRectangle(menu_rect[3])) //Boton 4 diferencial_y --
				 {if(cons_dif_y==0);
					else cons_dif_y-=0.1;}
				 else if(DetectRectangle(menu_rect[4])) //Boton 5 Volver
				 {state=20;draw_flag = 1;screen_flag=0;}
				 
			 }
				 
				//////////// OBTENCION DEL VALOR DE LAS BARRAS Y ACTUALIZACION

			  for (j = 0; j < num_bars_menu;j++) {		 // Mira todas las barras y las actualiza si debe
			  	nb =  num_botton_menu + j;				// Rectangulo correspondiente a la barra

			   	if(DetectRectangle(menu_rect[nb])){ 	
						state_in_process=1;
				    Get_CalibrationValue(j,menu_rect[nb],(type_bar>>j)&1);	/* Nuevo valor */
					  UpdateBar(j,menu_rect[nb],(type_bar>>j)&1);			   /* Actualizacion */

					screen_flag=0;
						
						umbral_y=(float)Calibration_vars[0]/255;

					}
				}
				state_in_process=0;
}
//Estado Configurar 1
void Config_1(void)
{
	
	if(draw_flag==1)
	{
		state_in_process=1;
		read_type=5;
	
	Vd_OV7670=0;
	Vx_OV7670=40;
		
		Window(0,0,319,239);
			LCD_Clear(Blue);
		
		// Ponemos a 0 el numero de botones y barras en el menu
	  	num_botton_menu = 0;   
			num_bars_menu = 0;	 
			type_bar = 0;
		
	 // Dibujamos primero todos los botones que crearan su rectangulo asociado
		DrawArrow(235,225,Black,este);
		TextButton(30,215,"Captura y deteccion",Black);
		TextButton(258,215,"Volver",Black);
		draw_flag=0;	
		
		pass=1;
		state_in_process=0;	
		delay_only = 1;
		delay_us (500000);   

		while (delay_only == 1) {}
		
		
	}
	
	//******************** DETECCION DE BOTONES Y BARRAS *****************************
				
			Ptr = Read_Ads7846();	

			if(Ptr!=(void*)0)		 // If detected pulse
				{
				getDisplayPoint(&display, Ptr, &matrix ) ;	// display.x and display.y of detection
				screen_flag=1;	
			}				
				else{return;}
				
			if(screen_flag==1)   {				// Si hemos tocado 
				Window(0,0,319,239);						

				//////////// ACCION QUE PROGRAMEMOS A LOS BOTONES 
				if(DetectRectangle(menu_rect[0])) //Boton 1 Flecha
				 {state=5;draw_flag = 1;screen_flag=0;}
			  else if(DetectRectangle(menu_rect[1])) //Boton 2 Captura y deteccion
				 {state=8;draw_flag = 1;screen_flag=0;}
				else if(DetectRectangle(menu_rect[2])) //Boton 3 Volver
				 {state=2;draw_flag = 1;screen_flag=0;}
				}
				screen_flag=0;
			}	

//Estado Configurar 2
void Config_2(void)
{
		
	if(draw_flag==1)
	{
		state_in_process=1;
		read_type=5;
	
	Vd_OV7670=0;
	Vx_OV7670=40;
		
		Window(0,0,319,239);
			LCD_Clear(Blue);
		
		// Ponemos a 0 el numero de botones y barras en el menu
	  	num_botton_menu = 0;   
			num_bars_menu = 0;	 
			type_bar = 0;
		
			// Dibujamos primero todos los botones que crearan su rectangulo asociado
		    DrawArrow(17,225,Black,oeste);
				TextButton(48,215,"RGB",Black);
				TextButton(95,215,"Fps",Black);
				TextButton(143,215,"Luz",Black);
				TextButton(190,215,"Reset",Black);
				TextButton(258,215,"Volver",Black);
		
		draw_flag=0;	
		
		pass=1;	
			state_in_process=0;
					delay_only = 1;
					delay_us (500000);   

					while (delay_only == 1) {}		
					
		}
		
		//******************** DETECCION DE BOTONES Y BARRAS *****************************
				
			Ptr = Read_Ads7846();	 

			if(Ptr!=(void*)0)		 // If detected pulse
				{
				getDisplayPoint(&display, Ptr, &matrix ) ;	// display.x and display.y of detection
				screen_flag=1;	
			}						
			else{return;}
			
			if(screen_flag==1)   {				// Si hemos tocado 
				Window(0,0,319,239);						

				//////////// ACCION QUE PROGRAMEMOS A LOS BOTONES 
				if(DetectRectangle(menu_rect[0])) //Boton 1 Flecha
				 {state=4;draw_flag = 1;screen_flag=0;}
			  else if(DetectRectangle(menu_rect[1])) //Boton 2 RGB
				 {state=13;draw_flag = 1;screen_flag=0;man_auto=0;}
				else if(DetectRectangle(menu_rect[2])) //Boton 3 FPS
				 {state=9;draw_flag = 1;screen_flag=0;}
				 else if(DetectRectangle(menu_rect[3])) //Boton 4 LUZ
				 {state=13;draw_flag = 1;screen_flag=0;man_auto=1;}
				 else if(DetectRectangle(menu_rect[4])) //Boton 5 RESET
				 {state=5;draw_flag = 1;screen_flag=0;reset_OV7670();}
				 else if(DetectRectangle(menu_rect[5])) //Boton 6 VOLVER
				 {state=2;draw_flag = 1;screen_flag=0;}
			 }
			 screen_flag=0;
			 
}
//Estado Configurar Luz
void Light_Configuration(void)
{
		
	if(draw_flag==1){
		state_in_process=1;
		read_type=5;
	
	Vd_OV7670=140;
	Vx_OV7670=40;
		
		Window(0,0,319,239);
			LCD_Clear(Blue);
		      GUI_Text(275,10,"Brillo",Black,Blue);
					GUI_Text(225,10,"Contra",Black,Blue);
					GUI_Text(183,10,"Lumin",Black,Blue);

			// LO PROPIO DEL MENU ESPECIFICO
		    getOV7670_BriConLum ();
			Calibration_vars[0] = OV7670bright;
			Calibration_vars[1] = OV7670contrast;
			Calibration_vars[2] = OV7670luminis;
			OV7670_BriConLum (OV7670bright, OV7670contrast, OV7670luminis,0);  /* Modo manual */

			// Ponemos a 0 el numero de botones y barras en el menu
	  	num_botton_menu = 0;   
			num_bars_menu = 0;	 
			type_bar = 0;
		
			// Dibujamos primero todos los botones que crearan su rectangulo asociado
				DrawArrow(17,225,Black,oeste);
				TextButton(48,215,"RGB",Black);
				TextButton(95,215,"Fps",Black);
				TextButton(143,215,"Luz",Black);
				TextButton(190,215,"Reset",Black);
				TextButton(258,215,"Volver",Black);
		
		
		  // Despues dibujamos las calibration bars que tambien crearan su rectangulo asociado
		  	DrawCalibrationBar(195,30,17,160,vertical);	
			  DrawCalibrationBar(245,30,17,160,vertical);	
		 	  DrawCalibrationBar(295,30,17,160,vertical);	
				
			draw_flag=0;		
			pass=1;
				state_in_process=0;
					delay_only = 1;
					delay_us (500000);   

					while (delay_only == 1) {}
					
		}
	
		   //******************** DETECCION DE BOTONES Y BARRAS *****************************
				
			Ptr = Read_Ads7846();	 // Creo que esto detecta que pulsemos la pantalla

			if(Ptr!=(void*)0)		 // If detected pulse
				{
				getDisplayPoint(&display, Ptr, &matrix ) ;	// display.x and display.y of detection
				screen_flag=1;	
					
			}						
			else{return;}
			
			if(screen_flag==1)   {				// Si hemos tocado 
				Window(0,0,319,239);						// In case somehow its fucked

				//////////// ACCION QUE PROGRAMEMOS A LOS BOTONES 
				if(DetectRectangle(menu_rect[0])) //Boton 1 Flecha
				 {state=4;draw_flag = 1;screen_flag=0;}
			  else if(DetectRectangle(menu_rect[1])) //Boton 2 RGB
				 {state=13;draw_flag = 1;screen_flag=0;man_auto=0;}
				else if(DetectRectangle(menu_rect[2])) //Boton 3 FPS
				 {state=9;draw_flag = 1;screen_flag=0;}
				 else if(DetectRectangle(menu_rect[3])) //Boton 4 LUZ
				 {state=13;draw_flag = 1;screen_flag=0;man_auto=1;}
				 else if(DetectRectangle(menu_rect[4])) //Boton 5 RESET
				 {state=5;draw_flag = 1;screen_flag=0;reset_OV7670();}
				 else if(DetectRectangle(menu_rect[5])) //Boton 6 VOLVER
				 {state=2;draw_flag = 1;screen_flag=0;}
			 }
			
				//////////// OBTENCION DEL VALOR DE LAS BARRAS Y ACTUALIZACION

			  for (j = 0; j < num_bars_menu;j++) {		 // Mira todas las barras y las actualiza si debe
			  	nb =  num_botton_menu + j;				// Rectangulo correspondiente a la barra

			   	if(DetectRectangle(menu_rect[nb])){ 
				state_in_process=1;	
				    Get_CalibrationValue(j,menu_rect[nb],(type_bar>>j)&1);	/* Nuevo valor */
					UpdateBar(j,menu_rect[nb],(type_bar>>j)&1);			   /* Actualizacion */

					screen_flag=0;

					 /// ACCION DE LAS BARRAS
					 OV7670bright = Calibration_vars[0];
					OV7670contrast = Calibration_vars[1];
					 OV7670luminis = Calibration_vars[2];

					OV7670_BriConLum (OV7670bright, OV7670contrast, OV7670luminis ,0);  /* Modo manual */
					}
				}
				
				  state_in_process=0;
}

//Estado Configurar fps
void Choose_FPS(void)
{
	char cad_fps[10];
	

	 	
	if(draw_flag==1){
				state_in_process=1;
			read_type=5;
	
	Vd_OV7670=120;
	Vx_OV7670=40;
		
			Window(0,0,319,239);
			LCD_Clear(Blue);
		GUI_Text(207,170,"FPS reales:",Black,Blue);


			// Ponemos a 0 el numero de botones y barras en el menu
	  	  num_botton_menu = 0;   
			  num_bars_menu = 0;	 
			  type_bar = 0;
			// Dibujamos primero todos los botones que crearan su rectangulo asociado
				DrawArrow(17,225,Black,oeste);
				TextButton(48,215,"RGB",Black);
				TextButton(95,215,"Fps",Black);
				TextButton(143,215,"Luz",Black);
				TextButton(190,215,"Reset",Black);
				TextButton(258,215,"Volver",Black);
		
				TextButton(203,30," 1 ",Black);
				TextButton(243,30," 2 ",Black);
				TextButton(283,30," 3 ",Black);	
				TextButton(203,70," 5 ",Black);
				TextButton(243,70,"10 ",Black);  
				TextButton(283,70,"15 ",Black);
				TextButton(203,110,"20 ",Black);
				TextButton(243,110,"30 ",Black);
				TextButton(283,110,"40 ",Black);				
		

			draw_flag=0;			
			
			pass=1;
			state_in_process=0;
			delay_only = 1;
			delay_us (500000);   

			while (delay_only == 1) {}
						
		}
	

	
		   //******************** DETECCION DE BOTONES Y BARRAS *****************************
				
			Ptr = Read_Ads7846();	 // Creo que esto detecta que pulsemos la pantalla

			if(Ptr!=(void*)0){		 // If detected pulse
				getDisplayPoint(&display, Ptr, &matrix ) ;	// display.x and display.y of detection
				screen_flag=1;	
			}	
				else{return;}
				
			if(screen_flag==1)   {				// Si hemos tocado y soltado 
				Window(0,0,319,239);						

				/// ACCION QUE PROGRAMEMOS A LOS BOTONES 
				if(DetectRectangle(menu_rect[0])) //Boton 1 Flecha
				 {state=4;draw_flag = 1;screen_flag=0;}
			  else if(DetectRectangle(menu_rect[1])) //Boton 2 RGB
				 {state=13;draw_flag = 1;screen_flag=0;man_auto=0;}
				else if(DetectRectangle(menu_rect[2])) //Boton 3 FPS
				 {state=9;draw_flag = 1;screen_flag=0;}
				 else if(DetectRectangle(menu_rect[3])) //Boton 4 LUZ
				 {state=13;draw_flag = 1;screen_flag=0;man_auto=1;}
				 else if(DetectRectangle(menu_rect[4])) //Boton 5 RESET
				 {state=5;draw_flag = 1;screen_flag=0;reset_OV7670();}
				 else if(DetectRectangle(menu_rect[5])) //Boton 6 VOLVER
				 {state=2;draw_flag = 1;screen_flag=0;}
				 else if(DetectRectangle(menu_rect[6])) //Boton 7 1fps
				 {OV7670_FPS(1);}
				 else if(DetectRectangle(menu_rect[7])) //Boton 8 2fps
				 {OV7670_FPS(2);}
				 else if(DetectRectangle(menu_rect[8])) //Boton 9 3fps
				 {OV7670_FPS(3);}
				 else if(DetectRectangle(menu_rect[9])) //Boton 10 5fps
				 {OV7670_FPS(5);}
				 else if(DetectRectangle(menu_rect[10])) //Boton 11 10fps
				 {OV7670_FPS(10);}
				 else if(DetectRectangle(menu_rect[11])) //Boton 12 15fps
				 {OV7670_FPS(15);}
				 else if(DetectRectangle(menu_rect[12])) //Boton 13 20fps
				 {OV7670_FPS(20);}
				 else if(DetectRectangle(menu_rect[13])) //Boton 14 30fps
				 {OV7670_FPS(30);}
				 else if(DetectRectangle(menu_rect[14])) //Boton 15 40fps
				 {OV7670_FPS(40);}

				state_in_process=1;
				conversor_IaS(OV7670fps,cad_fps);
				GUI_Text(300,170,cad_fps,Grey,Black);
				state_in_process=0;
				 
			 }
				 screen_flag=0;
			 
}

//Estado Configurar RGB
void RGB_Configuration(void)
{
	
	
	if(draw_flag==1){
		state_in_process=1;
	read_type=5;
	
	Vd_OV7670=140;
	Vx_OV7670=40;
	
			Window(0,0,319,239);
			LCD_Clear(Blue);
					GUI_Text(283,10,"Azul",Black,Blue);
					GUI_Text(230,10,"Verde",Black,Blue);
					GUI_Text(187,10,"Rojo",Black,Blue);

			// LO PROPIO DEL MENU ESPECIFICO
		OV7670_AWBmode (0);		/* Modo de ganancia manual */
		getOV7670_RGBgain ();
		Calibration_vars[0] = OV7670R_gain;
			Calibration_vars[1] = OV7670G_gain;
			Calibration_vars[2] = OV7670B_gain;

			// Ponemos a 0 el numero de botones y barras en el menu
	  	  num_botton_menu = 0;   
			  num_bars_menu = 0;	 
			  type_bar = 0;
			// Dibujamos primero todos los botones que crearan su rectangulo asociado
				DrawArrow(17,225,Black,oeste);
				TextButton(48,215,"RGB",Black);
				TextButton(95,215,"Fps",Black);
				TextButton(143,215,"Luz",Black);
				TextButton(190,215,"Reset",Black);
				TextButton(258,215,"Volver",Black);
		
		
		  // Despues dibujamos las calibration bars que tambien crearan su rectangulo asociado
		  	DrawCalibrationBar(195,30,17,160,vertical);	
			  DrawCalibrationBar(245,30,17,160,vertical);	
		 	  DrawCalibrationBar(295,30,17,160,vertical);	

			draw_flag=0;


		pass=1;
					 state_in_process=0;
					delay_only = 1;
					delay_us (500000);   

					while (delay_only == 1) {}
						
		}
	
		   //******************** DETECCION DE BOTONES Y BARRAS *****************************
				
			Ptr = Read_Ads7846();	 

			if(Ptr!=(void*)0){		 // If detected pulse
				getDisplayPoint(&display, Ptr, &matrix ) ;	// display.x and display.y of detection
				screen_flag=1;	
				
			}	
				else {return;}
				
			if(screen_flag==1)   {				// Si hemos tocado 
				Window(0,0,319,239);						

				/// ACCION QUE PROGRAMEMOS A LOS BOTONES 
				if(DetectRectangle(menu_rect[0])) //Boton 1 Flecha
				 {state=4;draw_flag = 1;screen_flag=0;}
			  else if(DetectRectangle(menu_rect[1])) //Boton 2 RGB
				 {state=13;draw_flag = 1;screen_flag=0;man_auto=0;}
				else if(DetectRectangle(menu_rect[2])) //Boton 3 FPS
				 {state=9;draw_flag = 1;screen_flag=0;}
				 else if(DetectRectangle(menu_rect[3])) //Boton 4 LUZ
				 {state=13;draw_flag = 1;screen_flag=0;man_auto=1;}
				 else if(DetectRectangle(menu_rect[4])) //Boton 5 RESET
				 {state=5;draw_flag = 1;screen_flag=0;reset_OV7670();}
				 else if(DetectRectangle(menu_rect[5])) //Boton 6 VOLVER
				 {state=2;draw_flag = 1;screen_flag=0;}
			 }
				 
				//////////// OBTENCION DEL VALOR DE LAS BARRAS Y ACTUALIZACION

			  for (j = 0; j < num_bars_menu;j++) {		 // Mira todas las barras y las actualiza si debe
			  	nb =  num_botton_menu + j;				// Rectangulo correspondiente a la barra

			   	if(DetectRectangle(menu_rect[nb])){ 
						state_in_process=1;						
				    Get_CalibrationValue(j,menu_rect[nb],(type_bar>>j)&1);	/* Nuevo valor */
					  UpdateBar(j,menu_rect[nb],(type_bar>>j)&1);			   /* Actualizacion */

					screen_flag=0;

					 /// ACCION DE LAS BARRAS
					 OV7670R_gain = Calibration_vars[0] ;
					 OV7670G_gain = Calibration_vars[1] ;
					 OV7670B_gain = Calibration_vars[2];

  				OV7670_RGBgain (OV7670R_gain,OV7670G_gain,OV7670B_gain);	 /* Establece color */
					}
				}
				state_in_process=0;


}
// Estado mostrar IP
void Show_IP(void)
{
	if(draw_flag==1)
	{
		pass=0;
		Window(0,0,319,239);
		LCD_Clear(Blue);
		
		
		// Ponemos a 0 el numero de botones y barras en el menu
	  	num_botton_menu = 0;   
		num_bars_menu = 0;	 
		type_bar = 0;
		
	 	// Dibujamos primero todos los botones que crearan su rectangulo asociado
		TextButton(258,215,"Volver",Black);
		
		draw_flag=0;
		
		delay_only = 1;
		delay_us (500000);   
		LCDupdate =__TRUE;
		while (delay_only == 1) {}
	}

	 upd_display ();	

	//******************** DETECCION DE BOTONES Y BARRAS *****************************
			
			

			Ptr = Read_Ads7846();	 // Creo que esto detecta que pulsemos la pantalla

			if(Ptr!=(void*)0){		 // If detected pulse
				getDisplayPoint(&display, Ptr, &matrix ) ;	// display.x and display.y of detection
				screen_flag=1;	
			}	
			else{return;}

				if(screen_flag==1)   {				// Si hemos tocado y soltado 
				Window(0,0,319,239);						// In case somehow its fucked

				/// ACCION QUE PROGRAMEMOS A LOS BOTONES 
				if(DetectRectangle(menu_rect[0])) //Boton 1 volver
				 {state=1;draw_flag = 1;screen_flag=0;}
			  				 
			 }
			 screen_flag=0;
		
}

//Estado mostrar configuracion actual
void Show_Actual_Config(void)
{
	char cad_var[10];
	if (draw_flag==1)
	{
		pass=0;
		Window(0,0,319,239);
		LCD_Clear(Blue);
		
					
					GUI_Text(15,10,"Umbral de deteccion",Black,Blue);
					conversor_IaS(umbral,cad_var);
					GUI_Text(280,10,cad_var,Grey,Blue);
		
					GUI_Text(15,35,"Brillo",Black,Blue);
					conversor_IaS(OV7670bright,cad_var);
					GUI_Text(280,35,cad_var,Grey,Blue);
		
					GUI_Text(15,60,"Contraste",Black,Blue);
					conversor_IaS(OV7670contrast,cad_var);
					GUI_Text(280,60,cad_var,Grey,Blue);
					
					GUI_Text(15,85,"Luminosidad",Black,Blue);
					conversor_IaS(OV7670luminis,cad_var);
					GUI_Text(280,85,cad_var,Grey,Blue);
					
					GUI_Text(15,110,"Ganancia del rojo",Black,Blue);
					conversor_IaS(OV7670R_gain,cad_var);
					GUI_Text(280,110,cad_var,Grey,Blue);
					
					GUI_Text(15,135,"Ganancia del verde",Black,Blue);
					conversor_IaS(OV7670G_gain,cad_var);
					GUI_Text(280,135,cad_var,Grey,Blue);
					
					GUI_Text(15,160,"Ganancia del azul",Black,Blue);
					conversor_IaS(OV7670B_gain,cad_var);
					GUI_Text(280,160,cad_var,Grey,Blue);

					GUI_Text(15,185,"FPS deseadas",Black,Blue);
					conversor_IaS(OV7670fps,cad_var);
					GUI_Text(280,185,cad_var,Grey,Blue);


					
	// Ponemos a 0 el numero de botones y barras en el menu
	  	num_botton_menu = 0;   
			num_bars_menu = 0;	 
			type_bar = 0;
		
	 // Dibujamos primero todos los botones que crearan su rectangulo asociado
					TextButton(258,215,"Volver",Black);
				
				draw_flag=0;
				delay_only = 1;
					delay_us (500000);   

					while (delay_only == 1) {}
	}
	
	//******************** DETECCION DE BOTONES Y BARRAS *****************************
				
			Ptr = Read_Ads7846();	 // Creo que esto detecta que pulsemos la pantalla

			if(Ptr!=(void*)0){		 // If detected pulse
				getDisplayPoint(&display, Ptr, &matrix ) ;	// display.x and display.y of detection
				screen_flag=1;	
			}	
			else{return;}

				if(screen_flag==1)   {				// Si hemos tocado y soltado 
				Window(0,0,319,239);						// In case somehow its fucked

				/// ACCION QUE PROGRAMEMOS A LOS BOTONES 
				if(DetectRectangle(menu_rect[0])) //Boton 1 volver
				 {state=2;draw_flag = 1;screen_flag=0;}
			  				 
			 }
			 screen_flag=0;
}

//Estado configurar umbral de deteccion y captura de color
void Config_Umbral_Captura(void)
{
	
	
	if(draw_flag==1)
	{
		state_in_process=1;
		read_type=7;
		Vd_OV7670=80;
		Vx_OV7670=40;	
		
		Window(0,0,319,239);
			LCD_Clear(Blue);
		
		GUI_Text(245,10,"Umbral",Black,Blue);
		LCD_DrawLine(285,125,285,150,Black);
		LCD_DrawLine(310,125,310,150,Black);
		LCD_DrawLine(285,125,310,125,Black);
		LCD_DrawLine(285,150,310,150,Black);
		DrawRectangle(285,125,25,25,color_detect);
		
		Calibration_vars[0]=umbral*8;
		
		// Ponemos a 0 el numero de botones y barras en el menu
	  	num_botton_menu = 0;   
			num_bars_menu = 0;	 
			type_bar = 0;
		
	 // Dibujamos primero todos los botones que crearan su rectangulo asociado
		DrawArrow(235,225,Black,este);
		TextButton(30,215,"Captura y deteccion",Black);
		TextButton(258,215,"Volver",Black);
		TextButton(255,187,"Captura",Black);
		
		// Despues dibujamos las calibration bars que tambien crearan su rectangulo asociado
		  	DrawCalibrationBar(255,30,17,153,vertical);	
		
		
		draw_flag=0;			
		
		pass=1;
		  state_in_process=0;
					delay_only = 1;
					delay_us (500000);   

					while (delay_only == 1) {}
						
		
	}
	
	//******************** DETECCION DE BOTONES Y BARRAS *****************************
				
			Ptr = Read_Ads7846();	 // Creo que esto detecta que pulsemos la pantalla

			if(Ptr!=(void*)0){		 // If detected pulse
				getDisplayPoint(&display, Ptr, &matrix ) ;	// display.x and display.y of detection
				screen_flag=1;	
				
			}	
			else{return;}

			if(Capt_flag==1&&screen_flag==1)
				{
						color_detect = GetColor();
						DrawRectangle(285,125,25,25,color_detect);
						
						screen_flag=0;	
					read_type=7;
				} 
			
			if(screen_flag==1)   {				// Si hemos tocado y soltado 
				Window(0,0,319,239);				// In case somehow its fucked

				/// ACCION QUE PROGRAMEMOS A LOS BOTONES 
				if(DetectRectangle(menu_rect[0])) //Boton 1 Flecha
				 {state=5;draw_flag = 1;screen_flag=0;}
			  else if(DetectRectangle(menu_rect[1])) //Boton 2 Captura y umbral de deteccion
				 {state=8;draw_flag = 1;screen_flag=0;}
				else if(DetectRectangle(menu_rect[2])) //Boton 3 Volver
				 {state=2;draw_flag = 1;screen_flag=0;}
				 else if(DetectRectangle(menu_rect[3])) //Boton 4 Captura
				 {Capt_flag=1;screen_flag=0;read_type=5;}
				 
			 }
				 
				//////////// OBTENCION DEL VALOR DE LAS BARRAS Y ACTUALIZACION

			  for (j = 0; j < num_bars_menu;j++) {		 // Mira todas las barras y las actualiza si debe
			  	nb =  num_botton_menu + j;				// Rectangulo correspondiente a la barra

			   	if(DetectRectangle(menu_rect[nb])){ 
							state_in_process=1;
				    Get_CalibrationValue(j,menu_rect[nb],(type_bar>>j)&1);	/* Nuevo valor */
					  UpdateBar(j,menu_rect[nb],(type_bar>>j)&1);			   /* Actualizacion */

					screen_flag=0;
						
						umbral=Calibration_vars[0]/8;

					}
				}
				state_in_process=0;
}

//Estado manual auto
void Manual_vs_Auto(void)
{
	
	
	if(draw_flag==1)
	{
		state_in_process=1;
		read_type=5;
	
	Vd_OV7670=80;
	Vx_OV7670=40;
			
			Window(0,0,319,239);
			LCD_Clear(Blue);
		
		// Ponemos a 0 el numero de botones y barras en el menu
	  	  num_botton_menu = 0;   
			  num_bars_menu = 0;	 
			  type_bar = 0;
			// Dibujamos primero todos los botones que crearan su rectangulo asociado
				DrawArrow(17,225,Black,oeste);
				TextButton(48,215,"RGB",Black);
				TextButton(95,215,"Fps",Black);
				TextButton(143,215,"Luz",Black);
				TextButton(190,215,"Reset",Black);
				TextButton(258,215,"Volver",Black);
				TextButton(245,75,"Manual",Black);
				TextButton(245,125,"Auto",Black);
				
				
		draw_flag=0;		

			pass=1;
			state_in_process=0;
					delay_only = 1;
					delay_us (500000);   

					while (delay_only == 1) {}
						
		}
	
		   //******************** DETECCION DE BOTONES Y BARRAS *****************************
				
			Ptr = Read_Ads7846();	 // Creo que esto detecta que pulsemos la pantalla

			if(Ptr!=(void*)0){		 // If detected pulse
				getDisplayPoint(&display, Ptr, &matrix ) ;	// display.x and display.y of detection
				screen_flag=1;	
			}	
				else{return;}
				
			if(screen_flag==1)   {				// Si hemos tocado y soltado 
				Window(0,0,319,239);						

				/// ACCION QUE PROGRAMEMOS A LOS BOTONES 
				if(DetectRectangle(menu_rect[0])) //Boton 1 Flecha
				 {state=4;draw_flag = 1;screen_flag=0;}
			  else if(DetectRectangle(menu_rect[1])) //Boton 2 RGB
				 {state=13;draw_flag = 1;screen_flag=0;man_auto=0;}
				else if(DetectRectangle(menu_rect[2])) //Boton 3 FPS
				 {state=9;draw_flag = 1;screen_flag=0;}
				 else if(DetectRectangle(menu_rect[3])) //Boton 4 LUZ
				 {state=13;draw_flag = 1;screen_flag=0;man_auto=1;}
				 else if(DetectRectangle(menu_rect[4])) //Boton 5 RESET
				 {state=5;draw_flag = 1;screen_flag=0;/*LLAMAR A FUNCION RESETEO*/}
				 else if(DetectRectangle(menu_rect[5])) //Boton 6 VOLVER
				 {state=2;draw_flag = 1;screen_flag=0;}
				 else if(DetectRectangle(menu_rect[6])) //Boton 7 Manual
				 {if(man_auto==0)
					 state=6;
					 else if(man_auto==1)
						 state=7;
				draw_flag = 1;screen_flag=0;}
				 else if(DetectRectangle(menu_rect[7])) //Boton 8 Auto
				 {
				 if(man_auto==0){
				 	 OV7670_AWBmode (1);	//LLAMAR FUNCION AUTO RGB

				 }
				
				 else if(man_auto==1){

				 OV7670_BriConLum (0x00, 0x40, 7, 1) ;//LLAMAR FUNCION AUTO LUZ


				}
				state=5;draw_flag = 1;screen_flag=0;}
			 }
			 screen_flag=0;
			 
		
}
//Estado Menu Deteccion
void Menu_Deteccion(void)
{
	if(draw_flag==1)
				{
					pass=0;
					Window(0,0,319,239);
				LCD_Clear(Blue);
				GUI_Text(15,17,"MENU DETECCION",Black,Blue);
				GUI_Text(40,51,"Deteccion sin seguimiento",Black,Blue);
				GUI_Text(40,85,"Deteccion con seguimiento",Black,Blue);

					
					// Ponemos a 0 el numero de botones y barras en el menu
	  	  num_botton_menu = 0;   
			  num_bars_menu = 0;	 
			  type_bar = 0;
			
					// Dibujamos primero todos los botones que crearan su rectangulo asociado
					DrawCross(20,51,Black);
					DrawCross(20,85,Black);					
					TextButton(240,210,"Atras",Black);
					
					
					draw_flag=0;
					
					delay_only = 1;
					delay_us (500000);   

					while (delay_only == 1) {}
					
				}
				
					Ptr=Read_Ads7846();
				
				if(Ptr!=(void*)0){		 // If detected pulse
				getDisplayPoint(&display, Ptr, &matrix ) ;	// display.x and display.y of detection
				screen_flag=1;	
				}
				else{return;}
				
				if(screen_flag==1)   {				// Si hemos tocado  
				Window(0,0,319,239);						

				/// ACCION QUE PROGRAMEMOS A LOS BOTONES 
				if(DetectRectangle(menu_rect[0])) //Boton 1 Deteccion sin seguimiento
				 {state=15;draw_flag = 1;screen_flag=0;}
			  else if(DetectRectangle(menu_rect[1])) //Boton 2 Deteccion con seguimiento
				 {state=14;draw_flag = 1;screen_flag=0;}
				else if(DetectRectangle(menu_rect[2])) //Boton 3 Atras
				 {state=2;draw_flag = 1;screen_flag=0;}
			 }
			 screen_flag=0;
}

//Estado deteccion con seguimiento
void Deteccion_Seguimiento(void)
{
	
	
	if(draw_flag==1)
	{
		read_type=1;
	
	modo=0;  //activar flag de seguimiento con la camara
	pass=1;
	//Ventana total de impresion de la camara, si se toca la pantalla se regresa al menu camara
	//dar valor a read para deteccion
	delay_only = 1;
	delay_us (500000);   

	while (delay_only == 1) {}
		draw_flag=0;
	}
		
	Ptr=Read_Ads7846();
				
	if(Ptr!=(void*)0){		 // If detected pulse
			getDisplayPoint(&display, Ptr, &matrix ) ;	// display.x and display.y of detection
			screen_flag=1;	
		}
		else{return;}
		
		if(screen_flag==1)
		{state=12;draw_flag=1;screen_flag=0;modo =1;}
		screen_flag=0;
			
}

//Estado deteccion sin seguimiento
void Deteccion_no_Seguimiento(void)
{

	if(draw_flag==1)
	{
			read_type=1;
	modo=1;//manual, no movimiento
	pass=1;
	//Ventana total de impresion de la camara, si se toca la pantalla se regresa al menu camara
	//dar valor a read para deteccion
	delay_only = 1;
	delay_us (500000);   

	while (delay_only == 1) {}
		draw_flag=0;
	}
		
	Ptr=Read_Ads7846();
				
	if(Ptr!=(void*)0){		 // If detected pulse
			getDisplayPoint(&display, Ptr, &matrix ) ;	// display.x and display.y of detection
			screen_flag=1;	
		}
		else{return;}
		
		if(screen_flag==1)
		{state=12;draw_flag=1;screen_flag=0;}
		screen_flag=0;
			
}
//Estado menu juegos
void Menu_juegos(void)
{
	if(draw_flag==1)
				{
					pass=0;
					Window(0,0,319,239);
				LCD_Clear(Blue);
				GUI_Text(15,17,"JUEGOS",Black,Blue);
				GUI_Text(40,51,"Pong",Black,Blue);
				GUI_Text(40,85,"Space Invaders",Black,Blue);
				GUI_Text(40,119,"Caramel CRUSH",Black,Blue);
				
				
					// Ponemos a 0 el numero de botones y barras en el menu
	  	  num_botton_menu = 0;   
			  num_bars_menu = 0;	 
			  type_bar = 0;
			
					// Dibujamos primero todos los botones que crearan su rectangulo asociado
					DrawCross(20,51,Black);
					DrawCross(20,85,Black);
					DrawCross(20,119,Black);
					
					TextButton(240,210,"Atras",Black);
					
					draw_flag=0;
					delay_only = 1;
					delay_us (500000);   

					while (delay_only == 1) {}
				}
				
				Ptr=Read_Ads7846();
				
				if(Ptr!=(void*)0){		 // If detected pulse
				getDisplayPoint(&display, Ptr, &matrix ) ;	// display.x and display.y of detection
				screen_flag=1;	
				}
				else{return;}
				
				if(screen_flag==1)   {				// Si hemos tocado
				Window(0,0,319,239);						
					
				/// ACCION QUE PROGRAMEMOS A LOS BOTONES 
				if(DetectRectangle(menu_rect[0])) //Boton 1 Pong
				 {state=24;screen_flag=0;draw_flag = 1;
					newGame = 1;
					read_type = 3;
					pass =1;
					game = 1;
					periodic_signal2_ms(100); 
				 }
			  else if(DetectRectangle(menu_rect[1])) //Boton 2 Space Invaders
				 {state=24;draw_flag = 1;screen_flag=0;
				 
					newGame = 1;
					read_type = 3;
					pass =1;
					game = 2;
					periodic_signal2_ms(100); 
				 
				 }
				else if(DetectRectangle(menu_rect[2])) //Boton 3 Caramel Crash
				 {state=24;draw_flag = 1;screen_flag=0;
					newGame = 1;
					pass =0;
					game = 3; 
					periodic_signal2_ms(100); 				 
				 
				 }
				 else if(DetectRectangle(menu_rect[3])) //Boton 4 Atras
				 {state=1;draw_flag = 1;screen_flag=0;}
				 
			 }
			 screen_flag=0;
}								   
//Estado jugando
void Jugando(void){
	if(draw_flag==1)
				{
					menu_rect[0].x = 0;
					menu_rect[0].y = 0;
					menu_rect[0].w = 40;
					menu_rect[0].h = 50;

					draw_flag=0;
					
					delay_only = 1;
					delay_us (500000);   
					while (delay_only == 1);
					
				}

			 	screen_flag=0;	


		Ptr = Read_Ads7846();
		if(Ptr!=(void*)0){		 // If detected pulse
				getDisplayPoint(&display, Ptr, &matrix ) ;	// display.x and display.y of detection
				screen_flag=1;	
				if(DetectRectangle(menu_rect[0])){
				   	pass =0;
					game = 0; 
					periodic_signal2_ms(0); 
				   	state=23;draw_flag = 1;screen_flag=0;

					}
				}
				
	
}
