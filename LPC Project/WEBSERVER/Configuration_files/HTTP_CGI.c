/*----------------------------------------------------------------------------
 *      RL-ARM - TCPnet
 *----------------------------------------------------------------------------
 *      Name:    HTTP_CGI.C
 *      Purpose: HTTP Server CGI Module
 *      Rev.:    V4.22
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/


#include <Net_Config.h>
#include <stdio.h>
#include <string.h>
#include "../../GLCD/GLCD.h"
#include "../../OV7670/ov7670.h"
#include "../../SERVOS/servos.h"
int conversor_SaI(char * cadnum);
/* ---------------------------------------------------------------------------
 * The HTTP server provides a small scripting language.
 *
 * The script language is simple and works as follows. Each script line starts
 * with a command character, either "i", "t", "c", "#" or ".".
 *   "i" - command tells the script interpreter to "include" a file from the
 *         virtual file system and output it to the web browser.
 *   "t" - command should be followed by a line of text that is to be output
 *         to the browser.
 *   "c" - command is used to call one of the C functions from the this file.
 *         It may be followed by the line of text. This text is passed to
 *         'cgi_func()' as a pointer to environment variable.
 *   "#' - command is a comment line and is ignored (the "#" denotes a comment)
 *   "." - denotes the last script line.
 *
 * --------------------------------------------------------------------------*/

/* http_demo.c */
extern U16 AD_in (U32 ch);
extern U8  get_button (void);

/* at_System.c */
extern  LOCALM localm[];
#define LocM   localm[NETIF_ETH]

/* Net_Config.c */
extern struct tcp_cfg   tcp_config;
extern struct http_cfg  http_config;
#define tcp_NumSocks    tcp_config.NumSocks
#define tcp_socket      tcp_config.Scb
#define http_EnAuth     http_config.EnAuth
#define http_auth_passw http_config.Passw

extern BOOL LEDrun;
extern void LED_out (U32 val);
extern BOOL LCDupdate;
extern U8   lcd_text[2][16+1];
extern U8		camara[2][16+1];

/* Local variables. */
static U8 P2;

U8	select_menu=0, BCL=1, RGB=1;
U8	aux_web[40], aux_data_form;
/* My structure of CGI status U32 variable. This variable is private for */
/* each HTTP Session and is not altered by HTTP Server. It is only set to  */
/* zero when the cgi_func() is called for the first time.                  */
typedef struct {
  U16 xcnt;
  U16 unused;
} MY_BUF;
#define MYBUF(p)        ((MY_BUF *)p)

/*----------------------------------------------------------------------------
 * HTTP Server Common Gateway Interface Functions
 *---------------------------------------------------------------------------*/

/*--------------------------- cgi_process_data ------------------------------*/

