#include "GLCD.h"
#include "AsciiLib.h"

#define  SPFD5408B  5  /* 0x5408 */

/*******************************************************************************
* Function Name  : Lcd_Configuration
* Description    : Configures LCD Control lines
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void LCD_Configuration(void)
{
	/* Configure the LCD Control pins */
    PINSEL_CFG_Type PinCfg;

	/* DB[0.7] = P2.0...P2.7 */ 
	PinCfg.Funcnum = 0;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 2;
	PinCfg.Pinnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 1;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 2;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 3;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 4;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 5;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 6;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 7;
	PINSEL_ConfigPin(&PinCfg);
	/* Set Output */
    GPIO_SetDir(2, (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6) | (1<<7), 1);

	/* DB[8.15]= P0.15...P0.22 */
	PinCfg.Funcnum = 0;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 2;
	PinCfg.Pinnum = 15;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 16;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 17;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 18;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 19;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 20;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 21;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 22;
	PINSEL_ConfigPin(&PinCfg);
	/* Set Output */
    GPIO_SetDir(0, (1<<15) | (1<<16) | (1<<17) | (1<<18) | (1<<19) | (1<<20) | (1<<21) | (1<<22), 1);
	
	/*RS = P1.27, WR = P1.28, RD = P1.29*/
	PinCfg.Funcnum = 0;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 1;
	PinCfg.Pinnum = 27;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 28;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 29;
	PINSEL_ConfigPin(&PinCfg);
	/* Set Output */
    GPIO_SetDir(1, (1<<27) | (1<<28) | (1<<29), 1);

	/*CSA = P2.8 */
	PinCfg.Funcnum = 0;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 2;
	PinCfg.Pinnum = 8;
	PINSEL_ConfigPin(&PinCfg);
	/* Output */
    GPIO_SetDir(2, (1<<8), 1);
}

/*******************************************************************************
* Function Name  : LCD_Send
* Description    : 
* Input          : - byte: byte to be sent
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static __attribute__((always_inline)) void LCD_Send (uint16_t byte) 
{
	uint32_t temp;
	temp = byte;

	LPC_GPIO2->FIOPIN =  temp & 0x000000ff;        /* Write D0..D7 */
	LPC_GPIO0->FIOPIN =  (temp << 7) & 0x007F8000; /* Write D8..D15 */
}

/*******************************************************************************
* Function Name  : LCD_Read
* Description    : 
* Input          : - byte: byte to be read
* Output         : None
* Return         : 
* Attention		 : None
*******************************************************************************/
static __attribute__((always_inline)) uint16_t LCD_Read (void) 
{
	uint32_t high;
	uint16_t low;
	
	LPC_GPIO2->FIODIR &= ~(0x000000FF);             /* P2.0...P2.7   Input DB[0..7] */
	LPC_GPIO0->FIODIR &= ~(0x007F8000); 			/* P0.15...P0.22 Input DB[8..15]*/
	low  = LPC_GPIO2->FIOPIN & 0x000000ff;          /* Read D0..D7 */
	high = LPC_GPIO0->FIOPIN & 0x007f8000;          /* Read D8..D15 */
	low |= (high >> 7);
	
	LPC_GPIO2->FIODIR |= 0x000000FF;                /* P2.0...P2.7   Output DB[0..7] */
	LPC_GPIO0->FIODIR |= 0x007F8000; 			    /* P0.15...P0.22 Output DB[8..15]*/
	
	return  low;
}

