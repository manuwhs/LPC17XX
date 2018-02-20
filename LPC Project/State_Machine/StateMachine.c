#include "States.h"

void State_Machine(void)
{
		
	switch(state)
		{
			case 1:   //ESTADO MENU PRINCIPAL
			{
				Menu_Principal();
				break;
			}
			case 2:		//ESTADO MENU CAMARA
			{
				Menu_Camara();
				break;
			}
			case 3:   //ESTADO MENU MOTORES
			{
				Menu_Motores();
				break;
			}
			case 4:   //ESTADO CONFIGURAR 1
			{
				Config_1();
				break;
			}
			case 5:    //ESTADO CONFIGURAR 2
			{
				Config_2();
				break;
			}
			case 6:    //ESTADO CONFIGURAR RGB
			{
				RGB_Configuration();
				break;
			}
			case 7:   //ESTADO CONFIGURAR LUZ
			{
				Light_Configuration();
				break;
			}
			case 8:    //ESTADO CONFIGURAR UMBRAL Y CAPTURA
			{
				Config_Umbral_Captura();
				break;
			}
			case 9:    //ESTADO ELEGIR FPS
			{
				Choose_FPS();
				break;
			}
			case 10:    //ESTADO MOSTRAR IP
			{
				Show_IP();
				break;
			}
			case 11:    //ESTADO MOSTRAR CONFIGURACION ACTUAL
			{
				Show_Actual_Config();
				break;
			}
			case 12:   //ESTADO MENU DETECCION
			{
				Menu_Deteccion();
				break;
			}
			case 13:	//ESTADO MANUAL VS AUTO
			{
				Manual_vs_Auto();
				break;
			}
			case 14:	//ESTADO DETECCION CON SEGUIMIENTO
			{
				Deteccion_Seguimiento();
				break;
			}
			case 15:	//ESTADO DETECCION SIN SEGUIMIENTO
			{
				Deteccion_no_Seguimiento();
				break;
			}
			case 16:	//ESTADO MOSTRAR IMAGEN CAMARA
			{
				Camera_Image();
				break;
			}
			case 17:	//ESTADO CONFIGURAR MOTORES
			{
				Config_Mov_Motores();
				break;
			}
			case 18:	//ESTADO GIRO MANUAL MOTORES
			{
				Giro_Manual_Camara();
				break;
			}
			case 19:  //ESTADO ELEGIR MODO
			{
				Elegir_modo_control();
				break;
			}
			case 20:  //ESTADO CONFIG VARIABLES DE CONTROL
			{
				Config_var_control();
				break;
			}
			case 21:   //ESTADO 
			{
				Config_control_hor();
				break;
			}
			case 22:
			{
				Config_control_ver();
				break;
			}
			case 23: //estado menu juegos
			{
				Menu_juegos();
				break;
			}
			case 24: //estado jugando
			{
				Jugando();
				break;
			}
			default:break;
		}


}