void cgi_process_data (U8 code, U8 *dat, U16 len) {
  /* This function is called by HTTP server to process the returned Data    */
  /* for the CGI Form POST method. It is called on SUBMIT from the browser. */
  /* Parameters:                                                            */
  /*   code  - callback context code                                        */
  /*           0 = www-url-encoded form data                                */
  /*           1 = filename for file upload (0-terminated string)           */
  /*           2 = file upload raw data                                     */
  /*           3 = end of file upload (file close requested)                */
  /*           4 = any xml encoded POST data (single or last stream)        */
  /*           5 = the same as 4, but with more xml data to follow          */
  /*               Use http_get_content_type() to check the content type    */  
  /*   dat   - pointer to POST received data                                */
  /*   len   - received data length                                         */
  U8 passw[12],retyped[12];
  U8 *var,stpassw;
	dat[len] = 0;
	//GUI_Text(23,23,dat,White,0);
	//while(1);
  switch (code) {
    case 0:
      /* Url encoded form data received. */
      break;

    default:
      /* Ignore all other codes. */
      return;
  }

  stpassw = 0;
  var = (U8 *)alloc_mem (40);
  do {
    /* Parse all returned parameters. */
    dat = http_get_var (dat, var, 40);
    if (var[0] != 0) {

      if (str_scomp (var, "menu_change=") == __TRUE) { //vemos en que pagina nos encontramos
       
        str_copy (aux_web, var+12);
		select_menu=conversor_SaI(aux_web);
		return;
      }

      if (select_menu == 1) { // camara=1

      if (str_scomp (var, "fps=") == __TRUE) {
        
        str_copy (aux_web, var+4);
		aux_data_form=conversor_SaI(aux_web);
		if (OV7670fps !=aux_data_form) {
			OV7670fps = aux_data_form;
			OV7670_FPS (OV7670fps);
		} 

      }
      else if (str_scomp (var, "barra_inp4=") == __TRUE) {
      
        str_copy (aux_web, var+11);
		aux_data_form=conversor_SaI(aux_web);
		if (OV7670contrast !=aux_data_form) {
			OV7670contrast = aux_data_form;
			OV7670_BriConLum (OV7670bright, OV7670contrast,OV7670luminis ,BCL);
		} 
      }
      else if (str_scomp (var, "barra_inp3=") == __TRUE) {
       
        str_copy (aux_web, var+11);
	   	aux_data_form=conversor_SaI(aux_web);
		if (OV7670bright !=aux_data_form) {
			OV7670bright = aux_data_form;
			OV7670_BriConLum (OV7670bright, OV7670contrast,OV7670luminis ,BCL);
		} 
      }
      else if (str_scomp (var, "barra_inp5=") == __TRUE) {
       
        str_copy (aux_web, var+11);
        aux_data_form=conversor_SaI(aux_web);
		if (OV7670luminis !=aux_data_form) {
			OV7670luminis = aux_data_form;
			OV7670_BriConLum (OV7670bright, OV7670contrast,OV7670luminis ,BCL);
		} 
      }
      else if (str_scomp (var, "barra_inp0=") == __TRUE) {
       
        str_copy (aux_web, var+11);
        aux_data_form=conversor_SaI(aux_web);
		if (OV7670R_gain !=aux_data_form) {
			OV7670R_gain = aux_data_form;
			OV7670_RGBgain (OV7670R_gain ,OV7670G_gain, OV7670B_gain);
		} 
      }
      else if (str_scomp (var, "barra_inp1=") == __TRUE) {
       
        str_copy (aux_web, var+11);
        aux_data_form=conversor_SaI(aux_web);
		if (OV7670G_gain !=aux_data_form) {
			OV7670G_gain = aux_data_form;
			OV7670_RGBgain (OV7670R_gain ,OV7670G_gain, OV7670B_gain);
		} 
      }
      else if (str_scomp (var, "barra_inp2=") == __TRUE) {
       
        str_copy (aux_web, var+11);
        aux_data_form=conversor_SaI(aux_web);
		if (OV7670B_gain !=aux_data_form) {
			OV7670B_gain = aux_data_form;
			OV7670_RGBgain (OV7670R_gain ,OV7670G_gain, OV7670B_gain);
		} 
      }
      else if (str_scomp (var, "barra_inp6=") == __TRUE) {
       
        str_copy (aux_web, var+11);
        umbral = conversor_SaI(aux_web)/8;
      }
	 else if (str_scomp (var, "RGB=") == __TRUE) {
       
        str_copy (aux_web, var+4);
        aux_data_form=conversor_SaI(aux_web);
		if (RGB !=aux_data_form) {
			RGB = aux_data_form;
			OV7670_AWBmode (RGB);
		} 
      }
	 else if (str_scomp (var, "BCL=") == __TRUE) {
       
        str_copy (aux_web, var+4);
        aux_data_form=conversor_SaI(aux_web);
		if (BCL !=aux_data_form) {
			BCL = aux_data_form;
			OV7670_BriConLum (OV7670bright, OV7670contrast,OV7670luminis ,BCL);
		} 
      }
     }
   if (select_menu == 0) { //motores==0

      if (str_scomp (var, "seg=") == __TRUE) {
        
        str_copy (aux_web, var+4);
        modo=conversor_SaI(aux_web);
      }
	  if (modo == 1) {	// Modo manual
	      if (str_scomp (var, "barra_inp1=") == __TRUE) { // movimiento del motor en el eje x
	       
	        str_copy (aux_web, var+11);
					//GUI_Text(80,50,var+11,White,0);
	        umbral_x=(float)conversor_SaI(aux_web)/255;
	      }
	      else if (str_scomp (var, "barra_inp0=") == __TRUE) { //movimiento del motor en el eje y
	       
	        str_copy (aux_web, var+11);
					//GUI_Text(20,20,var+11,White,0);
	        umbral_y=(float)conversor_SaI(aux_web)/255;
	      }
		}
     }
   
    }
    
    
  }while (dat);
  free_mem ((OS_FRAME *)var);
	//while(1);

  if (stpassw == 0x03) {
    len = strlen ((const char *)passw);
    if (mem_comp (passw, retyped, len) == __TRUE) {
      /* OK, both entered passwords the same, change it. */
      str_copy (http_auth_passw, passw);
    }
  }
}