/*******************************************************************************
* Function Name  : LCD_WriteIndex
* Description    : 
* Input          : - index: 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static __attribute__((always_inline)) void LCD_WriteIndex(uint16_t index)
{
	LCD_CS(0);
	LCD_RS(0);
	LCD_RD(1);
	LCD_Send( index ); 
	LCD_WR(0);  
	LCD_WR(1);
	LCD_CS(1);
}

/*******************************************************************************
* Function Name  : LCD_WriteData
* Description    : 
* Input          : - index: 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static __attribute__((always_inline)) void LCD_WriteData(uint16_t data)
{				
	LCD_CS(0);
	LCD_RS(1);   
	LCD_Send( data );
	LCD_WR(0);     
	LCD_WR(1);
	LCD_CS(1);
}

/*******************************************************************************
* Function Name  : LCD_ReadData
* Description    : 
* Input          : None
* Output         : None
* Return         : 
* Attention		 : None
*******************************************************************************/
static __attribute__((always_inline)) uint16_t LCD_ReadData(void)
{ 
	uint16_t value;
	
	LCD_CS(0);
	LCD_RS(1);
	LCD_WR(1);
	LCD_RD(0);
	value = LCD_Read();
	
	LCD_RD(1);
	LCD_CS(1);
	
	return value;
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
 void LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue)
{ 
	/* Write 16-bit Index, then Write Reg */  
	LCD_WriteIndex(LCD_Reg);         
	/* Write 16-bit Reg */
	LCD_WriteData(LCD_RegValue);  
}
/*******************************************************************************
* Function Name  : LCD_ReadReg
* Description    : Reads the selected LCD Register.
* Input          : None
* Output         : None
* Return         : LCD Register Value.
* Attention		 : None
*******************************************************************************/
static __attribute__((always_inline)) uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{
	uint16_t LCD_RAM;
	
	 //Write 16-bit Index (then Read Reg) 
	LCD_WriteIndex(LCD_Reg);
	 //Read 16-bit Reg 
	LCD_RAM = LCD_ReadData();      	
	return LCD_RAM;
}

/*******************************************************************************
* Function Name  : LCD_SetCursor
* Description    : Sets the cursor position.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position. 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
 void LCD_SetCursor(uint16_t Xpos,uint16_t Ypos)
{

	/* 0x9320 0x9325 0x5408 0x1505 0x0505 0x7783 0x4531 0x4535 */
          LCD_WriteReg(0x0020, Ypos );     	  // Horizontal -> 0-239	  Eje Y pa nosotros
          LCD_WriteReg(0x0021, MAX_X -1 - Xpos  );   // Vertical -> 0-319		  Eje X pa nosotros
	      // Estamos haciendo el cambio de coordenadas para poner el eje (0,0) arriba a la izda.
}

/*******************************************************************************
* Function Name  : delay_ms
* Description    : Delay Time
* Input          : - nCount: Delay Time
* Output         : None
* Return         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void delay_ms(uint16_t ms)    
{ 
	uint16_t i,j; 
	for( i = 0; i < ms; i++ )
	{ 
		for( j = 0; j < 1141; j++ );
	}
} 

/*******************************************************************************
* Function Name  : LCD_Initializtion
* Description    : Initialize TFT Controller.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_Initialization(void)
{

	
	LCD_Configuration();
	delay_ms(100);

		
		LCD_WriteReg(0x0001,0x0100);	  /* Driver Output Control Register */ 
		LCD_WriteReg(0x0002,0x0600);    /* LCD Driving Waveform Control */
		LCD_WriteReg(0x0003,0x1018);	  /* Entry Mode */	//   ------------------> 1
															//	 |
															//	 |
															//	\/
															//	 2
								// Decrementar horizontal suyo e incrementar vertical
		LCD_WriteReg(0x0004,0x0000);	  /* Scalling Control register */
		LCD_WriteReg(0x0008,0x0207);	  /* Display Control 2 */
		LCD_WriteReg(0x0009,0x0000);	  /* Display Control 3 */
		LCD_WriteReg(0x000A,0x0000);	  /* Frame Cycle Control */
		LCD_WriteReg(0x000C,0x0000);	  /* External Display Interface Control 1 */
		LCD_WriteReg(0x000D,0x0000);    /* Frame Maker Position */
		LCD_WriteReg(0x000F,0x0000);	  /* External Display Interface Control 2 */
		delay_ms(50);
		LCD_WriteReg(0x0007,0x0100);	  /* Display Control */
		delay_ms(50);
		LCD_WriteReg(0x0010,0x16B0);    /* Power Control 1 */
		LCD_WriteReg(0x0011,0x0001);    /* Power Control 2 */
		LCD_WriteReg(0x0017,0x0001);    /* Power Control 3 */
		LCD_WriteReg(0x0012,0x0138);    /* Power Control 4 */
		LCD_WriteReg(0x0013,0x0800);    /* Power Control 5 */
		LCD_WriteReg(0x0029,0x0009);	  /* NVM read data 2 */
		LCD_WriteReg(0x002a,0x0009);	  /* NVM read data 3 */
		LCD_WriteReg(0x00a4,0x0000);  
		LCD_WriteReg(0x0050,0x0000);	  /* 38 */
		LCD_WriteReg(0x0051,0x00EF);	  /* B8 */
		LCD_WriteReg(0x0052,0x0000);	  /* 60 */
		LCD_WriteReg(0x0053,0x013F);	  /* E0 */
		   
		LCD_WriteReg(0x0060,0x2700);	  /* Driver Output Control */
										 
		LCD_WriteReg(0x0061,0x0003);	  /* Driver Output Control */
		LCD_WriteReg(0x006A,0x0000);	  /* Vertical Scroll Control */
		
		LCD_WriteReg(0x0080,0x0000);	  /* Display Position ®C Partial Display 1 */
		LCD_WriteReg(0x0081,0x0000);	  /* RAM Address Start ®C Partial Display 1 */
		LCD_WriteReg(0x0082,0x0000);	  /* RAM address End - Partial Display 1 */
		LCD_WriteReg(0x0083,0x0000);	  /* Display Position ®C Partial Display 2 */
		LCD_WriteReg(0x0084,0x0000);	  /* RAM Address Start ®C Partial Display 2 */
		LCD_WriteReg(0x0085,0x0000);	  /* RAM address End ®C Partail Display2 */
		LCD_WriteReg(0x0090,0x0013);	  /* Frame Cycle Control */
		LCD_WriteReg(0x0092,0x0000); 	  /* Panel Interface Control 2 */
		LCD_WriteReg(0x0093,0x0003);	  /* Panel Interface control 3 */
		LCD_WriteReg(0x0095,0x0110);	  /* Frame Cycle Control */
		LCD_WriteReg(0x0007,0x0173);
		
}


