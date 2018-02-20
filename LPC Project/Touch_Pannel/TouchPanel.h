/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               TouchPanel.h
** Descriptions:            The TouchPanel application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

#ifndef _TOUCHPANEL_H_
#define _TOUCHPANEL_H_

/* Includes ------------------------------------------------------------------*/
#include "../LPC17xx_Libraries/lpc17xx_ssp.h"
#include "../LPC17xx_Libraries/lpc17xx_gpio.h"
#include "../LPC17xx_Libraries/lpc17xx_pinsel.h"

/* Private typedef -----------------------------------------------------------*/
typedef	struct 
{
  uint16_t xmin;
	uint16_t xmax;
	uint16_t ymin;
  uint16_t ymax;
}Window_size;


typedef	struct POINT 
{
   uint16_t x;
   uint16_t y;
}Coordinate;


typedef struct Matrix 
{						
long double An,  
            Bn,     
            Cn,   
            Dn,    
            En,    
            Fn,     
            Divider ;
} Matrix ;


typedef	struct {
  uint16_t x;
  uint16_t y;
  uint16_t w;
  uint16_t h;

}Rectangle;



extern Rectangle pl1, pl2, ball,invader, misil[10];
extern	Rectangle menu_rect[20]; 
extern	uint8_t num_botton_menu;   
extern	uint8_t num_bars_menu;	  
extern	uint32_t type_bar;	
extern Coordinate * Ptr;
extern Coordinate * Ptr2;
/* Private variables ---------------------------------------------------------*/
extern Coordinate ScreenSample[3];
extern Coordinate DisplaySample[3];
extern Matrix matrix ;
extern Coordinate  display ;
extern uint16_t Capt_flag;
extern uint16_t Calibration_vars[9];
extern Window_size actual_window;
extern uint16_t Detection_color;

/* Private define ------------------------------------------------------------*/
/* AD通道选择命令字和工作寄存器 */
#define	CHX 	        0x90 	/* 通道Y+的选择控制字 */	
#define	CHY 	        0xd0	/* 通道X+的选择控制字 */
#define norte		1
#define sur			2
#define este		3
#define oeste		4
#define BARS_RANGE	255
//#define umbral 0
#define brillo 1
#define contraste 2
#define luminosidad 3
#define Red_value 4
#define Green_value 5
#define Blue_value 6
#define vert_move 7
#define horiz_move 8
#define vertical 0
#define horizontal 1


/* Physical level marcos */
/* PORT number that TP_IRQ pin assigned on */
#define TP_IRQ_PORT_NUM		2
/* PIN number that  TP_IRQ pin assigned on */
#define TP_IRQ_PIN_NUM		13
/* PORT number that TP_CS pin assigned on */
#define TP_CS_PORT_NUM		0
/* PIN number that  TP_CS pin assigned on */
#define TP_CS_PIN_NUM		6

//#define TP_CS(x)   ((x) ? GPIO_SetValue(TP_CS_PORT_NUM, (1<<TP_CS_PIN_NUM) ) : GPIO_ClearValue(TP_CS_PORT_NUM, (1<<TP_CS_PIN_NUM) ) );
#define TP_CS(x)	((x) ? (LPC_GPIO0->FIOSET = (1<<TP_CS_PIN_NUM)) : (LPC_GPIO0->FIOCLR = (1<<TP_CS_PIN_NUM)));

//#define TP_INT_IN   (GPIO_ReadValue(TP_IRQ_PORT_NUM)&(1<<TP_IRQ_PIN_NUM))
#define TP_INT_IN   ( LPC_GPIO2->FIOPIN & (1<<TP_IRQ_PIN_NUM) ) 

/* Private function prototypes -----------------------------------------------*/				
void TP_Init(void);	
Coordinate *Read_Ads7846(void);
void TouchPanel_Calibrate(void);
void DrawCross(uint16_t Xpos,uint16_t Ypos,uint16_t color);
void TP_DrawPoint(uint16_t Xpos,uint16_t Ypos);
uint8_t setCalibrationMatrix( Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr);
uint8_t getDisplayPoint(Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr );
void DrawArrow(uint16_t xpos,uint16_t ypos,uint16_t color, uint16_t apunta);
void TextButton(uint16_t Xpos, uint16_t Ypos, uint8_t *str, uint16_t color);
void DrawCalibrationBar(uint16_t xmin, uint16_t xmax, uint16_t ymin, uint16_t ymax,uint16_t situation);
void Get_CalibrationValue(uint16_t num_var, Rectangle bar,uint16_t situation);
void UpdateBar(uint16_t num_var, Rectangle bar, uint8_t situation);
uint16_t GetColor(void);
void DrawRectangle(uint16_t xmin, uint16_t ymin, uint16_t len_x, uint16_t len_y,uint16_t color);
uint8_t DetectRectangle(Rectangle r); // return 1 if detect point is inside rectangle
#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
