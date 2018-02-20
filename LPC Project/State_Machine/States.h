#include "LPC17xx.h"
#include "../Touch_Pannel/TouchPanel.h"
extern int state;
extern int draw_flag,screen_flag,state_in_process;
extern	Rectangle menu_rect[20]; 
extern	uint8_t num_botton_menu;   
extern	uint8_t num_bars_menu;	  
extern	uint32_t type_bar;	

void State_Machine(void);
/* Private function prototypes -----------------------------------------------*/
void Menu_Principal(void);
void Menu_Camara(void);
void RGB_Configuration(void);
void Menu_Motores(void);
void Config_1(void);
void Config_2(void);
void Light_Configuration(void);
void Config_Umbral_Captura(void);
void Manual_vs_Auto(void);
void Show_Actual_Config(void);
void Menu_Deteccion(void);
void Choose_FPS(void);
void Giro_Manual_Camara(void);
void Show_IP(void);
void Deteccion_Seguimiento(void);
void Deteccion_no_Seguimiento(void);
void Camera_Image(void);
void Config_Mov_Motores(void);
void Elegir_modo_control(void);
void Config_var_control(void);
void Config_control_hor(void);
void Config_control_ver(void);
void Menu_juegos(void);
void Jugando(void);