/*******************************************************************************
* Function Name  : LCD_Clear
* Description    : 
* Input          : - Color: Screen Color
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_Clear(uint16_t Color)
{
	uint32_t index;
	LCD_SetCursor(0,0);
	LCD_WriteIndex(0x0022);
	for( index = 0; index < MAX_X * MAX_Y; index++ )
	{
		LCD_WriteData(Color);
	}
}
 void window_clear(uint16_t len_x,uint16_t len_y,uint16_t Color)
{
	uint32_t index;
	LCD_WriteIndex(0x0022);
	for( index = 0; index < len_x * len_y; index++ )
	{
		LCD_WriteData(Color);
	}
}




void draw_RGB565_image (uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *image){
   uint32_t i;

   Window (x,y,x+w-1,y+h-1);
   LCD_SetCursor(x,y);
   //Window(0,0,319,239);

   LCD_WriteIndex(0x0022);
   for (i = 0; i < w*h; i++){
			LCD_WriteData((image[2*i] << 8)|(image[2*i+1] << 0));

   /*
   	 		LPC_GPIO2->FIOPIN =  image[2*i] & 0x000000ff;        // Write D0..D7 
			LPC_GPIO0->FIOPIN =  (image[2*i+1] << 15) & 0x007F8000; // Write D8..D15 

		
			LPC_GPIO1->FIOCLR = PIN_WR;
			LPC_GPIO1->FIOSET = PIN_WR;

			*/

	}

	Window(0,0,319,239);
 }



/******************************************************************************
* Function Name  : LCD_BGR2RGB
* Description    : 
* Input          : - color: BRG   
* Output         : None
* Return         : RGB 
* Attention		 : 
*******************************************************************************/
static uint16_t LCD_BGR2RGB(uint16_t color)
{
	uint16_t  r, g, b, rgb;
	
	b = ( color>>0 )  & 0x1f;
	g = ( color>>5 )  & 0x3f;
	r = ( color>>11 ) & 0x1f;
	
	rgb =  (b<<11) + (g<<5) + (r<<0);
	
	return( rgb );
}

