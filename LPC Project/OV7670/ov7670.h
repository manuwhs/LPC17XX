#ifndef __OV7670_H 
#define __OV7670_H
#include "../Touch_Pannel/TouchPanel.h"
// #include "type.h"
#include "ov7670reg.h"
#include "i2c.h"

#define OV7670_ADDR     0x42

#define QVGA_HEIGHT 240
#define QVGA_WIDTH 320

#define HEIGHT_ZONES 12			/* Zones in which we divide the screen height */
#define WIDTH_ZONES 16			/* Zones in which we divide the screen width */

#define SCN_ZONES HEIGHT_ZONES*WIDTH_ZONES
#define PIX_H_ZONE QVGA_HEIGHT/HEIGHT_ZONES			/* Zones in which we divide the screen width */
#define PIX_W_ZONE QVGA_WIDTH/WIDTH_ZONES	


#define SSCB 1

#define RED_MASK 0x001F
#define GREEN_MASK 0x03F
#define BLUE_MASK 0x001F

#define	RGB_565_read   	 0x00
#define	RGB_565_detect   0x01


uint32_t ov7670_set(uint8_t reg_OV, uint8_t value);
uint32_t ov7670_read(uint8_t reg_OV);
void Init_OV7670 (void);

void read_window_RGB565 (uint16_t Vw, uint16_t Vh);
void detect_read_frame_RGB565 (uint8_t algorithm);
void detect_window_RGB565 (uint16_t Vw, uint16_t Vh, uint8_t algorithm);
void montwell_detect (void);
void masscentre_detect (void);
void show_blocks (void);
void read_frame_RGB565_b (void);
void photo_frame_RGB565 (void);
void read_frame_RGB565 (void);

void OV7670_windowing (uint16_t Hi,uint16_t Hf, uint16_t Vi,uint16_t Vf );
void set_Umbral (uint8_t um);
void OV7670_BriConLum (uint8_t  B, uint8_t C, uint16_t L, uint8_t mode);
void AWBC_control (void);
void OV7670_AWBmode (uint8_t mode);
void OV7670_RGBgain (uint8_t R ,uint8_t G, uint8_t B);
void OV7670_FPS (uint8_t fps);
void resolution (uint8_t num);
void scaler (uint8_t XSC, uint8_t YSC);
void timing_resol (uint8_t num1, uint8_t num2, uint8_t num3);
void OV7670_mirror (uint8_t mirr);

extern volatile uint32_t frame_read_delay_us;
extern uint32_t pass;	// Read or not to read from camera		   
extern uint32_t fps;  // Frames per second
extern uint32_t fps_aux;  // Frames per second
extern uint8_t read_type;	// Kind of reading
extern uint8_t fps_divider;	// FPS we want
extern uint8_t take_picture;	// FPS we want
extern uint8_t show_picture;	// FPS we want

extern uint16_t Vd_OV7670;	// FPS we want
extern uint16_t Vx_OV7670;	// FPS we want
extern uint16_t const_block;	// FPS we want
extern uint8_t distances[HEIGHT_ZONES][WIDTH_ZONES];
extern char delay_read[10];


extern uint32_t num;
extern uint16_t color_detect;			 /* Color a detectar */
extern uint8_t cd_R;	 /* Valores de RGV del color a detectar */
extern uint8_t cd_G;
extern uint8_t cd_B;
extern uint8_t average;						 /* For converting to grayscale */
extern uint8_t umbral;					 /* Umbral del color a detectar */

extern uint16_t scn_zone[HEIGHT_ZONES][WIDTH_ZONES];			/* Value of every one of the detection zones */

extern uint32_t det_x;
extern uint32_t det_y;
extern uint32_t total_mass;


extern uint16_t try_QCIF_H;
extern uint16_t try_QCIF_W;
extern uint16_t bgColor;


int min_int(int a, int b);

extern uint8_t OV7670contrast ;
extern uint8_t OV7670bright;
extern uint16_t OV7670luminis;		   /* Luminosidad en modo manual */
extern uint8_t OV7670R_gain, OV7670G_gain, OV7670B_gain;
extern uint8_t OV7670fps;
extern uint8_t resol_type; 	/* Type of resolution VGA, QVGA, QQVGA or QQQVGA */



#endif