/*--------------------------- cgi_func --------------------------------------*/

U16 cgi_func (U8 *env, U8 *buf, U16 buflen, U32 *pcgi) {
  /* This function is called by HTTP server script interpreter to make a    */
  /* formated output for 'stdout'. It returns the number of bytes written   */
  /* to the output buffer. Hi-bit of return value (len is or-ed with 0x8000)*/
  /* is a repeat flag for the system script interpreter. If this bit is set */
  /* to 1, the system will call the 'cgi_func()' again for the same script  */
  /* line with parameter 'pcgi' pointing to a 4-byte buffer. This buffer    */
  /* can be used for storing different status variables for this function.  */
  /* It is set to 0 by HTTP Server on first call and is not altered by      */
  /* HTTP server for repeated calls. This function should NEVER write more  */
  /* than 'buflen' bytes to the buffer.                                     */
  /* Parameters:                                                            */
  /*   env    - environment variable string                                 */
  /*   buf    - HTTP transmit buffer                                        */
  /*   buflen - length of this buffer (500-1400 bytes - depends on MSS)     */
  /*   pcgi   - pointer to session local buffer used for repeated loops     */
  /*            This is a U32 variable - size is 4 bytes. Value is:         */
  /*            - on 1st call = 0                                           */
  /*            - 2nd call    = as set by this function on first call       */
  U32 len = 0;
  static U32 adv;
if (select_menu ==0){// menu motores==0
  switch (env[0]) {

	/* file 'webs.cgi' */
	
      case 'a' :
          len = sprintf((char *)buf,(const char *)&env[2],modo);
      break;
      case 'b' :
          len = sprintf((char *)buf,(const char *)&env[2],det_x);
      break;
      case 'c' :
          len = sprintf((char *)buf,(const char *)&env[2],det_y);
      break;
      case 'd' :
          len = sprintf((char *)buf,(const char *)&env[2],(U8)(umbral_x*255));
      break;
      case 'e' :
          len = sprintf((char *)buf,(const char *)&env[2],(U8)(umbral_y*255));
      break;
			case 'f' :
          len = sprintf((char *)buf,(const char *)&env[2],select_menu);
      break;

      }
  }
	if (select_menu ==1){ //camara==1
		switch (env[0]) {
      case 'g' :
          len = sprintf((char *)buf,(const char *)&env[2],OV7670fps);
      break;
      case 'h' :
          len = sprintf((char *)buf,(const char *)&env[2],OV7670contrast);
      break;
      case 'i' :
          len = sprintf((char *)buf,(const char *)&env[2],OV7670bright);
      break;
      case 'd' :
          len = sprintf((char *)buf,(const char *)&env[2],OV7670G_gain);
      break;
      case 'e' :
          len = sprintf((char *)buf,(const char *)&env[2],OV7670R_gain);
      break;
      case 'f' :
          len = sprintf((char *)buf,(const char *)&env[2],select_menu);
      break;
      case 'j' :
          len = sprintf((char *)buf,(const char *)&env[2],OV7670B_gain);
      break;
      case 'k' :
          len = sprintf((char *)buf,(const char *)&env[2],OV7670luminis);
      break;
			case 'l' :
          len = sprintf((char *)buf,(const char *)&env[2],(umbral*8));
      break;
			case 'n' :
          len = sprintf((char *)buf,(const char *)&env[2],RGB);
      break;
			case 'm' :
          len = sprintf((char *)buf,(const char *)&env[2],BCL);
      break;
      }
		}
  return ((U16)len);
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