/******************************************************************************
* Function Name  : LCD_GetPoint
* Description    : ªÒ»°÷∏∂®◊˘±Íµƒ—’…´÷µ
* Input          : - Xpos: Row Coordinate
*                  - Xpos: Line Coordinate 
* Output         : None
* Return         : Screen Color
* Attention		 : None
*******************************************************************************/
uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos)
{
	uint16_t dummy;
	
	LCD_SetCursor(Xpos,Ypos);
	LCD_WriteIndex(0x0022);  
	
		/* 0x9320 0x9325 0x5408 0x1505 0x0505 */
             dummy = LCD_ReadData();   /* Empty read */
             dummy = LCD_ReadData(); 	
 		     return  LCD_BGR2RGB( dummy );
	
}

/******************************************************************************
* Function Name  : LCD_SetPoint
* Description    : ‘⁄÷∏∂®◊˘±Íª≠µ„
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point)
{
	if( Xpos >= MAX_X || Ypos >= MAX_Y )
	{
		return;
	}
	LCD_SetCursor(Xpos,Ypos);
	LCD_WriteReg(0x0022,point);
}

/******************************************************************************
* Function Name  : PutChar
* Description    : Ω´Lcd∆¡…œ»Œ“‚Œª÷√œ‘ æ“ª∏ˆ◊÷∑˚
* Input          : - Xpos: ÀÆ∆Ω◊¯±Í 
*                  - Ypos: ¥π÷±◊¯±Í  
*				   - ASCI: œ‘ æµƒ◊÷∑˚
*				   - charColor: ◊÷∑˚—’…´   
*				   - bkColor: ±≥æ∞—’…´ 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void PutChar( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor )
{
	uint16_t i, j;
    uint8_t buffer[16], tmp_char;
    GetASCIICode(buffer,ASCI);  /* »°◊÷ƒ£ ˝æ› */
	if (text_mode == 0) {

/*
	  ********  This is the original way the GLCD coordinates work *********

							     VERTICAL
	MAX_X -1	 <---------------<---------------------<------------------<----  (0,0)
 	---------------------------------------------------------------------------
	|						  Eje X 										  |	   |
	|				0	|-----------------------|							  |	   |
	|				1	|						|							  |	   |
	|				2	|	   _  _  _  _		|							  |	   |
	|				3	|	 |					|	  E						  |	  \	/
	|				4	|	 |					|	  J						  |	   |   H
	|				5	|	 |					|	  E						  |	   |   O
	|				6	|	 | _  _  _  _		|							  |	   |   R
	|				7	|	 |					|	  Y						  |	   |   I
	|				8	|	 |					|							  |	   |   Z
	|				9	|	 |					|							  |	   |   O
	|				10	|	 |					|							  |	  \	/  N
	|				11	|	 |					|							  |	   |   T
	|				12	|	 |					|							  |	   |   A
	|				13	|						|							  |	   |   L
	|				14	|						|							  |	  \	/
	|				15	| _  _  _  _  _  _  _  _|							  |	   '
	|					  0	 1	2  3  4	 5	6  7							  |
	|																		  | MAX_Y - 1
	---------------------------------------------------------------------------
  ***************************************************************************************
  **** We will convert it to this coordinates by doing the simple swap MAX_X - pos_x ****
  ***************************************************************************************
							     VERTICAL
 (0,0) --------------->--------------------->-------------------------> 	  MAX_X -1
 	---------------------------------------------------------------------------
	|						  Eje X 										  |	   |
	|				0	|-----------------------|							  |	   |
	|				1	|						|							  |	   |
	|				2	|	   _  _  _  _		|							  |	   |
	|				3	|	 |					|	  E						  |	  \	/
	|				4	|	 |					|	  J						  |	   |   H
	|				5	|	 |					|	  E						  |	   |   O
	|				6	|	 | _  _  _  _		|							  |	   |   R
	|				7	|	 |					|	  Y						  |	   |   I
	|				8	|	 |					|							  |	   |   Z
	|				9	|	 |					|							  |	   |   O
	|				10	|	 |					|							  |	  \	/  N
	|				11	|	 |					|							  |	   |   T
	|				12	|	 |					|							  |	   |   A
	|				13	|						|							  |	   |   L
	|				14	|						|							  |	  \	/
	|				15	| _  _  _  _  _  _  _  _|							  |	   '
	|					  0	 1	2  3  4	 5	6  7							  |
	|																		  |	 MAX_Y - 1
	---------------------------------------------------------------------------

  Be aware that Åg00Åhh . VSA8-0< VEA8-0 . 9Åfh13F
*/

	    for( i=0; i<16; i++ )		 /* Por cada columna larga " i = eje Y"*/
	    {
	        tmp_char = buffer[i];
	        for( j=0; j<8; j++ )	  /* Por cada fila corta " j = eje X"*/
	        {
	            if( (tmp_char >>(7- j)) & (0x01 == 0x01) )
	            {
	                LCD_SetPoint( Xpos + j, Ypos + i, charColor );  /* ◊÷∑˚—’…´ */
	            }
	            else
	            {
	                LCD_SetPoint( Xpos + j, Ypos + i, bkColor );  /* ±≥æ∞—’…´ */
	            }
	        }
	    }
	}

		if (text_mode == 1) {
	    for( i=0; i<16; i++ )
	    {
	        tmp_char = buffer[i];
	        for( j=0; j<8; j++ )
	        {
	            if( (tmp_char >> (7 - j)) & (0x01 == 0x01) )
	            {
	                LCD_SetPoint( Xpos - i, Ypos + j, charColor );  /* ◊÷∑˚—’…´ */
	            }
	            else
	            {
	                LCD_SetPoint( Xpos - i, Ypos + j, bkColor );  /* ±≥æ∞—’…´ */
	            }
	        }
	    }
	}
}










void PutFigure( uint16_t Xpos, uint16_t Ypos, uint16_t num_fig, uint16_t charColor, uint16_t bkColor,uint8_t transp){
	uint16_t i, j,size = 2, s1,s2;
    uint16_t *buffer = &size, tmp_char;		// CAMBIAR LO DE BUFER !!!!
    buffer =(uint16_t *) figures[num_fig];

	if (text_mode == 0) {

	    for( i=0; i<8; i++ )		 /* Por cada columna larga " i = eje Y"*/
	    {

	        tmp_char = buffer[i];
		for( s1=0; s1<size; s1++ ){
			
	        for( j=0; j<16; j++ ){	  /* Por cada fila corta " j = eje X"*/
	        
	          for( s2=0; s2<size; s2++ ){   
			    if( (tmp_char >>(15- j)) & (0x01 == 0x01) )
	            {
	                LCD_SetPoint( Xpos + size*j+s2, Ypos + (size*i+s1), charColor );  /* ◊÷∑˚—’…´ */
	            }
	            else
	            {
	                LCD_SetPoint( Xpos + size*j+s2, Ypos + (size*i+s1), bkColor );  /* ±≥æ∞—’…´ */
	            }
	         }
			}

	       } 
		}
	}

	if (text_mode == 1) {

	    for( i=0; i<8; i++ )		 /* Por cada columna larga " i = eje Y"*/
	    {

	        tmp_char = buffer[i];
		for( s1=0; s1<size; s1++ ){
			
	        for( j=0; j<16; j++ ){	  /* Por cada fila corta " j = eje X"*/
	        
	          for( s2=0; s2<size; s2++ ){   
			    if( (tmp_char >>(15- j)) & (0x01 == 0x01) )
	            {
	                LCD_SetPoint( Xpos - (size*i+s1), Ypos + size*j+s2, charColor );  /* ◊÷∑˚—’…´ */
	            }
	            else
	            {
	                LCD_SetPoint( Xpos - (size*i+s1), Ypos +  size*j+s2, bkColor );  /* ±≥æ∞—’…´ */
	            }
	         }
			}

	       } 
		}
	}
}
/******************************************************************************
* Function Name  : GUI_Text
* Description    : ‘⁄÷∏∂®◊˘±Íœ‘ æ◊÷∑˚¥Æ
* Input          : - Xpos: ––◊˘±Í
*                  - Ypos: ¡–◊˘±Í 
*				   - str: ◊÷∑˚¥Æ
*				   - charColor: ◊÷∑˚—’…´   
*				   - bkColor: ±≥æ∞—’…´ 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor)
{
    uint8_t TempChar;
    do
    {


	  	 TempChar = *str++;  
	     PutChar( Xpos, Ypos, TempChar, Color, bkColor );  

		if (text_mode == 0) {
/*
	Cada vez excibamos un caracter disminuimos el eje X en 8 y aumentamos el Y en 16 
*/		  
	        if(( Xpos <= (MAX_X-1) - 8 )&&( Ypos <= (MAX_Y-1) - 16 )){		 // Si no se va a ir por la decha ni abajo
	            Xpos += 8;
	        } 
	        else if (Xpos > (MAX_X-1) - 8 ){	   			// Si se va a ir por la derecha
	        
	            Xpos = 0;
	            Ypos += 16;
	        }

	        if ( Ypos > (MAX_Y-1) - 16 ){	   			// Si se va a ir por abajo
	        
	            Ypos = 0;
	        }
    	}

		if (text_mode == 1) {
/*
	Cada vez excibamos un caracter disminuimos el eje X en 8 y aumentamos el Y en 16 
*/
		 // 									 abajo
	        if(( Xpos <= (MAX_X-1) - 16 )&&( Ypos <= (MAX_Y-1) - 8 )){ // Si no se va a ir por la izda ni abajo
	            Ypos += 8;
	        } 
	        else if ( Ypos > (MAX_Y-1) - 8 ){	   			// Si se va a ir por abajo
	        
	            Ypos = 0;
	            Xpos -= 16;
	        }

	        if ( Xpos > (MAX_X-1) - 16 ){	   			// Si se va a ir por la izda
	        
	            Xpos = (MAX_X-1);
	        }
    	}
	}
    while ( *str != 0 );
}

/******************************************************************************
* Function Name  : _LCD_DrawImage
* Description    : ‘⁄÷∏∂®◊˘±Íœ‘ æ◊÷∑˚¥Æ
* Input          : *Image: ––◊˘±Í
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_DrawImage(uint16_t *Image)
{

	uint16_t index;
	
		LCD_WriteReg(0x0050,0x0037);	  /* 38 */
		LCD_WriteReg(0x0051,0x00B6);	  /* B8 */
		LCD_WriteReg(0x0052,0x005F);	  /* 60 */
		LCD_WriteReg(0x0053,0x00DF);	  /* E0 */
	

	
	for(index=0;index<15000;index++)
	{	
//		LCD_WriteIndex(0x0022);
//	
//		LCD_WriteData(Image[index]);
		LCD_WriteReg(0x0022,Image[index]);
	}

}

/******************************************************************************
* Function Name  : LCD_DrawLine
* Description    : Bresenham's line algorithm
* Input          : - x1: Aµ„––◊˘±Í
*                  - y1: Aµ„¡–◊˘±Í 
*				   - x2: Bµ„––◊˘±Í
*				   - y2: Bµ„¡–◊˘±Í 
*				   - color: œﬂ—’…´
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/	 
void LCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color )
{
    short dx,dy;      
    short temp;
		int caso=0;

    if( (x0<x1) & (y0<y1))     
    {caso=1;}
    else if( (x0>x1) & (y0<y1))     
    {caso=2;
		temp = x1;
		x1 = x0;
		x0 = temp;
		temp = y1;
		y1 = y0;
		y0 = temp;}
		else if( (x0<x1) & (y0>y1))
		{caso=2;}
		else if ( (x0>x1) & (y0>x1))
		{caso=1;
		temp = x1;
		x1 = x0;
		x0 = temp;
		temp = y1;
		y1 = y0;
		y0 = temp; }
		
		if( x0 == x1 )     /* X÷·…œ√ª”–‘ˆ¡ø ª≠¥π÷±œﬂ */ 
    {
			if(y0<y1)
			{
        do
        { 
            LCD_SetPoint(x0, y0, color);   /* ÷µ„œ‘ æ √Ë¥π÷±œﬂ */
            y0++;
        }
        while( y1 >= y0 ); 
			}
			else
			{
        do
        { 
            LCD_SetPoint(x0, y0, color);   /* ÷µ„œ‘ æ √Ë¥π÷±œﬂ */
            y0--;
        }
        while( y0 >= y1 ); 
			}
		return; 
    }
    if( y0 == y1 )     /* Y÷·…œ√ª”–‘ˆ¡ø ª≠ÀÆ∆Ω÷±œﬂ */ 
    {
			if(x0<x1)
			{
        do
        {
            LCD_SetPoint(x0, y0, color);   /* ÷µ„œ‘ æ √ËÀÆ∆Ωœﬂ */
            x0++;
        }
        while( x1 >= x0 ); 
			}
			else
			{
				do
        {
            LCD_SetPoint(x0, y0, color);   /* ÷µ„œ‘ æ √ËÀÆ∆Ωœﬂ */
            x0--;
        }
        while( x0 >= x1 );
			}
		return;
    }
  
		switch(caso)
		{
			case 1:	//x0<x1 & y0<y1 Û (x0>x1) & (y0>x1)
				{
					dx = x1-x0;       /* X÷·∑ΩœÚ…œµƒ‘ˆ¡ø */
					dy = y1-y0;       /* Y÷·∑ΩœÚ…œµƒ‘ˆ¡ø */
					
					/* ≤º¿º…≠∫∫ƒ∑(Bresenham)À„∑®ª≠œﬂ */
					if( dx > dy )                         /* øøΩ¸X÷· */
						{
							temp = 2 * dy - dx;               /* º∆À„œ¬∏ˆµ„µƒŒª÷√ */
							while( x0 != x1 )
								{
									LCD_SetPoint(x0,y0,color);    /* ª≠∆µ„ */ 
									x0++;                         /* X÷·…œº”1 */
									if( temp > 0 )                /* ≈–∂œœ¬œ¬∏ˆµ„µƒŒª÷√ */
									{
											y0++;                     /* Œ™”“…œœ‡¡⁄µ„£¨º¥£®x0+1,y0+1£© */ 
											temp += 2 * dy - 2 * dx;
									}
									else
										{
											temp += 2 * dy;           /* ≈–∂œœ¬œ¬∏ˆµ„µƒŒª÷√ */
										}
								}
								LCD_SetPoint(x0,y0,color);
						}
						else
						{
							temp = 2 * dx - dy;                      /* øøΩ¸Y÷· */       
							while( y0 != y1 )
							{
								LCD_SetPoint(x0,y0,color);
								y0++;
								if( temp > 0 )
									{
										x0++;
										temp+=2*dy-2*dx;
									}
								else
									{
										temp += 2 * dy;
									}
							} 
							LCD_SetPoint(x0,y0,color);
						}
						break;
					}
					case 2: // (x0<x1) & (y0>y1) Û (x0>x1) & (y0<y1)
					{
					dx = x1-x0;       /* X÷·∑ΩœÚ…œµƒ‘ˆ¡ø */
					dy = y0-y1;       /* Y÷·∑ΩœÚ…œµƒ‘ˆ¡ø */
					
					/* ≤º¿º…≠∫∫ƒ∑(Bresenham)À„∑®ª≠œﬂ */
					if( dx > dy )                         /* øøΩ¸X÷· */
						{
							temp = 2 * dy - dx;               /* º∆À„œ¬∏ˆµ„µƒŒª÷√ */
							while( x0 != x1 )
								{
									LCD_SetPoint(x0,y0,color);    /* ª≠∆µ„ */ 
									x0++;                         /* X÷·…œº”1 */
									if( temp > 0 )                /* ≈–∂œœ¬œ¬∏ˆµ„µƒŒª÷√ */
									{
											y0--;                     /* Œ™”“…œœ‡¡⁄µ„£¨º¥£®x0+1,y0+1£© */ 
											temp += 2 * dy - 2 * dx;
									}
									else
										{
											temp += 2 * dy;           /* ≈–∂œœ¬œ¬∏ˆµ„µƒŒª÷√ */
										}
								}
								LCD_SetPoint(x0,y0,color);
						}
					else
						{
							temp = 2 * dx - dy;                      /* øøΩ¸Y÷· */       
							while( y0 != y1 )
							{
								LCD_SetPoint(x0,y0,color);
								y0--;
								if( temp > 0 )
									{
										x0++;
										temp+=2*dy-2*dx;
									}
								else
									{
										temp += 2 * dy;
									}
							} 
							LCD_SetPoint(x0,y0,color);
						}
						break;
					}
					default:break;
			}
}


/******************************************************************************
* Function Name  : _LCD_ShowCameraPixel
* Description    : 
* Input          : 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/

void LCD_ShowCameraPixel(uint16_t Color)
{
	
	LCD_WriteReg(0x0022,Color);
	
}
